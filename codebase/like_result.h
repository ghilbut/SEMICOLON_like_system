#ifndef LIKE_RESULT_H_
#define LIKE_RESULT_H_

#include "like_message.h"
#include <boost/asio.hpp>
#include <deque>


class LikeResultDelegate;

typedef boost::asio::ip::tcp Tcp;
typedef std::deque<chat_message> chat_message_queue;

class LikeResult {
public:
    LikeResult(boost::asio::io_service& io_service, LikeResultDelegate& delegate);
    ~LikeResult(void);

    bool Connect(const char* ip, const char* port);
    void Open(void);
    void Close(void);
    void Disconnect(void);
    void Write(const chat_message& msg);


private:
    void handle_connect(const boost::system::error_code& error);
    void handle_read_header(const boost::system::error_code& error);
    void handle_read_body(const boost::system::error_code& error);
    void handle_write(const boost::system::error_code& error);

    void do_write(chat_message msg);
    void do_close(void);


private:
    boost::asio::io_service& io_service_;
    Tcp::socket socket_;
    chat_message read_msg_;
    chat_message_queue write_msgs_;

    LikeResultDelegate& delegate_;
    std::string name_;
};

#endif  // LIKE_RESULT_H_