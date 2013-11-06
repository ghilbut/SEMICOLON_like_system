#include "like_room.h"
#include "like_session.h"
#include <boost/assert.hpp>


LikeRoom::LikeRoom(void) 
    : host_(0) {
    printf("[INFO] room is constructed.\n");
}

LikeRoom::~LikeRoom(void) {
    printf("[INFO] room is distructed.\n");
}

bool LikeRoom::SetHost(LikeSessionPtr session) {
    if (host_.get() != 0) {
        return false;
    }
    host_ = session;
    // TODO(jh81.kim): test
    host_->Like(11);
    return true;
}

void LikeRoom::SetGuest(LikeSessionPtr session) {
    guests_.insert(session);
    session->BindDelegate(this);

    // TODO(jh81.kim): test
    session->AlreadyLike(false);
}

void LikeRoom::Close(void) {

    // TODO(jh81.kim): broadcast close

    if (host_.get() != 0) {
        host_->Close();
        host_.reset();
    }
}

void LikeRoom::OnOpen(LikeSessionPtr session, const std::string& user) {
    BOOST_ASSERT_MSG(false, "[ERROR] server event only.\n");
}

void LikeRoom::OnClose(LikeSessionPtr session, const std::string& user) {
    // TODO(jh81.kim): relay to server
}

void LikeRoom::OnJoin(LikeSessionPtr session, const std::string& user, const std::string& target) {
    BOOST_ASSERT_MSG(false, "[ERROR] server event only.\n");
}

void LikeRoom::OnLike(LikeSessionPtr session, bool like) {

    // TODO(jh81.kim): test
    // check like is correct
    session->AlreadyLike(like);
}

void LikeRoom::OnDisconnected(LikeSessionPtr session) {
    printf("[INFO] LikeRoom::OnDisconnected");
    if (host_ == session) {
        host_.reset();
    }
}