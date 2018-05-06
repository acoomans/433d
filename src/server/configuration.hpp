#ifndef INC_433_CONFIGURATION_H
#define INC_433_CONFIGURATION_H

#include "config.h"

#include <string>
#include <unordered_map>

class configuration {

public:

    // log
    std::string log_filename;
    int log_level;

    // messaging
    std::unordered_map<std::string, std::string> messages;

    // radio
    int tx_pin;
    int tx_protocol;
    int tx_pulse;

    // server
    int port;

    void load(const std::string &filename);

};

#endif //INC_433_CONFIGURATION_H
