// Лабораторна 1.2.5 — одна операція на виклик, 10 пар потоків, lock_guard
#include "console_utf8.hpp"
#include "thread_log.hpp"
#include <algorithm>
#include <chrono>
#include <iostream>
#include <list>
#include <mutex>
#include <sstream>
#include <thread>

std::list<int> l;
std::mutex mtx;

void AddToList(int value, int launchIndex) {
    std::ostringstream role;
    role << "AddToList#" << launchIndex;
    const std::string roleStr = role.str();

    log_thread_begin(roleStr);
    {
        std::lock_guard<std::mutex> guard(mtx);
        l.push_back(value);
    }
    std::ostringstream msg;
    msg << "додано значення " << value;
    log_thread_line(roleStr, msg.str());
    log_thread_end(roleStr);
}

void ListContains(int value, int launchIndex) {
    std::ostringstream role;
    role << "ListContains#" << launchIndex;
    const std::string roleStr = role.str();

    log_thread_begin(roleStr);
    bool found = false;
    {
        std::lock_guard<std::mutex> guard(mtx);
        found = std::find(l.begin(), l.end(), value) != l.end();
    }
    std::ostringstream msg;
    msg << "перевірка значення " << value << " — "
        << (found ? "входить" : "не входить");
    log_thread_line(roleStr, msg.str());
    log_thread_end(roleStr);
}

int main() {
    setup_console_utf8();
    log_main_line("введення базового значення");
    int base = 0;
    console_write_utf8(
        "Введіть базове значення (для додавання та перевірки): ");
    if (!(std::cin >> base)) {
        base = 100;
        console_print_line_utf8("(використано значення за замовчуванням: " +
                                std::to_string(base) + ")");
    }

    int current = base;
    for (int i = 0; i < 10; ++i) {
        const int launchIndex = i + 1;
        std::ostringstream info;
        info << "ітерація " << launchIndex
             << "/10: запуск AddToList(" << current << ") та ListContains("
             << base << ")";
        log_main_line(info.str());

        std::thread tAdd(AddToList, current, launchIndex);
        std::thread tCheck(ListContains, base, launchIndex);
        tAdd.detach();
        tCheck.detach();
        ++current;
    }

    log_main_line("очікування завершення detached-потоків (2 с)");
    std::this_thread::sleep_for(std::chrono::seconds(2));

    mtx.lock();
    console_print_line_utf8("Кінцевий розмір списку: " + std::to_string(l.size()));
    mtx.unlock();
    log_main_line("завершення main");
    return 0;
}
