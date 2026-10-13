// Лабораторна 1.2.3 — спільний список без м'ютекса
#include "console_utf8.hpp"
#include "thread_log.hpp"
#include <algorithm>
#include <iostream>
#include <list>
#include <sstream>
#include <thread>

std::list<int> l;
int baseValue = 0;

void AddToList() {
    const std::string role = "AddToList";
    log_thread_begin(role);
    int next = baseValue;
    for (int i = 0; i < 10; ++i) {
        l.push_back(next);
        std::ostringstream msg;
        msg << "додано елемент " << next << " (крок " << (i + 1) << "/10)";
        log_thread_line(role, msg.str());
        ++next;
    }
    log_thread_end(role);
}

void ListContains() {
    const std::string role = "ListContains";
    log_thread_begin(role);
    for (int attempt = 1; attempt <= 10; ++attempt) {
        const bool found =
            std::find(l.begin(), l.end(), baseValue) != l.end();
        std::ostringstream msg;
        msg << "спроба " << attempt << "/10: значення " << baseValue << " — "
            << (found ? "входить" : "не входить");
        log_thread_line(role, msg.str());
    }
    log_thread_end(role);
}

int main() {
    setup_console_utf8();
    log_main_line("введення базового значення для списку");
    console_write_utf8("Введіть базове значення для списку: ");
    if (!(std::cin >> baseValue)) {
        baseValue = 100;
        console_print_line_utf8("(використано значення за замовчуванням: " +
                                std::to_string(baseValue) + ")");
    }

    log_main_line("запуск потоків AddToList та ListContains");
    std::thread tAdd(AddToList);
    std::thread tCheck(ListContains);
    tAdd.join();
    tCheck.join();

    log_main_line("обидва потоки завершені");
    console_print_line_utf8("Кінцевий розмір списку: " + std::to_string(l.size()));
    return 0;
}
