// 1.2.2 — condition_variable, Waits x3, Awake
#include "console_utf8.hpp"
#include "thread_log.hpp"

#include <chrono>
#include <condition_variable>
#include <mutex>
#include <thread>

std::mutex mtx;
std::condition_variable cv;
int i = 0;

void Waits(int id) {
    log_thread_begin("Waits#" + std::to_string(id));
    std::unique_lock<std::mutex> lock(mtx);
    console_print_line_utf8("Потік Waits входить у стан очікування (id=" +
                            std::to_string(id) + ")");
    cv.wait(lock, [] { return i == 1; });
    console_print_line_utf8("Потік Waits: очікування завершене (id=" +
                            std::to_string(id) + ")");
    log_thread_end("Waits#" + std::to_string(id));
}

void Awake() {
    log_thread_begin("Awake");
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    console_print_line_utf8("Awake: починаю повідомляти решту (notify_all #1)");
    cv.notify_all();

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    {
        std::lock_guard<std::mutex> lock(mtx);
        i = 1;
    }
    console_print_line_utf8("Awake: i = 1, повторне повідомлення (notify_all #2)");
    cv.notify_all();
    log_thread_end("Awake");
}

int main() {
    setup_console_utf8();
    log_main_line("три потоки Waits + потік Awake (join Awake, потім Waits)");
    std::thread t1(Waits, 1);
    std::thread t2(Waits, 2);
    std::thread t3(Waits, 3);
    std::thread awake(Awake);
    awake.join();
    t1.join();
    t2.join();
    t3.join();
    log_main_line("завершення");
    return 0;
}
