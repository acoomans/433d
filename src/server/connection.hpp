#ifndef INC_433_SESSION_H
#define INC_433_SESSION_H

#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>

#include "log.hpp"
#include "message_queue.hpp"

using boost::asio::ip::tcp;

class connection : public std::enable_shared_from_this<connection> {

public:
  typedef boost::shared_ptr<connection> pointer;

  static message_queue<std::string> *mq;

  static pointer create(boost::asio::io_context &io_context) {
    return pointer(new connection(io_context));
  }

  tcp::socket &getSocket() { return socket; }

  void start() {
    boost::asio::ip::address addr = socket.remote_endpoint().address();

    socket.async_read_some(
        boost::asio::buffer(buff),
        [this, addr](boost::system::error_code ec, std::size_t length) {
          if (!ec) {
            std::string message = std::string(this->buff.data(), length);
            LOG(info) << "Received message '" << message << "' from " << addr;
            mq->push(message);
          }
        });
  }

protected:
  tcp::socket socket;
  boost::array<char, 256> buff;

  connection(boost::asio::io_context &io_context) : socket(io_context) {}
};

#endif // INC_433_SESSION_H
