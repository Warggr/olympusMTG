#include "client/networkclient.h"
#include "server/async.h"
#include <catch2/catch_test_macros.hpp>
#include <thread>

TEST_CASE("Network", "[network]") {
    NetworkClient client;
    AsyncNetworker server;

    std::thread parallel(&AsyncNetworker::waitForConnection, &server);
    client.init("127.0.0.1", "Test player");
    parallel.join();

    std::string message = "Hello, world!";

    client.send(message);
    REQUIRE(server.receiveMessage() == message);

    message = "Hi, World!";
    server.send(message);
    REQUIRE(client.receiveMessage() == message);
}
