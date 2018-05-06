#ifndef INC_433_TRANSMITTER_H
#define INC_433_TRANSMITTER_H

#include "config.h"

#include <string>
#include <thread>

#include <RCSwitch.h>

#include "configuration.hpp"
#include "log.hpp"
#include "message_queue.hpp"


class transmitter {

public:
    typedef std::string message;

    transmitter(configuration& conf, message_queue<message>& mq, RCSwitch& sw) : conf(conf), mq(mq), sw(sw) {}

    void start() {
      sw.setProtocol(conf.tx_protocol);
      sw.setPulseLength(conf.tx_pulse);
      sw.enableTransmit(conf.tx_pin);

      should_stop = false;
      s = std::thread(&transmitter::consume, this);
      s.detach();
    }

    void stop() {
      should_stop = true;
    }

    message_queue<std::string>&getMessageQueue() {
      return mq;
    }

private:
    configuration& conf;
    message_queue<std::string>& mq;
    RCSwitch& sw;
    std::thread s;
    bool should_stop;

    void consume() {
      while(!should_stop) {
        auto message = mq.pop();
        auto code = conf.messages[message];

        LOG_INFO << "Sending code '" << code << "'";
        sw.send(std::stoi(code), 24);
      }
    }
};

#endif //INC_433_TRANSMITTER_H
