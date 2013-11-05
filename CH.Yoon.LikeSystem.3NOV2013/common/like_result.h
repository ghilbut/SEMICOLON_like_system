#ifndef LIKE_RESULT_H_
#define LIKE_RESULT_H_

#include "like_message.h"
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <deque>


class LikeResultDelegate;

typedef boost::asio::ip::tcp Tcp;
typedef std::deque<chat_message> chat_message_queue;

class LikeResult {
public:
    LikeResult(LikeResultDelegate& delegate);
    ~LikeResult(void);

    void Open(const char* ip, const char* port, const char* user_id);
    void Close(void);
    void Write(const chat_message& msg);


private:
    void handle_connect(const boost::system::error_code& error);
    void handle_read_header(const boost::system::error_code& error);
    void handle_read_body(const boost::system::error_code& error);
    void handle_write(const boost::system::error_code& error);

    void do_write(chat_message msg);
    void do_close(void);


private:
    std::string user_id_;
    LikeResultDelegate& delegate_;

    boost::asio::io_service io_service_;
    boost::thread thread_;

    Tcp::socket socket_;
    chat_message read_msg_;
    chat_message_queue write_msgs_;
};

#endif  // LIKE_RESULT_H_