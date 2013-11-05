#ifndef LIKE_SERVER_H_
#define LIKE_SERVER_H_

#include "like_room.h"
#include "like_session_delegate.h"
#include <boost/asio.hpp>
#include <set>
#include <map>


class LikeRoom;

typedef boost::asio::ip::tcp Tcp;
typedef boost::shared_ptr<LikeRoom> LikeRoomPtr;

class LikeServer : public LikeSessionDelegate {
public:
    LikeServer(boost::asio::io_service& io_service, const Tcp::endpoint& endpoint);
    void start_accept(void);
    void handle_accept(LikeSessionPtr session, const boost::system::error_code& error);

    // LikeSessionDelegate
    virtual void OnOpen(LikeSessionPtr session, const std::string& user);
    virtual void OnClose(LikeSessionPtr session, const std::string& user);
    virtual void OnJoin(LikeSessionPtr session, const std::string& user, const std::string& target);
    // virtual void OnLeave();
    virtual void OnLike(bool like);
    virtual void OnDisconnected(LikeSessionPtr session);

private:
    boost::asio::io_service& io_service_;
    Tcp::acceptor acceptor_;

    std::set<LikeSessionPtr> session_list_;
    std::map<std::string, LikeRoomPtr> rooms_;
};

#endif  // LIKE_SERVER_H_