#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE "ClientServerTests"
#include <boost/test/unit_test.hpp>

#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <thread>

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/chrono.hpp>

#include <client.hpp>
#include <server.hpp>

#include "mock_connection.hpp"


BOOST_AUTO_TEST_CASE(client_server_message) {

    bool ran = false;

    boost::thread t1([&](){
        boost::asio::io_context server_ioc;
        server<mock_connection> s(server_ioc, 10000);

        mock_connection::cb = [&](std::string message) {
            BOOST_CHECK(message.compare("abc") == 0);
            ran = true;
            s.stop();
        };
        s.run();
    });

    std::thread t2([](){
        boost::asio::io_context client_ioc;
        client c = client(client_ioc, "localhost", "10000");
        c.send("abc");
    });

    t1.try_join_for(boost::chrono::milliseconds(3000));
    t2.join();

    BOOST_CHECK(ran);
}

BOOST_AUTO_TEST_CASE(client_server_multiple_messages) {

    auto v = std::set<std::string>();

    boost::thread t1([&](){
        boost::asio::io_context server_ioc;
        server<mock_connection> s(server_ioc, 10001);

        mock_connection::cb = [&](std::string message) {
            v.insert(message);
            if (v.size() == 3) {
                s.stop();
            }
        };
        s.run();
    });

    std::thread t2([](){
        boost::asio::io_context client_ioc;
        client c = client(client_ioc, "localhost", "10001");
        c.send("a");
    });

    std::thread t3([](){
        boost::asio::io_context client_ioc;
        client c = client(client_ioc, "localhost", "10001");
        c.send("b");
    });

    std::thread t4([](){
        boost::asio::io_context client_ioc;
        client c = client(client_ioc, "localhost", "10001");
        c.send("c");
    });


    t2.join();
    t3.join();
    t4.join();

    t1.try_join_for(boost::chrono::milliseconds(10000));

    BOOST_CHECK(v.size() == 3);
    BOOST_CHECK(v.count("a"));
    BOOST_CHECK(v.count("b"));
    BOOST_CHECK(v.count("c"));
}
