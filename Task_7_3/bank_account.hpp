#pragma once

#include <boost/asio.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/use_awaitable.hpp>
#include <boost/asio/strand.hpp>
#include <cstdint>
#include <stdexcept>
#include <atomic>

namespace asio = boost::asio;

class BankAccount
{
public:
    explicit BankAccount(asio::io_context& ctx);

    asio::awaitable<void>    async_deposit(int64_t amount);
    asio::awaitable<void>    async_withdraw(int64_t amount);
    asio::awaitable<int64_t> async_get_balance();

private:
    int64_t                                    balance_{0};
    asio::strand<asio::io_context::executor_type> strand_;
};

asio::awaitable<void> run_transactions(
    BankAccount& account,
    int64_t& total_deposited,
    int64_t& total_withdrawn,
    std::atomic<int>& done_count,
    int total_coroutines
);
