#ifndef LIKE_RESULT_DELEGATE_H_
#define LIKE_RESULT_DELEGATE_H_

class LikeResultDelegate {
public:
    virtual void OnConnected(void) = 0;
    virtual void OnLikeCount(unsigned int count) = 0;
    virtual void OnClosed(void) = 0;
    virtual void OnDisconnected(void) = 0;

protected:
    ~LikeResultDelegate(void) {}
};

#endif  // LIKE_RESULT_DELEGATE_H_