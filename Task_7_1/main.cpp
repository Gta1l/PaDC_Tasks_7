#include "echo_server.hpp"
#include <iostream>

int main()
{
    try {
        asio::io_context ctx;
        asio::co_spawn(ctx, run_listener(ctx, 12345), asio::detached);
        std::cout << "[Main] Echo server started. Connect with: telnet 127.0.0.1 12345\n";
        ctx.run();
    } catch (const std::exception& e) {
        std::cerr << "[Main] Fatal: " << e.what() << "\n";
        return 1;
    }
}
