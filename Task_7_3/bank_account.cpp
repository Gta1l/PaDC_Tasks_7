#include "bank_account.hpp"
#include <iostream>
#include <random>

BankAccount::BankAccount(asio::io_context& ctx)
    : strand_(asio::make_strand(ctx))
{}

asio::awaitable<void> BankAccount::async_deposit(int64_t amount)
{
    co_await asio::post(strand_, asio::use_awaitable);
    balance_ += amount;
}

asio::awaitable<void> BankAccount::async_withdraw(int64_t amount)
{
    co_await asio::post(strand_, asio::use_awaitable);
    if (amount > balance_)
        throw std::invalid_argument("Insufficient funds");
    balance_ -= amount;
}

asio::awaitable<int64_t> BankAccount::async_get_balance()
{
    co_await asio::post(strand_, asio::use_awaitable);
    co_return balance_;
}

asio::awaitable<void> run_transactions(
    BankAccount& account,
    int64_t& total_deposited,
    int64_t& total_withdrawn,
    std::atomic<int>& done_count,
    int total_coroutines)
{
    thread_local std::mt19937 rng{std::random_device{}()};
    std::uniform_int_distribution<int64_t> dist(1, 100);

    for (int i = 0; i < 10; ++i) {
        int64_t amount = dist(rng);
        co_await account.async_deposit(amount);
        total_deposited += amount;
    }

    for (int i = 0; i < 10; ++i) {
        int64_t amount = dist(rng);
        try {
            co_await account.async_withdraw(amount);
            total_withdrawn += amount;
        } catch (const std::invalid_argument&) {
        }
    }

    int remaining = done_count.fetch_add(1, std::memory_order_relaxed) + 1;
    if (remaining == total_coroutines) {
        std::cout << "[Bank] All coroutines finished.\n";
    }
}
