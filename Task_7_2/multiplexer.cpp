#include "multiplexer.hpp"
#include <iostream>
#include <variant>

asio::awaitable<std::string> read_from(tcp::socket& sock, std::string name)
{
    char buf[1024];
    auto [ec, n] = co_await sock.async_read_some(
        asio::buffer(buf),
        asio::as_tuple(asio::use_awaitable)
    );
    if (ec == asio::error::eof) {
        std::cout << "[" << name << "] Connection closed\n";
        co_return std::string{};
    }
    if (ec) throw boost::system::system_error(ec);
    co_return "[" + name + "] " + std::string(buf, n);
}

asio::awaitable<void> multiplexer(tcp::socket sock1, tcp::socket sock2)
{
    try {
        for (;;) {
            auto result = co_await (
                read_from(sock1, "sock1") || read_from(sock2, "sock2")
            );
            std::visit([](auto& val) {
                if (!val.empty()) std::cout << val;
            }, result);
        }
    } catch (const std::exception& e) {
        std::cout << "[Multiplexer] Stopped: " << e.what() << "\n";
    }
}

asio::awaitable<void> run_demo(asio::io_context& ctx)
{
    tcp::acceptor acc(ctx, tcp::endpoint(tcp::v4(), 13000));
    std::cout << "[Demo] Waiting for first client on port 13000...\n";
    tcp::socket s1 = co_await acc.async_accept(asio::use_awaitable);
    std::cout << "[Demo] First client connected\n";

    std::cout << "[Demo] Waiting for second client on port 13000...\n";
    tcp::socket s2 = co_await acc.async_accept(asio::use_awaitable);
    std::cout << "[Demo] Second client connected. Starting multiplexer...\n";

    co_await multiplexer(std::move(s1), std::move(s2));
}
