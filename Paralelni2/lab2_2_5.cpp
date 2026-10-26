// 1.2.5 — std::async deferred vs async, n-те просте число
#include "console_utf8.hpp"
#include "primes.hpp"
#include "thread_log.hpp"

#include <chrono>
#include <cmath>
#include <future>
#include <iostream>
#include <string>

static void offer_math_on_index(int n) {
    console_print_line_utf8("Поки очікуємо результат: оберіть функцію для номера n=" +
                            std::to_string(n));
    console_write_utf8("1 — sqrt, 2 — sin, 3 — ln (інше — пропустити): ");
    int choice = 0;
    if (!(std::cin >> choice)) {
        std::cin.clear();
        return;
    }
    const double x = static_cast<double>(n);
    switch (choice) {
    case 1:
        console_print_line_utf8("sqrt(n) = " + std::to_string(std::sqrt(x)));
        break;
    case 2:
        console_print_line_utf8("sin(n) = " + std::to_string(std::sin(x)));
        break;
    case 3:
        console_print_line_utf8("ln(n) = " + std::to_string(std::log(x)));
        break;
    default:
        break;
    }
}

static void run_mode(const char* title, std::launch policy, int n) {
    log_main_line(std::string(title));
    const auto t0 = std::chrono::steady_clock::now();
    std::future<int> fut = std::async(policy, nth_prime, n);
    offer_math_on_index(n);
    const int prime = fut.get();
    const auto t1 = std::chrono::steady_clock::now();
    const auto ms =
        std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
    console_print_line_utf8(std::to_string(n) + "-те просте число: " +
                            std::to_string(prime));
    console_print_line_utf8("Час до отримання результату (мс): " +
                            std::to_string(ms));
}

int main() {
    setup_console_utf8();
    console_write_utf8("Введіть n (номер простого числа): ");
    int n = 0;
    if (!(std::cin >> n) || n <= 0) {
        n = 1000;
        console_print_line_utf8("(використано n=1000 за замовчуванням)");
    }

    run_mode("Режим std::launch::deferred", std::launch::deferred, n);
    run_mode("Режим std::launch::async", std::launch::async, n);

    console_print_line_utf8(
        "Висновок: deferred обчислює при get(); async — у фоні. "
        "Для великого n async часто дає меншу затримку на get(), "
        "якщо обчислення вже йшло під час вводу користувача.");
    return 0;
}
