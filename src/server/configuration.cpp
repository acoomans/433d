#include "configuration.hpp"

#include <exception>
#include <iostream>
#include <string>

#include <boost/foreach.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include "log.hpp"

namespace pt = boost::property_tree;

void configuration::load(const std::string &filename) {

  pt::ptree tree;

  bool read = false;

  try {
    pt::read_json(filename, tree);
    read = true;
  } catch (std::exception &e) {
    LOG_DEBUG << e.what();
  }

  if (!read) {
    try {
      pt::read_xml(filename, tree);
      read = true;
    } catch (std::exception &e) {
      LOG_DEBUG << e.what();
    }
  }

  if (!read) {
    try {
      pt::read_ini(filename, tree);
      read = true;
    } catch (std::exception &e) {
      LOG_DEBUG << e.what();
    }
  }

  if (!read) {
    LOG_WARNING << "Could not parse configuration file";
  }

  // log
  log_filename =
      tree.get<std::string>("log.filename", SERVER_DEFAULT_LOG_FILENAME);
  log_level = tree.get("log.level", 0);

  // messaging
  for (auto& kv : tree.get_child("messages")) {
    auto message = kv.first;
    auto code = kv.second.get_value("");

    messages.insert(std::make_pair(message, code));

    LOG_DEBUG << "Loading message '" << message << "' -> code '" << code << "' ";
  }

  // radio
  tx_pin = tree.get("radio.tx_pin", DEFAULT_TX_WIRINGPI_PIN);
  tx_protocol = tree.get("radio.tx_protocol", DEFAULT_TX_PROTOCOL);
  tx_pulse = tree.get("radio.tx_pulse", DEFAULT_TX_PULSE);

  // server
  port = tree.get("server.port", DEFAULT_PORT);
}