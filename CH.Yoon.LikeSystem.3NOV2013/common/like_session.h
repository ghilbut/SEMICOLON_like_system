#ifndef LIKE_SESSION_H_
#define LIKE_SESSION_H_

#include "like_message.h"
#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <deque>


class LikeSessionDelegate;

typedef boost::asio::ip::tcp Tcp;
typedef std::deque<chat_message> chat_message_queue;

class LikeSession : public boost::enable_shared_from_this<LikeSession> {
public:
    LikeSession(boost::asio::io_service& io_service);
    ~LikeSession(void);

    void BindDelegate(LikeSessionDelegate* delegate);
    void Start(void);
    void Like(unsigned int count);
    void AlreadyLike(bool like);
    void Close(void);

    Tcp::socket& socket(void);


private:
    void deliver(const chat_message& msg);
    void handle_read_header(const boost::system::error_code& error);
    void handle_read_body(const boost::system::error_code& error);
    void handle_write(const boost::system::error_code& error);

    void do_write(chat_message msg);


private:
    boost::asio::io_service& io_service_;
    Tcp::socket socket_;
    chat_message read_msg_;
    chat_message_queue write_msgs_;

    LikeSessionDelegate* delegate_;
};

#endif  // LIKE_SESSION_H_