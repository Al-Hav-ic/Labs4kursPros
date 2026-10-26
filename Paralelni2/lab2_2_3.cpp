// 1.2.3 — Thread1..3 + Notify (notify_one)
#include "console_utf8.hpp"
#include "thread_log.hpp"

#include <chrono>
#include <condition_variable>
#include <mutex>
#include <thread>

std::mutex mtx;
std::condition_variable cv;
int i = 0;

void ThreadWorker(int number) {
    const std::string role = "Thread" + std::to_string(number);
    log_thread_begin(role);
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [] { return i == 1; });
    console_print_line_utf8("Повідомлення з потоку " + std::to_string(number));
    log_thread_end(role);
}

void Notify() {
    log_thread_begin("Notify");
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    console_print_line_utf8("Notify: notify_one() після встановлення i = 1");
    {
        std::lock_guard<std::mutex> lock(mtx);
        i = 1;
    }
    cv.notify_one();
    log_thread_end("Notify");
}

int main() {
    setup_console_utf8();
    log_main_line("Thread1..3 очікують; Notify пробуджує один потік");
    std::thread t1(ThreadWorker, 1);
    std::thread t2(ThreadWorker, 2);
    std::thread t3(ThreadWorker, 3);
    std::thread notifier(Notify);
    notifier.join();
    t1.join();
    t2.join();
    t3.join();
    log_main_line("завершення");
    return 0;
}
