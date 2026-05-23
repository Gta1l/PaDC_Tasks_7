#pragma once

#include <boost/asio.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/use_awaitable.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/write.hpp>

namespace asio = boost::asio;
using tcp = asio::ip::tcp;

asio::awaitable<void> echo_session(tcp::socket sock);
asio::awaitable<void> run_listener(asio::io_context& ctx, uint16_t port);
