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

	roomName_.insert(std::pair<std::string, bool>("apple", false));
	roomName_.insert(std::pair<std::string, bool>("avocado", false));
	roomName_.insert(std::pair<std::string, bool>("banana", false));
	roomName_.insert(std::pair<std::string, bool>("blackberry", false));
	roomName_.insert(std::pair<std::string, bool>("cantloupe", false));
	roomName_.insert(std::pair<std::string, bool>("cherry", false));
	roomName_.insert(std::pair<std::string, bool>("chili", false));
	roomName_.insert(std::pair<std::string, bool>("coconut", false));
	roomName_.insert(std::pair<std::string, bool>("durian", false));
	roomName_.insert(std::pair<std::string, bool>("graph", false));
	roomName_.insert(std::pair<std::string, bool>("guava", false));
	roomName_.insert(std::pair<std::string, bool>("huckleberry", false));
	roomName_.insert(std::pair<std::string, bool>("honeydew", false));
	roomName_.insert(std::pair<std::string, bool>("kiwi", false));
	roomName_.insert(std::pair<std::string, bool>("lemon", false));
	roomName_.insert(std::pair<std::string, bool>("lime", false));
	roomName_.insert(std::pair<std::string, bool>("mango", false));
	roomName_.insert(std::pair<std::string, bool>("melon", false));
	roomName_.insert(std::pair<std::string, bool>("orange", false));
	roomName_.insert(std::pair<std::string, bool>("papaya", false));
	roomName_.insert(std::pair<std::string, bool>("peach", false));
	roomName_.insert(std::pair<std::string, bool>("pepper", false));
	roomName_.insert(std::pair<std::string, bool>("pear", false));
	roomName_.insert(std::pair<std::string, bool>("pineapple", false));
	roomName_.insert(std::pair<std::string, bool>("strawberry", false));
	roomName_.insert(std::pair<std::string, bool>("tomato", false));
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

void LikeServer::OnOpen(LikeSessionPtr session, const std::string& user) {
    printf("[INFO] LikeServer::OnOpen(%s)\n", user.c_str());
/*
    if (!json_.isMember(user)) {
        printf("[ERROR] invalid user name to create a room (%s).\n", user.c_str());
        return;
    }
*/
    LikeRoomPtr room;
    std::map<std::string, LikeRoomPtr>::iterator itr = rooms_.find(user);
    if (itr == rooms_.end()) {
		std::map<std::string, bool>::iterator i = roomName_.begin();
		while (i != roomName_.end()) {
			if ((*i).second == false) {
				(*i).second = true;
				break;
			}
			i++;
		}
		std::string name = (*i).first;
        printf("[INFO] create new room (%s / %s).\n", user.c_str(), name.c_str());
        room.reset(new LikeRoom(json_[user], *this));
        rooms_[user] = room;
		room.get()->set_name(name);
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
    session->BindDelegate(this);
    std::map<std::string, LikeRoomPtr>::iterator itr = rooms_.find(user);
    if (itr != rooms_.end()) {
        printf("[INFO] close room (%s).\n", user.c_str());
        //(itr->second)->Close();
		std::string name = (*itr).second.get()->name();
		roomName_[name] = false;
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
