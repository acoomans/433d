#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE "MessageQueueTests"
#include <boost/test/unit_test.hpp>

#include <iostream>
#include <string>
#include <thread>
#include <set>

#include <message_queue.hpp>

void producer(int id, message_queue<std::string> &mq) {
  for (int i = 0; i < 50; ++i) {
    auto v = std::to_string(id) + "-" + std::to_string(i);
    mq.push(v);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
}

void consumer(int id, message_queue<std::string> &mq, std::set<std::string> &result) {
  for (int i = 0; i < 75; ++i) {
    auto v = mq.pop();
    result.insert(v);
    std::this_thread::sleep_for(std::chrono::milliseconds(250));
  }
}

BOOST_AUTO_TEST_CASE(message_queue_producers) {

  message_queue<std::string> q(300);

  std::thread p1(producer, 0, std::ref(q));
  std::thread p2(producer, 1, std::ref(q));
  std::thread p3(producer, 2, std::ref(q));

  p1.join();
  p2.join();
  p3.join();

  BOOST_CHECK(q.size() == 150);
}


BOOST_AUTO_TEST_CASE(message_queue_producers_consumers) {

  message_queue<std::string> q(300);

  std::set<std::string> result1;
  std::set<std::string> result2;

  std::thread p1(producer, 0, std::ref(q));
  std::thread p2(producer, 1, std::ref(q));
  std::thread p3(producer, 2, std::ref(q));

  std::thread c1(consumer, 0, std::ref(q), std::ref(result1));
  std::thread c2(consumer, 1, std::ref(q), std::ref(result2));

  p1.join();
  p2.join();
  p3.join();

  c1.join();
  c2.join();

  BOOST_CHECK(result1.size() + result2.size() == 150);
}
