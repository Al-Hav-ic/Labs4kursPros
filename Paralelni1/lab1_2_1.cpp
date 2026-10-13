// Лабораторна 1.2.1 — потоки без join() та без detach()
#include "console_utf8.hpp"
#include "thread_log.hpp"
#include <thread>

void Thread1() {
    log_thread_begin("Thread1");
    log_thread_line("Thread1", "результат виведення: 1");
    log_thread_end("Thread1");
}

void Thread2() {
    log_thread_begin("Thread2");
    log_thread_line("Thread2", "результат виведення: 2");
    log_thread_end("Thread2");
}

int main() {
    setup_console_utf8();
    log_main_line("створення потоків Thread1 та Thread2 (без join/detach)");
    std::thread t1(Thread1);
    std::thread t2(Thread2);
    log_main_line("завершення main (потоки можуть не встигнути / можливий terminate)");
    return 0;
}
