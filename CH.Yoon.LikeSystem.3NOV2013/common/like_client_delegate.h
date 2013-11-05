#ifndef LIKE_CLIENT_DELEGATE_H_
#define LIKE_CLIENT_DELEGATE_H_

class LikeClientDelegate {
public:
    virtual void OnConnected(void) = 0;
    virtual void OnDisconnected(void) = 0;
    virtual void OnAlreadyLike(bool like) = 0;

protected:
    ~LikeClientDelegate(void) {}
};

#endif  // LIKE_CLIENT_DELEGATE_H_