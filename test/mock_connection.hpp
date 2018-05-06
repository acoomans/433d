#ifndef INC_433_MOCK_CONNECTION_H
#define INC_433_MOCK_CONNECTION_H

#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>

using boost::asio::ip::tcp;

class mock_connection : public std::enable_shared_from_this<mock_connection> {

public:
    typedef boost::shared_ptr<mock_connection> pointer;
    typedef std::function<void(std::string)> callback;

    static callback cb;

    static pointer create(boost::asio::io_context &io_context) {
      return pointer(new mock_connection(io_context));
    }

    tcp::socket &getSocket() { return socket; }

    void start() {
      boost::asio::ip::address ad = socket.remote_endpoint().address();

      socket.async_read_some(
              boost::asio::buffer(buff),
              [this, ad](boost::system::error_code ec, std::size_t length) {
                  if (!ec) {
                    std::string message = std::string(this->buff.data(), length);
                    mock_connection::cb(message);
                  }
              });
    }

protected:
    tcp::socket socket;
    boost::array<char, 256> buff;

    mock_connection(boost::asio::io_context &io_context) : socket(io_context) {}
};

#endif //INC_433_MOCK_CONNECTION_H
