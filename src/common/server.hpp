#ifndef INC_433_TCP_SERVER_H
#define INC_433_TCP_SERVER_H

#include <boost/asio.hpp>
#include <boost/bind.hpp>

using boost::asio::ip::tcp;

template <class T>
class server {

public:
  server(boost::asio::io_context &io_context, unsigned short port)
      : io_context(io_context),
        acceptor(io_context, tcp::endpoint(tcp::v4(), port)) {
    start_accept();
  }

  const void run() { io_context.run(); }

  const void stop() { io_context.stop(); }

private:
  boost::asio::io_context &io_context;
  tcp::acceptor acceptor;

  void start_accept() {
    typename T::pointer new_connection =
        T::create(acceptor.get_executor().context());

    acceptor.async_accept(new_connection->getSocket(),
                          boost::bind(&server::handle_accept, this,
                                      new_connection,
                                      boost::asio::placeholders::error)
    );
  }

  void handle_accept(typename T::pointer new_connection,
                     const boost::system::error_code &error) {
    if (!error) {
      new_connection->start();
    }
    start_accept();
  }
};

#endif // INC_433_TCP_SERVER_H