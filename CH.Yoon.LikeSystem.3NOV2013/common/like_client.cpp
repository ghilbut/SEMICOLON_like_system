#include "like_client.h"
#include "like_client_delegate.h"
#include <json/json.h>
#include <boost/bind.hpp>


LikeClient::LikeClient(LikeClientDelegate& delegate)
    : delegate_(delegate)
    , socket_(io_service_) {
    // nothing
}

LikeClient::~LikeClient(void) {
    io_service_.reset();
}

bool LikeClient::Connect(const char* ip, const char* port) {
    BOOST_ASSERT_MSG(!socket_.is_open(), "[ERROR] already connected.");
    if (socket_.is_open()) {
        return false;
    }

    io_service_.reset();
    Tcp::resolver resolver(io_service_);
    Tcp::resolver::query query(ip, port);
    Tcp::resolver::iterator iterator = resolver.resolve(query);
    boost::asio::async_connect(socket_, iterator, boost::bind(&LikeClient::handle_connect, this, boost::asio::placeholders::error));
    thread_.swap(boost::thread(boost::bind(&boost::asio::io_service::run, &io_service_)));
    thread_.detach();

    return true;
}

void LikeClient::Join(const char* user_id, const char* target) {
    user_id_ = user_id;
    target_ = target;

    Json::Value root(Json::objectValue);
    root["query"] = "join";
    root["user"] = user_id;
    root["target"] = target;
    Json::FastWriter writer;
    const std::string json = writer.write(root);

    chat_message msg;
    strcpy(msg.body(), json.c_str());
    msg.body_length(json.length());
    msg.encode_header();

    Write(msg);
}

void LikeClient::Like(bool like) {

    Json::Value root(Json::objectValue);
    root["query"] = "like";
    // root["user"] = user_id_;
    // root["target"] = target_;
    root["like"] = like;
    Json::FastWriter writer;
    const std::string json = writer.write(root);

    chat_message msg;
    strcpy(msg.body(), json.c_str());
    msg.body_length(json.length());
    msg.encode_header();

    Write(msg);
}

void LikeClient::Leave(void) {
    Json::Value root(Json::objectValue);
    root["query"] = "leave";
    Json::FastWriter writer;
    const std::string json = writer.write(root);

    chat_message msg;
    strcpy(msg.body(), json.c_str());
    msg.body_length(json.length());
    msg.encode_header();

    Write(msg);

    io_service_.post(boost::bind(&LikeClientDelegate::OnLeaved, &delegate_));
}

void LikeClient::Disconnect(void) {
    if (socket_.is_open()) {
        io_service_.post(boost::bind(&LikeClient::do_close, this));
    }
}

void LikeClient::Write(const chat_message& msg) {
    io_service_.post(boost::bind(&LikeClient::do_write, this, msg));
}




void LikeClient::handle_connect(const boost::system::error_code& error) {
    if (!error) {
        io_service_.post(boost::bind(&LikeClientDelegate::OnConnected, &delegate_));

        boost::asio::async_read(socket_
            , boost::asio::buffer(read_msg_.data(), chat_message::header_length)
            , boost::bind(&LikeClient::handle_read_header, this, boost::asio::placeholders::error));
    } else {
        printf("[ERROR] %s\n", error.message().c_str());
    }
}

void LikeClient::handle_read_header(const boost::system::error_code& error) {
    if (!error && read_msg_.decode_header()) {
        boost::asio::async_read(socket_,
        boost::asio::buffer(read_msg_.body(), read_msg_.body_length()),
        boost::bind(&LikeClient::handle_read_body, this,
        boost::asio::placeholders::error));
    } else {
        do_close();
    }
}

void LikeClient::handle_read_body(const boost::system::error_code& error)
{
    if (!error) {
        std::cout.write(read_msg_.body(), read_msg_.body_length());
        std::cout << "\n";
        boost::asio::async_read(socket_
            , boost::asio::buffer(read_msg_.data(), chat_message::header_length)
            , boost::bind(&LikeClient::handle_read_header, this, boost::asio::placeholders::error));





        // TODO(jh81.kim): update like status

        const char* begin = read_msg_.body();
        const char* end = begin + read_msg_.body_length();
        const std::string json(begin, end);

        Json::Value root(Json::objectValue);
        Json::Reader reader;
        if (!reader.parse(json, root, false)) {
            // TODO(jh81.kim): something wrong
            return;
        }

        const std::string query = root["query"].asString();
        if (query == "already_like") {
            bool like = root["like"].asBool();
            io_service_.post(boost::bind(&LikeClientDelegate::OnAlreadyLike, &delegate_, like));
        } else if (query == "close") {
            io_service_.post(boost::bind(&LikeClientDelegate::OnLeaved, &delegate_));
        } else {
            // nothing
        }





    } else {
        do_close();
    }
}

void LikeClient::handle_write(const boost::system::error_code& error) {
    if (!error) {
        write_msgs_.pop_front();
        if (!write_msgs_.empty()) {
            boost::asio::async_write(socket_
                , boost::asio::buffer(write_msgs_.front().data(), write_msgs_.front().length())
                , boost::bind(&LikeClient::handle_write, this, boost::asio::placeholders::error));
        }
    } else {
        do_close();
    }
}

void LikeClient::do_write(chat_message msg) {
    bool write_in_progress = !write_msgs_.empty();
    write_msgs_.push_back(msg);
    if (!write_in_progress) {
        boost::asio::async_write(socket_
            , boost::asio::buffer(write_msgs_.front().data(), write_msgs_.front().length())
            , boost::bind(&LikeClient::handle_write, this, boost::asio::placeholders::error));
    }
}

void LikeClient::do_close(void) {
    socket_.close();
    io_service_.post(boost::bind(&LikeClientDelegate::OnDisconnected, &delegate_));
}