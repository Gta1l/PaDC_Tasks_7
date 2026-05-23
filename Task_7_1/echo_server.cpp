#include "echo_server.hpp"
#include <iostream>

asio::awaitable<void> echo_session(tcp::socket sock)
{
    char data[1024];
    try {
        for (;;) {
            auto [ec, n] = co_await sock.async_read_some(
                asio::buffer(data),
                asio::as_tuple(asio::use_awaitable)
            );
            if (ec == asio::error::eof) {
                std::cout << "[Session] Client disconnected (EOF)\n";
                break;
            }
            if (ec) throw boost::system::system_error(ec);
            std::cout << "[Session] Echoing " << n << " bytes\n";
            co_await async_write(sock, asio::buffer(data, n), asio::use_awaitable);
        }
    } catch (const std::exception& e) {
        std::cout << "[Session] Error: " << e.what() << "\n";
    }
}

asio::awaitable<void> run_listener(asio::io_context& ctx, uint16_t port)
{
    tcp::acceptor acceptor(ctx, tcp::endpoint(tcp::v4(), port));
    std::cout << "[Server] Listening on port " << port << "\n";
    for (;;) {
        tcp::socket sock = co_await acceptor.async_accept(asio::use_awaitable);
        std::cout << "[Server] New connection accepted\n";
        asio::co_spawn(ctx, echo_session(std::move(sock)), asio::detached);
    }
}
