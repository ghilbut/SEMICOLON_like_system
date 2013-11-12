#ifndef LIKE_ROOM_H_
#define LIKE_ROOM_H_

#include "like_count.h"
#include "like_session_delegate.h"
#include "json/json.h"
#include <set>


typedef std::set<LikeSessionPtr> Guests;

class LikeRoom : public LikeSessionDelegate {
public:
    LikeRoom(const std::string& name, LikeSessionPtr& session, LikeSessionDelegate& delegate);
    ~LikeRoom(void);

    bool SetHost(LikeSessionPtr session);
    void SetGuest(LikeSessionPtr session, const std::string& user);
    void Close(void);

	const std::string& name(void) const;


private:
    virtual void OnOpen(LikeSessionPtr session);
    virtual void OnClose(LikeSessionPtr session, const std::string& name);
    virtual void OnJoin(LikeSessionPtr session, const std::string& user, const std::string& target);
    virtual void OnLike(LikeSessionPtr session, const std::string& user, bool like);
    virtual void OnLeave(LikeSessionPtr session);
    virtual void OnDisconnected(LikeSessionPtr session);


private:
    std::string name_;
    LikeCount count_;
    LikeSessionPtr host_;
    LikeSessionDelegate& delegate_;

    Guests guests_;
};

#endif  // LIKE_ROOM_H_