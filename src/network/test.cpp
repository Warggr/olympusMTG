#include "client/networkclient.h"
#include "server/networkmanager.h"
#include "server/async.h"
#include <boost/asio.hpp>
#include <catch2/catch_test_macros.hpp>
#include <thread>

TEST_CASE("Network", "[network]") {
    boost::asio::io_context context;

    auto const server_endpoint = tcp::endpoint(boost::asio::ip::make_address("127.0.0.1"), 4242);

    Server server(server_endpoint);
    AsyncNetworker& connection = server.declareAgent();
    std::thread parallel([&connection](){  });

    NetworkClient client("127.0.0.1", "Test player");
    parallel.join();

    std::string message = "Hello, world!";

    client.send(message);
    REQUIRE(server.receiveMessage() == message);

    message = "Hi, World!";
    server.send(message);
    REQUIRE(client.receiveMessage() == message);
}
