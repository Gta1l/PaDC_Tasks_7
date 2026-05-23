#include "multiplexer.hpp"
#include <iostream>

int main()
{
    try {
        asio::io_context ctx;
        asio::co_spawn(ctx, run_demo(ctx), asio::detached);
        std::cout << "[Main] Multiplexer server started.\n";
        std::cout << "[Main] Connect two clients: telnet 127.0.0.1 13000\n";
        ctx.run();
    } catch (const std::exception& e) {
        std::cerr << "[Main] Fatal: " << e.what() << "\n";
        return 1;
    }
}
