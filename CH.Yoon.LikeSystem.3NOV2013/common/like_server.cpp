#include "like_server.h"
#include "like_session.h"
#include "like_room.h"
#include <boost/assert.hpp>
#include <boost/bind.hpp>


LikeServer::LikeServer(Json::Value& json, boost::asio::io_service& io_service, const Tcp::endpoint& endpoint)
    : json_(json)
    , io_service_(io_service)
    , acceptor_(io_service, endpoint) {
    acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
    start_accept();

    const size_t size = json.size();
    for (size_t index = 0; index < size; ++index) {
        json.begin();
        unused_names_.insert(json[index].asString());
    }
}

void LikeServer::Stop(void) {
    std::set<LikeSessionPtr>::iterator itr = session_list_.begin();
    std::set<LikeSessionPtr>::iterator end = session_list_.end();
    for (; itr != end; ++itr) {
        (*itr)->Close();
    }
}

void LikeServer::start_accept(void) {
    LikeSessionPtr new_session(new LikeSession(io_service_));
    acceptor_.async_accept(new_session->socket()
        , boost::bind(&LikeServer::handle_accept, this, new_session, boost::asio::placeholders::error));
}

void LikeServer::handle_accept(LikeSessionPtr session, const boost::system::error_code& error) {
    if (!error) {
        printf("[INFO] session accepted.\n");
        session_list_.insert(session);
        session->BindDelegate(this);
        session->Start();
    }

    start_accept();
}

void LikeServer::OnOpen(LikeSessionPtr session) {
    printf("[INFO] LikeServer::OnOpen()\n");

    Json::Value root(Json::objectValue);
    root["query"] = "opened";
    root["success"] = false;

    if (unused_names_.empty()) {
        printf("[ERROR] every room names are used.\n");

        Json::FastWriter writer;
        const std::string json = writer.write(root);

        chat_message msg;
        msg.body_length(json.length());
        strcpy(msg.body(), json.c_str());
        msg.encode_header();
        session->Write(msg);

        return;
    }

    const std::string& name = *(unused_names_.begin());
    if (rooms_.find(name) != rooms_.end()) {
        BOOST_ASSERT(false);
        printf("[ERROR] room is already exists (%s).\n", name.c_str());

        Json::FastWriter writer;
        const std::string json = writer.write(root);

        chat_message msg;
        msg.body_length(json.length());
        strcpy(msg.body(), json.c_str());
        msg.encode_header();
        session->Write(msg);

        return;
    }

    printf("[INFO] create new room (%s).\n", name.c_str());

    LikeRoomPtr room(new LikeRoom(name, session, *this));
    rooms_[name] = room;

    root["success"] = true;
    root["name"] = name;

    Json::FastWriter writer;
    const std::string json = writer.write(root);

    chat_message msg;
    msg.body_length(json.length());
    strcpy(msg.body(), json.c_str());
    msg.encode_header();
    session->Write(msg);
}

void LikeServer::OnClose(LikeSessionPtr session, const std::string& name) {
    session->BindDelegate(this);
    std::map<std::string, LikeRoomPtr>::iterator itr = rooms_.find(name);
    if (itr != rooms_.end()) {
        printf("[INFO] close room (%s).\n", name.c_str());
        //(itr->second)->Close();
        const std::string tmp(name);
        rooms_.erase(itr);
        unused_names_.insert(tmp);
    } else {
        printf("[WARNING] room to close is not exists (%s).\n", name.c_str());
    }
}

void LikeServer::OnJoin(LikeSessionPtr session, const std::string& user, const std::string& target) {
    printf("[INFO] OnJoin(%s, %s)\n", user.c_str(), target.c_str());

    std::map<std::string, LikeRoomPtr>::iterator itr = rooms_.find(target);
    if (itr == rooms_.end()) {
        printf("[WARNING] room to join is not exists (%s).\n", target.c_str());
        return;
    }

    LikeRoom& room = *(itr->second);
    room.SetGuest(session, user);
}

void LikeServer::OnLike(LikeSessionPtr session, const std::string& user, bool like) {
    BOOST_ASSERT_MSG(false, "[ERROR] room only event.");
}

void LikeServer::OnLeave(LikeSessionPtr session) {
    session->BindDelegate(this);
}

void LikeServer::OnDisconnected(LikeSessionPtr session) {
    session_list_.erase(session);
}
