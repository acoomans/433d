#include "mock_connection.hpp"

mock_connection::callback mock_connection::cb = [](std::string message) {
    std::logic_error("Callback not set");
};
