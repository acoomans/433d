#include <config.h>

#include <iostream>
#include <string>
#include <vector>

#include <boost/asio.hpp>
#include <boost/program_options.hpp>

#include "configuration.hpp"
#include "connection.hpp"
#include "log.hpp"
#include "server.hpp"
#include "transmitter.hpp"


namespace po = boost::program_options;

using namespace std;

int main(int argc, char *argv[]) {
  try {
    po::options_description desc("server for 433Mhz communication");
    auto o = desc.add_options();
    o("help,h", "Show help message");
    o("version,v", "Show version");
    o("conf,c", po::value<string>()->default_value(SERVER_DEFAULT_CONF_FILENAME), "Configuration file");
    o("port,p", po::value<int>(), "Set port to listen on");
    o("log,l", po::value<string>(), "Log file");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help")) {
      std::cout << desc << "\n";
      return 1;
    }

    if (vm.count("version")) {
      cout << PROJECT_VERSION << "\n";
      return 1;
    }

    configuration conf;
    conf.load(vm["conf"].as<std::string>());

    log_filename = vm.count("log") ? vm["log"].as<std::string>() : conf.log_filename;

    message_queue<std::string> mq(100);
    RCSwitch sw;

    transmitter tx(conf, mq, sw);
    tx.start();

    int port = vm.count("port") ? vm["port"].as<int>() : conf.port;
    boost::asio::io_context io_context;

    connection::mq = &mq;
    server<connection> s(io_context, port);
    LOG_INFO << "433d server listening on port " << port;
    s.run();

  } catch (std::exception &e) {
    LOG_INFO << e.what() << std::endl;
    return 1;
  }

  LOG_INFO << "433d server stopped";
  return 0;
};