#include <string>
#include <vector>
#include <iostream>
// #include <redisclient/asio_headers.hpp>
#include <redisclient/redisasyncclient.h>

int main(int, char **)
{
    asio::ip::address address = asio::ip::make_address("127.0.0.1");
    const unsigned short port = 6379;
    asio::ip::tcp::endpoint endpoint(address, port);

    asio::io_context ioService;
    redisclient::RedisAsyncClient redis(ioService);
    asio::error_code ec;

    // redis.connect(endpoint, ec);

    if(ec)
    {
        std::cerr << "Can't connect to redis: " << ec.message() << std::endl;
        return EXIT_FAILURE;
    }

    redisclient::RedisValue result;

    // result = redis.command("SET", {"key", "value"});

    if( result.isError() )
    {
        std::cerr << "SET error: " << result.toString() << "\n";
        return EXIT_FAILURE;
    }

    // result = redis.command("GET", {"key"});

    if( result.isOk() )
    {
        std::cout << "GET: " << result.toString() << "\n";
        return EXIT_SUCCESS;
    }
    else
    {
        std::cerr << "GET error: " << result.toString() << "\n";
        return EXIT_FAILURE;
    }
}
