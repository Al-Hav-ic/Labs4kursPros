#pragma once

#include <cmath>

inline bool is_prime(int n) {
    if (n < 2) {
        return false;
    }
    if (n == 2) {
        return true;
    }
    if (n % 2 == 0) {
        return false;
    }
    const int limit = static_cast<int>(std::sqrt(static_cast<double>(n))) + 1;
    for (int d = 3; d < limit; d += 2) {
        if (n % d == 0) {
            return false;
        }
    }
    return true;
}

inline int nth_prime(int n) {
    if (n <= 0) {
        return 0;
    }
    int count = 0;
    int candidate = 2;
    while (count < n) {
        if (is_prime(candidate)) {
            ++count;
            if (count == n) {
                return candidate;
            }
        }
        ++candidate;
    }
    return 0;
}
