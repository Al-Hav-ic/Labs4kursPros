#pragma once

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#endif

#include <iostream>
#include <mutex>
#include <string>

inline void setup_console_utf8() {
#ifdef _WIN32
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut != INVALID_HANDLE_VALUE) {
        DWORD mode = 0;
        if (GetConsoleMode(hOut, &mode)) {
            SetConsoleMode(hOut, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
        }
    }
#endif
}

inline std::once_flag g_console_encoding_once;

inline void ensure_console_utf8() {
    std::call_once(g_console_encoding_once, setup_console_utf8);
}

#ifdef _WIN32
inline void console_write_utf8(const std::string& text) {
    ensure_console_utf8();
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) {
        std::cout << text;
        return;
    }
    if (text.empty()) {
        return;
    }
    const int wideLen = MultiByteToWideChar(
        CP_UTF8, MB_ERR_INVALID_CHARS, text.data(), static_cast<int>(text.size()),
        nullptr, 0);
    if (wideLen <= 0) {
        std::cout << text;
        return;
    }
    std::wstring wide(static_cast<size_t>(wideLen), L'\0');
    MultiByteToWideChar(CP_UTF8, 0, text.data(), static_cast<int>(text.size()),
                        wide.data(), wideLen);
    DWORD written = 0;
    WriteConsoleW(hOut, wide.c_str(), static_cast<DWORD>(wideLen), &written,
                  nullptr);
}
#else
inline void console_write_utf8(const std::string& text) {
    std::cout << text;
}
#endif

inline void console_print_line_utf8(const std::string& text) {
    console_write_utf8(text + "\n");
}
