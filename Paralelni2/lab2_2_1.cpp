// 1.2.1 — прапорець + unique_lock + sleep_for
#include "console_utf8.hpp"
#include "primes.hpp"
#include "thread_log.hpp"

#include <chrono>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>

std::queue<int> data_queue;
std::mutex mtx;
bool input_done = false;

void DataPreparation() {
    log_thread_begin("DataPreparation");
    console_print_line_utf8(
        "Введіть цілі числа (завершення: не-число, напр. літера або Ctrl+Z):");
    int value = 0;
    while (std::cin >> value) {
        {
            std::lock_guard<std::mutex> lock(mtx);
            data_queue.push(value);
        }
        log_thread_line("DataPreparation", "додано: " + std::to_string(value));
    }
    std::cin.clear();
    {
        std::lock_guard<std::mutex> lock(mtx);
        input_done = true;
    }
    log_thread_line("DataPreparation", "введення завершено, прапорець піднято");
    log_thread_end("DataPreparation");
}

void DataProcessing() {
    log_thread_begin("DataProcessing");
    std::unique_lock<std::mutex> lock(mtx);
    while (!input_done) {
        lock.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        lock.lock();
    }
    log_thread_line("DataProcessing", "прапорець встановлено — обробка черги");
    std::queue<int> copy = data_queue;
    lock.unlock();

    console_print_line_utf8("Прості числа з черги:");
    while (!copy.empty()) {
        const int v = copy.front();
        copy.pop();
        if (is_prime(v)) {
            console_print_line_utf8(std::to_string(v));
        }
    }
    log_thread_end("DataProcessing");
}

int main() {
    setup_console_utf8();
    log_main_line("запуск DataPreparation (detach) та DataProcessing (join)");
    std::thread prep(DataPreparation);
    prep.detach();
    std::thread proc(DataProcessing);
    proc.join();
    log_main_line("завершення");
    return 0;
}
