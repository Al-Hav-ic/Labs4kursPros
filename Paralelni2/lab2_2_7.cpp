// 1.2.7 — promise/future, без глобальних змінних
#include "console_utf8.hpp"
#include "primes.hpp"
#include "thread_log.hpp"

#include <chrono>
#include <cmath>
#include <future>
#include <iostream>
#include <thread>

static void thread1_work(int n, std::promise<int> prom_nth,
                         std::promise<int> prom_nth10,
                         std::promise<bool> prom_start) {
    log_thread_begin("Thread1");
    prom_nth.set_value(nth_prime(n));
    prom_start.set_value(true);
    prom_nth10.set_value(nth_prime(n * 10));
    log_thread_line("Thread1", "результати передані через promise");
    log_thread_end("Thread1");
}

static void thread2_work(int n, std::shared_future<bool> start_signal) {
    log_thread_begin("Thread2");
    start_signal.get();
    log_thread_line("Thread2", "отримано сигнал true, пауза 2 с");
    std::this_thread::sleep_for(std::chrono::seconds(2));
    const double root = std::sqrt(static_cast<double>(n));
    console_print_line_utf8("sqrt(n) = " + std::to_string(root));
    log_thread_end("Thread2");
}

int main() {
    setup_console_utf8();
    console_write_utf8("Введіть n: ");
    int n = 0;
    if (!(std::cin >> n) || n <= 0) {
        n = 10;
        console_print_line_utf8("(використано n=10)");
    }

    std::promise<int> prom_nth;
    std::promise<int> prom_nth10;
    std::promise<bool> prom_start;

    std::future<int> fut_nth = prom_nth.get_future();
    std::future<int> fut_nth10 = prom_nth10.get_future();
    std::shared_future<bool> fut_start = prom_start.get_future().share();

    std::thread t1(thread1_work, n, std::move(prom_nth), std::move(prom_nth10),
                   std::move(prom_start));
    std::thread t2(thread2_work, n, fut_start);

    t1.join();
    console_print_line_utf8(std::to_string(n) + "-те просте: " +
                            std::to_string(fut_nth.get()));
    console_print_line_utf8(std::to_string(n * 10) + "-те просте: " +
                            std::to_string(fut_nth10.get()));
    t2.join();

    log_main_line("завершення");
    return 0;
}
