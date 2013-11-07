#ifndef LIKE_ROOM_H_
#define LIKE_ROOM_H_

#include "like_count.h"
#include "like_session_delegate.h"
#include "json/json.h"
#include <set>


typedef std::set<LikeSessionPtr> Guests;

class LikeRoom : public LikeSessionDelegate {
public:
    LikeRoom(Json::Value& json, LikeSessionDelegate& delegate);
    ~LikeRoom(void);

    bool SetHost(LikeSessionPtr session);
    void SetGuest(LikeSessionPtr session, const std::string& user);
    void Close(void);


private:
    virtual void OnOpen(LikeSessionPtr session, const std::string& user);
    virtual void OnClose(LikeSessionPtr session, const std::string& user);
    virtual void OnJoin(LikeSessionPtr session, const std::string& user, const std::string& target);
    virtual void OnLike(LikeSessionPtr session, const std::string& user, bool like);
    virtual void OnLeave(LikeSessionPtr session);
    virtual void OnDisconnected(LikeSessionPtr session);


private:
    LikeCount count_;
    LikeSessionDelegate& delegate_;
    LikeSessionPtr host_;
    Guests guests_;
};

#endif  // LIKE_ROOM_H_