#ifndef INC_433_CLIENT_H
#define INC_433_CLIENT_H

#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class client {

public:
  client(boost::asio::io_context &io_context, std::string host,
         std::string port)
      : io_context(io_context), host(host), port(port) {}

  void send(std::string message) {
    tcp::resolver resolver(io_context);
    tcp::resolver::results_type endpoints = resolver.resolve(host, port);

    tcp::socket socket(io_context);
    boost::asio::connect(socket, endpoints);

    boost::system::error_code error;

    const char *mc = message.c_str();
    int ml = std::strlen(mc);

    socket.write_some(boost::asio::buffer(mc, ml), error);

    if (error && error != boost::asio::error::eof) {
      throw boost::system::system_error(error);
    }
    socket.close();
  }

private:
  std::string host;
  std::string port;
  boost::asio::io_context &io_context;
};

#endif // INC_433_CLIENT_H
