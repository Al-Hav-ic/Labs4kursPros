// Лабораторна 1.2.6 — someData, exchangePerson; Swap: std::lock + lock_guard(adopt_lock)
#include "console_utf8.hpp"
#include "thread_log.hpp"
#include <atomic>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>

class someData {
public:
    std::string name;
    std::string surname;
    std::string address;
    int age = 0;
};

class exchangePerson {
    someData data;
    mutable std::mutex mtx;

    static void swapData(someData& a, someData& b) {
        std::swap(a.name, b.name);
        std::swap(a.surname, b.surname);
        std::swap(a.address, b.address);
        std::swap(a.age, b.age);
    }

public:
    void logState(const std::string& role, const char* title) const {
        someData copy;
        {
            std::lock_guard<std::mutex> lock(mtx);
            copy = data;
        }
        log_thread_begin(role);
        log_thread_line(role, title);
        log_thread_line(role, "ім'я: " + copy.name);
        log_thread_line(role, "прізвище: " + copy.surname);
        log_thread_line(role, "адреса: " + copy.address);
        log_thread_line(role, "вік: " + std::to_string(copy.age));
        log_thread_end(role);
    }

    static void JohnDoe(exchangePerson& obj) {
        std::lock_guard<std::mutex> lock(obj.mtx);
        obj.data.name = "John";
        obj.data.surname = "Doe";
        obj.data.address = "Unknown";
        obj.data.age = 120;
    }

    static void JacobSmith(exchangePerson& obj) {
        std::lock_guard<std::mutex> lock(obj.mtx);
        obj.data.name = "Jacob";
        obj.data.surname = "Smith";
        obj.data.address = "Known";
        obj.data.age = 1;
    }

    static void Swap(exchangePerson& a, exchangePerson& b) {
        if (&a == &b) {
            return;
        }
        std::lock(a.mtx, b.mtx);
        std::lock_guard<std::mutex> lockA(a.mtx, std::adopt_lock);
        std::lock_guard<std::mutex> lockB(b.mtx, std::adopt_lock);
        swapData(a.data, b.data);
    }
};

int main() {
    setup_console_utf8();
    exchangePerson p1;
    exchangePerson p2;

    std::atomic<bool> johnDone{false};
    std::atomic<bool> jacobDone{false};

    log_main_line("запуск потоків JohnDoe (p1) та JacobSmith (p2) з detach");
    std::thread t1([&]() {
        log_thread_begin("JohnDoe");
        exchangePerson::JohnDoe(p1);
        log_thread_line("JohnDoe", "заповнено об'єкт p1 (John Doe)");
        log_thread_end("JohnDoe");
        johnDone = true;
    });
    std::thread t2([&]() {
        log_thread_begin("JacobSmith");
        exchangePerson::JacobSmith(p2);
        log_thread_line("JacobSmith", "заповнено об'єкт p2 (Jacob Smith)");
        log_thread_end("JacobSmith");
        jacobDone = true;
    });
    t1.detach();
    t2.detach();

    while (!johnDone || !jacobDone) {
        std::this_thread::yield();
    }

    log_main_line("дані до обміну");
    p1.logState("main/p1", "Об'єкт p1");
    p2.logState("main/p2", "Об'єкт p2");

    log_main_line("запуск потоку Swap (з join)");
    std::thread tSwap([&]() {
        log_thread_begin("Swap");
        exchangePerson::Swap(p1, p2);
        log_thread_line("Swap", "обмін полів someData між p1 та p2 виконано");
        log_thread_end("Swap");
    });
    tSwap.join();

    log_main_line("дані після обміну");
    p1.logState("main/p1", "Об'єкт p1");
    p2.logState("main/p2", "Об'єкт p2");

    log_main_line("завершення main");
    return 0;
}
