#ifndef LIKE_SESSION_DELEGATE_H_
#define LIKE_SESSION_DELEGATE_H_

#include <boost/shared_ptr.hpp>
#include <string>


class LikeSession;

typedef boost::shared_ptr<LikeSession> LikeSessionPtr;

class LikeSessionDelegate {
public:
    // LikeResult
    virtual void OnOpen(LikeSessionPtr session) = 0;
    virtual void OnClose(LikeSessionPtr session, const std::string& name) = 0;
    // LikeClient
    virtual void OnJoin(LikeSessionPtr session, const std::string& user, const std::string& target) = 0;
    virtual void OnLike(LikeSessionPtr session, const std::string& user, bool like) = 0;
    virtual void OnLeave(LikeSessionPtr session) = 0;
    // common
    virtual void OnDisconnected(LikeSessionPtr session) = 0;

protected:
    ~LikeSessionDelegate(void) {}
};

#endif  // LIKE_SESSION_DELEGATE_H_