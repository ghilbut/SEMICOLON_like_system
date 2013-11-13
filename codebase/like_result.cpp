#include "like_result.h"
#include "like_result_delegate.h"
#include <json/json.h>
#include <boost/assert.hpp>
#include <boost/bind.hpp>


LikeResult::LikeResult(boost::asio::io_service& io_service, LikeResultDelegate& delegate)
    : io_service_(io_service)
    , socket_(io_service_) 
    , delegate_(delegate) {
    // nothing
}

LikeResult::~LikeResult(void) {
    // nothing
}

bool LikeResult::Connect(const char* ip, const char* port) {
    BOOST_ASSERT(!socket_.is_open());
    if (socket_.is_open()) {
        return false;
    }

    Tcp::resolver resolver(io_service_);
    Tcp::resolver::query query(ip, port);
    Tcp::resolver::iterator iterator = resolver.resolve(query);
    boost::asio::async_connect(socket_, iterator, boost::bind(&LikeResult::handle_connect, this, boost::asio::placeholders::error));

    return true;
}

void LikeResult::Open(void) {

    Json::Value root(Json::objectValue);
    root["query"] = "open";
    Json::FastWriter writer;
    const std::string json = writer.write(root);

    chat_message msg;
    strcpy(msg.body(), json.c_str());
    msg.body_length(json.length());
    msg.encode_header();

    Write(msg);
}

void LikeResult::Close(void) {

    Json::Value root(Json::objectValue);
    root["query"] = "close";
    root["name"] = name_;
    Json::FastWriter writer;
    const std::string json = writer.write(root);

    chat_message msg;
    strcpy(msg.body(), json.c_str());
    msg.body_length(json.length());
    msg.encode_header();
    Write(msg);

    io_service_.post(boost::bind(&LikeResultDelegate::OnClosed, &delegate_));
}

void LikeResult::Disconnect(void) {
    io_service_.post(boost::bind(&LikeResult::do_close, this));
}

void LikeResult::Write(const chat_message& msg) {
    io_service_.post(boost::bind(&LikeResult::do_write, this, msg));
}


void LikeResult::handle_connect(const boost::system::error_code& error) {
    if (!error) {
        boost::asio::async_read(socket_
            , boost::asio::buffer(read_msg_.data(), chat_message::header_length)
            , boost::bind(&LikeResult::handle_read_header, this, boost::asio::placeholders::error));

        io_service_.post(boost::bind(&LikeResultDelegate::OnConnected, &delegate_));
    } else {
        //printf("[ERROR] %s\n", error.message().c_str());
        do_close();
    }
}

void LikeResult::handle_read_header(const boost::system::error_code& error) {
    if (!error && read_msg_.decode_header()) {
        boost::asio::async_read(socket_
            , boost::asio::buffer(read_msg_.body(), read_msg_.body_length())
            , boost::bind(&LikeResult::handle_read_body, this, boost::asio::placeholders::error));
    } else {
        do_close();
    }
}

void LikeResult::handle_read_body(const boost::system::error_code& error) {
    if (!error) {
        std::cout.write(read_msg_.body(), read_msg_.body_length());
        std::cout << "\n";
        boost::asio::async_read(socket_
            , boost::asio::buffer(read_msg_.data(), chat_message::header_length)
            , boost::bind(&LikeResult::handle_read_header, this, boost::asio::placeholders::error));





        // TODO(jh81.kim): update like count

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
        if (query == "opened") {
            const Json::Value& name = root["name"];
            if (name.isString()) {
                name_ = name.asString();
                io_service_.post(boost::bind(&LikeResultDelegate::OnOpened, &delegate_, name_));
            }
        } else if (query == "like") {
            const unsigned int count = root["count"].asUInt();
            io_service_.post(boost::bind(&LikeResultDelegate::OnLikeCount, &delegate_, count));
        } else if (query == "closed") {
            io_service_.post(boost::bind(&LikeResultDelegate::OnClosed, &delegate_));
        } else {
            // nothing
        }





    } else {
        do_close();
    }
}

void LikeResult::handle_write(const boost::system::error_code& error)
{
    if (!error) {
        write_msgs_.pop_front();
        if (!write_msgs_.empty()) {
            boost::asio::async_write(socket_
                , boost::asio::buffer(write_msgs_.front().data(), write_msgs_.front().length())
                , boost::bind(&LikeResult::handle_write, this, boost::asio::placeholders::error));
        }
    } else {
        do_close();
    }
}

void LikeResult::do_write(chat_message msg) {
    bool write_in_progress = !write_msgs_.empty();
    write_msgs_.push_back(msg);
    if (!write_in_progress) {
        boost::asio::async_write(socket_
            , boost::asio::buffer(write_msgs_.front().data(), write_msgs_.front().length())
            , boost::bind(&LikeResult::handle_write, this, boost::asio::placeholders::error));
    }
}

void LikeResult::do_close(void) {
    if (socket_.is_open()) {
        socket_.close();
        delegate_.OnDisconnected();
    }
}