#include "like_room.h"
#include "like_session.h"


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
    return true;
}

void LikeRoom::SetGuest(LikeSessionPtr session) {
    guests_.insert(session);
    session->BindDelegate(this);
}

void LikeRoom::Close(void) {

    // TODO(jh81.kim): broadcast close

    if (host_.get() != 0) {
        host_->Close();
        host_.reset();
    }
}

void LikeRoom::OnOpen(LikeSessionPtr session, const std::string& user) {
    // nothing, server only event
}

void LikeRoom::OnClose(LikeSessionPtr session, const std::string& user) {
    // nothing, server only event
}

void LikeRoom::OnJoin(LikeSessionPtr session, const std::string& user, const std::string& target) {
    // nothing, server only event
}

void LikeRoom::OnLike(bool like) {
    printf("[INFO] LikeRoom::OnLike");
}

void LikeRoom::OnDisconnected(LikeSessionPtr session) {
    printf("[INFO] LikeRoom::OnDisconnected");
}