cmake_minimum_required(VERSION 3.7.2)
project(433
        VERSION 0.1
        LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 11)
include(ExternalProject)

include(cmake/clang_static_analysis.cmake)


# Boost

find_package(Boost 1.66.0 REQUIRED COMPONENTS
        date_time
        log
        program_options
        regex
        serialization
        system
        thread
        unit_test_framework
        )
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIR})
if(Boost_FOUND)
    add_definitions(-DBOOST)
endif()


# Libraries

find_library(wiringPi_FOUND wiringPi)
if(wiringPi_FOUND)
    add_definitions(-DWIRINGPI)
else()
    include_directories(mock)
endif()


# External projects

find_file(rcswitch_FOUND NAMES RCSwitch.h PATHS ${CMAKE_CURRENT_SOURCE_DIR}/rc-switch/)
if(rcswitch_FOUND)
    include_directories(rc-switch)
    add_library(rcswitch
            rc-switch/RCSwitch.cpp
            rc-switch/RCSwitch.h
            )
    set_target_properties(rcswitch PROPERTIES LINKER_LANGUAGE CXX)
#    add_dependencies(rcswitch wiringPi)
    add_definitions(-DRCSWITCH)
    add_definitions(-DRPI)
endif()


# Configuration

configure_file (
        "${PROJECT_SOURCE_DIR}/src/config.h.in"
        "${PROJECT_BINARY_DIR}/config/config.h"
)
include_directories(${PROJECT_BINARY_DIR}/config)


# Common

include_directories(src/common)
add_library(433lib
        src/common/client.hpp
        src/common/server.hpp
        src/common/message_queue.hpp
        src/common/message_queue.cpp
        )
set_target_properties(433lib PROPERTIES LINKER_LANGUAGE CXX)
add_clang_static_analysis(433lib)


# client

add_executable(433c
        src/client/433c.cpp)
target_link_libraries(433c ${Boost_LIBRARIES})
add_clang_static_analysis(433c)


# server

add_executable(433d
        src/server/433d.cpp
        src/server/configuration.hpp
        src/server/configuration.cpp
        src/server/log.hpp
        src/server/log.cpp
        src/server/transmitter.cpp src/server/transmitter.hpp src/server/connection.cpp)
target_link_libraries(433d ${Boost_LIBRARIES})
if(wiringPi_FOUND)
    target_link_libraries(433d wiringPi)
endif()
if(rcswitch_FOUND)
    target_link_libraries(433d rcswitch)
endif()
add_clang_static_analysis(433d)


# tests

add_executable(client_server_tests
        test/client_server_tests.cpp
        test/mock_connection.cpp)
target_link_libraries(client_server_tests
        ${Boost_LIBRARIES}
        ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
add_test(
        NAME "client_server_tests.client_server_message"
        COMMAND client_server_tests --run_test=client_server_message --catch_system_error=yes)
add_test(
        NAME "client_server_tests.client_server_multiple_messages"
        COMMAND client_server_tests --run_test=client_server_multiple_messages --catch_system_error=yes)

add_executable(message_queue_tests
        test/message_queue_tests.cpp)
target_link_libraries(message_queue_tests
        ${Boost_LIBRARIES}
        ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
add_test(
        NAME "message_queue_tests.message_queue_producers"
        COMMAND message_queue_tests --run_test=message_queue_producers --catch_system_error=yes)
add_test(
        NAME "message_queue_tests.message_queue_producers_consumers"
        COMMAND message_queue_tests --run_test=message_queue_producers_consumers --catch_system_error=yes)