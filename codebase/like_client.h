#ifndef LIKE_CLIENT_H_
#define LIKE_CLIENT_H_

#include "like_message.h"
#include <boost/asio.hpp>
#include <deque>


class LikeClientDelegate;

typedef boost::asio::ip::tcp Tcp;
typedef std::deque<chat_message> chat_message_queue;

class LikeClient
{
public:
  LikeClient(boost::asio::io_service& io_service, LikeClientDelegate& delegate);
  ~LikeClient(void);

  bool Connect(const char* ip, const char* port);
  void Join(const char* user_id, const char* target);
  void Like(bool like);
  void Leave(void);
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

    LikeClientDelegate& delegate_;
    std::string user_id_;
    std::string target_;
};

#endif  // LIKE_CLIENT_H_