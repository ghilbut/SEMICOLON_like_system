#include "like_server.h"
#include "like_session.h"
#include "like_room.h"
#include <boost/assert.hpp>
#include <boost/bind.hpp>


LikeServer::LikeServer(boost::asio::io_service& io_service, const Tcp::endpoint& endpoint)
    : io_service_(io_service)
    , acceptor_(io_service, endpoint) {
    acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
    start_accept();
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

void LikeServer::OnOpen(LikeSessionPtr session, const std::string& user) {
    printf("[INFO] LikeServer::OnOpen(%s)\n", user.c_str());

    LikeRoomPtr room;
    std::map<std::string, LikeRoomPtr>::iterator itr = rooms_.find(user);
    if (itr == rooms_.end()) {
        printf("[INFO] create new room (%s).\n", user.c_str());
        room.reset(new LikeRoom());
        rooms_[user] = room;
    } else {
        printf("[INFO] room is already exists (%s).\n", user.c_str());
        room = itr->second;
    }

    if (!room->SetHost(session)) {
        printf("[WARNING] host is already exists (%s).\n", user.c_str());
        session->Close();
    }
}

void LikeServer::OnClose(LikeSessionPtr session, const std::string& user) {
    std::map<std::string, LikeRoomPtr>::iterator itr = rooms_.find(user);
    if (itr != rooms_.end()) {
        printf("[INFO] close room (%s).\n", user.c_str());
        (itr->second)->Close();
        rooms_.erase(itr);
    } else {
        printf("[WARNING] room to close is not exists (%s).\n", user.c_str());
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
    room.SetGuest(session);
}

void LikeServer::OnLike(LikeSessionPtr session, bool like) {
    BOOST_ASSERT_MSG(false, "[ERROR] room only event.");
}

void LikeServer::OnDisconnected(LikeSessionPtr session) {
    session_list_.erase(session);
}