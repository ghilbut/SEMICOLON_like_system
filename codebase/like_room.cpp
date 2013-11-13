#include "like_room.h"
#include "like_session.h"
#include <json/json.h>
#include <boost/assert.hpp>


LikeRoom::LikeRoom(const std::string& name, LikeSessionPtr& session, LikeSessionDelegate& delegate) 
    : name_(name), host_(session), delegate_(delegate) {
    printf("[INFO] room is constructed.\n");
    host_->BindDelegate(this);
}

LikeRoom::~LikeRoom(void) {
    printf("[INFO] room is distructed.\n");
}

void LikeRoom::SetGuest(LikeSessionPtr session, const std::string& user) {
    guests_.insert(session);
    session->BindDelegate(this);

    bool like = count_.IsLike(user);
    session->AlreadyLike(like);
}

void LikeRoom::Close(void) {

    // TODO(jh81.kim): broadcast close

    if (host_.get() != 0) {
        host_->Close();
        host_.reset();
    }
}

const std::string& LikeRoom::name(void) const {
    return name_;
}

void LikeRoom::OnOpen(LikeSessionPtr session) {
    BOOST_ASSERT_MSG(false, "[ERROR] server event only.\n");
}

void LikeRoom::OnClose(LikeSessionPtr session, const std::string& name) {
    BOOST_ASSERT_MSG(host_ == session, "[ERROR] host only could close room.\n");
    BOOST_ASSERT_MSG(name_ == name, "[ERROR] local name and event name is not matched.\n");

    Json::Value root(Json::objectValue);
    root["query"] = "close";
    Json::FastWriter writer;
    const std::string json = writer.write(root);

    chat_message msg;
    strcpy(msg.body(), json.c_str());
    msg.body_length(json.length());
    msg.encode_header();

    Guests::iterator itr = guests_.begin();
    Guests::iterator end = guests_.end();
    for (; itr != end; ++itr) {
        LikeSessionPtr& guest = const_cast<LikeSessionPtr&>(*itr);
        guest->Write(msg);
        delegate_.OnLeave(guest);
    }
    delegate_.OnClose(session, name_);
}

void LikeRoom::OnJoin(LikeSessionPtr session, const std::string& user, const std::string& target) {
    BOOST_ASSERT_MSG(false, "[ERROR] server event only.\n");
}

void LikeRoom::OnLike(LikeSessionPtr session, const std::string& user, bool like) {

    const unsigned int count = count_.Like(user, like);
    host_->Like(name_, count);
    session->AlreadyLike(like);
}

void LikeRoom::OnLeave(LikeSessionPtr session) {
    Guests::iterator itr = guests_.find(session);
    if (itr != guests_.end()) {
        guests_.erase(itr);
    }
    delegate_.OnLeave(session);
}

void LikeRoom::OnDisconnected(LikeSessionPtr session) {
    printf("[INFO] LikeRoom::OnDisconnected()\n");
    if (host_ == session) {
        OnClose(host_, name_);
    }
}
