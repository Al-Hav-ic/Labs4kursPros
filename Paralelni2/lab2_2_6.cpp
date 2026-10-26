// 1.2.6 — packaged_task, два деки, робочий потік
#include "console_utf8.hpp"
#include "primes.hpp"
#include "thread_log.hpp"

#include <condition_variable>
#include <deque>
#include <future>
#include <iostream>
#include <mutex>
#include <sstream>
#include <thread>
#include <vector>

struct TaskQueues {
    std::mutex mtx;
    std::condition_variable cv;
    std::deque<std::packaged_task<void()>> tasks;
    std::deque<int> indices;
    bool stop = false;
};

static void worker_loop(TaskQueues& q) {
    log_thread_begin("Worker");
    for (;;) {
        std::packaged_task<void()> task;
        {
            std::unique_lock<std::mutex> lock(q.mtx);
            q.cv.wait(lock, [&] { return !q.tasks.empty() || q.stop; });
            if (q.stop && q.tasks.empty()) {
                break;
            }
            task = std::move(q.tasks.front());
            q.tasks.pop_front();
            q.indices.pop_front();
        }
        task();
    }
    log_thread_end("Worker");
}

int main() {
    setup_console_utf8();
    TaskQueues q;
    std::vector<std::future<void>> done;

    std::thread worker(worker_loop, std::ref(q));

    console_print_line_utf8(
        "Вводьте n (номер простого). Стоп-символ: q або від'ємне число.");
    std::string token;
    while (std::cin >> token) {
        if (token == "q" || token == "Q") {
            break;
        }
        int n = 0;
        try {
            n = std::stoi(token);
        } catch (...) {
            console_print_line_utf8("Пропущено: " + token);
            continue;
        }
        if (n < 0) {
            break;
        }

        std::packaged_task<void()> task([n]() {
            const int p = nth_prime(n);
            console_print_line_utf8(std::to_string(n) + "-те просте число: " +
                                    std::to_string(p));
        });
        done.push_back(task.get_future());
        {
            std::lock_guard<std::mutex> lock(q.mtx);
            q.indices.push_back(n);
            q.tasks.push_back(std::move(task));
            q.cv.notify_one();
        }
        log_main_line("додано завдання для n=" + std::to_string(n));
    }

    {
        std::lock_guard<std::mutex> lock(q.mtx);
        q.stop = true;
        q.cv.notify_all();
    }
    worker.join();

    for (auto& f : done) {
        f.get();
    }
    log_main_line("усі обчислення завершені");
    return 0;
}
