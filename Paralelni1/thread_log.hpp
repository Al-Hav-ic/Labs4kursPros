#pragma once

#include "console_utf8.hpp"

#include <mutex>
#include <sstream>
#include <string>
#include <thread>

// Синхронізація лише виводу в консоль (не захищає спільні дані лабораторної).
inline std::mutex g_console_log_mutex;

inline std::string thread_id_string() {
    std::ostringstream oss;
    oss << std::this_thread::get_id();
    return oss.str();
}

inline void log_thread_begin(const std::string& role) {
    std::lock_guard<std::mutex> lock(g_console_log_mutex);
    std::string line = "\n+---------- [" + role + "] id=" + thread_id_string() +
                       " START ----------+\n";
    console_write_utf8(line);
}

inline void log_thread_line(const std::string& role, const std::string& message) {
    std::lock_guard<std::mutex> lock(g_console_log_mutex);
    std::string line = "| [" + role + "] " + message + '\n';
    console_write_utf8(line);
}

inline void log_thread_end(const std::string& role) {
    std::lock_guard<std::mutex> lock(g_console_log_mutex);
    std::string line = "+---------- [" + role + "] id=" + thread_id_string() +
                       " END ----------+\n\n";
    console_write_utf8(line);
}

inline void log_main_line(const std::string& message) {
    std::lock_guard<std::mutex> lock(g_console_log_mutex);
    std::string line = "== [main] " + message + " ==\n";
    console_write_utf8(line);
}
