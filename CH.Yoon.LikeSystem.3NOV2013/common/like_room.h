#ifndef LIKE_ROOM_H_
#define LIKE_ROOM_H_

#include "like_session_delegate.h"
#include <set>


typedef std::set<LikeSessionPtr> Guests;

class LikeRoom : public LikeSessionDelegate {
public:
    LikeRoom(void);
    ~LikeRoom(void);

    bool SetHost(LikeSessionPtr session);
    void SetGuest(LikeSessionPtr session);
    void Close(void);


private:
    virtual void OnOpen(LikeSessionPtr session, const std::string& user);
    virtual void OnClose(LikeSessionPtr session, const std::string& user);
    virtual void OnJoin(LikeSessionPtr session, const std::string& user, const std::string& target);
    virtual void OnLike(bool like);
    virtual void OnDisconnected(LikeSessionPtr session);


private:
    LikeSessionPtr host_;
    Guests guests_;
};

#endif  // LIKE_ROOM_H_