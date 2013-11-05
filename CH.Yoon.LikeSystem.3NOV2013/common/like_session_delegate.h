#ifndef LIKE_SESSION_DELEGATE_H_
#define LIKE_SESSION_DELEGATE_H_

#include <boost/shared_ptr.hpp>
#include <string>


class LikeSession;

typedef boost::shared_ptr<LikeSession> LikeSessionPtr;

class LikeSessionDelegate {
public:
    // LikeResult
    virtual void OnOpen(LikeSessionPtr session, const std::string& user) = 0;
    virtual void OnClose(LikeSessionPtr session, const std::string& user) = 0;
    // LikeClient
    virtual void OnJoin(LikeSessionPtr session, const std::string& user, const std::string& target) = 0;
    // OnLeave() // LikeClient
    virtual void OnLike(bool like) = 0;
    // common
    virtual void OnDisconnected(LikeSessionPtr session) = 0;

protected:
    ~LikeSessionDelegate(void) {}
};

#endif  // LIKE_SESSION_DELEGATE_H_