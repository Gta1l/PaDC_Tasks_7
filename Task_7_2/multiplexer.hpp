#pragma once

#include <boost/asio.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/use_awaitable.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/experimental/awaitable_operators.hpp>
#include <string>

namespace asio = boost::asio;
using tcp = asio::ip::tcp;
using namespace asio::experimental::awaitable_operators;

asio::awaitable<std::string> read_from(tcp::socket& sock, std::string name);
asio::awaitable<void> multiplexer(tcp::socket sock1, tcp::socket sock2);
asio::awaitable<void> run_demo(asio::io_context& ctx);
