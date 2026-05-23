#include "bank_account.hpp"
#include <boost/asio/thread_pool.hpp>
#include <iostream>
#include <vector>
#include <atomic>
#include <thread>
#include <mutex>
#include <random>

int main()
{
    const int NUM_THREADS     = 4;
    const int NUM_COROUTINES  = 100;

    asio::io_context ctx;
    auto work = asio::make_work_guard(ctx);

    BankAccount account(ctx);

    std::atomic<int> done_count{0};
    std::atomic<int64_t> total_deposited{0};
    std::atomic<int64_t> total_withdrawn{0};

    for (int i = 0; i < NUM_COROUTINES; ++i) {
        asio::co_spawn(ctx,
            [&]() -> asio::awaitable<void> {
                thread_local std::mt19937 rng{std::random_device{}()};
                std::uniform_int_distribution<int64_t> dist(1, 100);

                for (int d = 0; d < 10; ++d) {
                    int64_t amount = dist(rng);
                    co_await account.async_deposit(amount);
                    total_deposited.fetch_add(amount, std::memory_order_relaxed);
                }

                for (int w = 0; w < 10; ++w) {
                    int64_t amount = dist(rng);
                    try {
                        co_await account.async_withdraw(amount);
                        total_withdrawn.fetch_add(amount, std::memory_order_relaxed);
                    } catch (const std::invalid_argument&) {
                    }
                }

                int remaining = done_count.fetch_add(1, std::memory_order_relaxed) + 1;
                if (remaining == NUM_COROUTINES) {
                    work.reset();
                }
            },
            asio::detached
        );
    }

    std::vector<std::thread> threads;
    threads.reserve(NUM_THREADS);
    for (int i = 0; i < NUM_THREADS; ++i) {
        threads.emplace_back([&ctx] { ctx.run(); });
    }
    for (auto& t : threads) t.join();

    int64_t dep  = total_deposited.load();
    int64_t with = total_withdrawn.load();
    int64_t expected = dep - with;

    asio::io_context tmp;
    auto work2 = asio::make_work_guard(tmp);
    int64_t final_balance = 0;
    asio::co_spawn(tmp,
        [&]() -> asio::awaitable<void> {
            final_balance = co_await account.async_get_balance();
            work2.reset();
        },
        asio::detached
    );
    tmp.run();

    std::cout << "[Result] Total deposited : " << dep           << "\n";
    std::cout << "[Result] Total withdrawn : " << with          << "\n";
    std::cout << "[Result] Expected balance: " << expected      << "\n";
    std::cout << "[Result] Actual  balance : " << final_balance << "\n";
    std::cout << "[Result] Match           : " << (expected == final_balance ? "YES" : "NO") << "\n";

    return 0;
}
