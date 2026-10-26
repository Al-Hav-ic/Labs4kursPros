// 1.2.4 — як 1.2.1, синхронізація через condition_variable
#include "console_utf8.hpp"
#include "primes.hpp"
#include "thread_log.hpp"

#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>

std::queue<int> data_queue;
std::mutex mtx;
std::condition_variable cv;
bool input_done = false;

void DataPreparation() {
    log_thread_begin("DataPreparation");
    console_print_line_utf8(
        "Введіть цілі числа (завершення: не-число або Ctrl+Z):");
    int value = 0;
    while (std::cin >> value) {
        {
            std::lock_guard<std::mutex> lock(mtx);
            data_queue.push(value);
            cv.notify_one();
        }
        log_thread_line("DataPreparation", "додано: " + std::to_string(value));
    }
    std::cin.clear();
    {
        std::lock_guard<std::mutex> lock(mtx);
        input_done = true;
        cv.notify_all();
    }
    log_thread_line("DataPreparation", "введення завершено");
    log_thread_end("DataPreparation");
}

void DataProcessing() {
    log_thread_begin("DataProcessing");
    std::unique_lock<std::mutex> lock(mtx);
    for (;;) {
        cv.wait(lock, [] {
            return !data_queue.empty() || input_done;
        });
        while (!data_queue.empty()) {
            const int v = data_queue.front();
            data_queue.pop();
            lock.unlock();
            if (is_prime(v)) {
                console_print_line_utf8("просте: " + std::to_string(v));
            }
            lock.lock();
        }
        if (input_done) {
            break;
        }
    }
    log_thread_end("DataProcessing");
}

int main() {
    setup_console_utf8();
    log_main_line("DataPreparation (detach) + DataProcessing (join), condition_variable");
    std::thread prep(DataPreparation);
    prep.detach();
    std::thread proc(DataProcessing);
    proc.join();
    log_main_line("завершення");
    return 0;
}
