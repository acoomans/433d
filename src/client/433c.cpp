#include <config.h>

#include <iostream>
#include <string>
#include <vector>

#include <boost/program_options.hpp>

#include "client.hpp"

namespace po = boost::program_options;

int main(int argc, char *argv[]) {
  try {

    po::options_description desc("client for 433d server");
    desc.add_options()("help,h", "Show help message")("version,v",
                                                      "Show version")(
        "host,h", po::value<std::string>()->default_value(CLIENT_DEFAULT_HOST),
        "Set host to connect to")(
        "port,p", po::value<std::string>()->default_value(CLIENT_DEFAULT_PORT),
        "Set port to connect to")("code", po::value<std::string>()->required(),
                                  "Code to send");

    po::positional_options_description positionalOptions;
    positionalOptions.add("code", 1);

    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv)
                  .options(desc)
                  .positional(positionalOptions)
                  .run(),
              vm);
    po::notify(vm);

    if (vm.count("help")) {
      std::cout << desc << "\n";
      return 0;
    }

    if (vm.count("version")) {
      std::cout << PROJECT_VERSION << "\n";
      return 0;
    }

    const std::string host = vm["host"].as<std::string>();
    const std::string port = vm["port"].as<std::string>();
    const std::string code = vm["code"].as<std::string>();

    std::cout << "Sending code '" << code << "' to " << host << ":" << port
              << "\n";

    boost::asio::io_context io_context;
    client c = client(io_context, host, port);
    c.send(code);

  } catch (std::exception &e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }

  return 0;
};