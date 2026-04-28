"""
Project Euler Problem 196: Prime Triplets

Compute S(5678027) + S(7208785) with a segmented sieve on the relevant rows.
"""

import math
from functools import lru_cache


def row_start(n):
    return n * (n - 1) // 2 + 1


def segmented_sieve(lo, hi):
    size = hi - lo + 1
    is_prime = bytearray(b"\x01") * size
    if lo <= 0:
        for i in range(min(size, 1 - lo + 1)):
            is_prime[i] = 0
    if lo <= 1 <= hi:
        is_prime[1 - lo] = 0

    sqrt_hi = int(math.isqrt(hi)) + 2
    small = bytearray(b"\x01") * (sqrt_hi + 1)
    small[0] = small[1] = 0
    for i in range(2, sqrt_hi + 1):
        if small[i]:
            for j in range(i * i, sqrt_hi + 1, i):
                small[j] = 0
            start = max(i * i, ((lo + i - 1) // i) * i)
            for j in range(start, hi + 1, i):
                is_prime[j - lo] = 0
    return is_prime


def solve(n):
    r_min = max(1, n - 3)
    r_max = n + 3
    lo = row_start(r_min)
    hi = row_start(r_max) + r_max - 1
    sieve = segmented_sieve(lo, hi)

    def is_prime(value):
        if value < lo or value > hi:
            return False
        return sieve[value - lo] == 1

    @lru_cache(maxsize=None)
    def count_prime_neighbors(row, pos):
        count = 0
        start = row_start(row)
        value = start + pos

        if pos > 0 and is_prime(value - 1):
            count += 1
        if pos < row - 1 and is_prime(value + 1):
            count += 1

        if row > 1:
            above = row_start(row - 1)
            for delta in (-1, 0, 1):
                neighbor = pos + delta
                if 0 <= neighbor < row - 1 and is_prime(above + neighbor):
                    count += 1

        below = row_start(row + 1)
        for delta in (-1, 0, 1):
            neighbor = pos + delta
            if 0 <= neighbor < row + 1 and is_prime(below + neighbor):
                count += 1

        return count

    target_start = row_start(n)
    total = 0

    for pos in range(n):
        value = target_start + pos
        if not is_prime(value):
            continue

        if count_prime_neighbors(n, pos) >= 2:
            total += value
            continue

        found = False

        if pos > 0 and is_prime(value - 1) and count_prime_neighbors(n, pos - 1) >= 2:
            found = True
        if not found and pos < n - 1 and is_prime(value + 1) and count_prime_neighbors(n, pos + 1) >= 2:
            found = True

        if not found and n > 1:
            above = row_start(n - 1)
            for delta in (-1, 0, 1):
                neighbor = pos + delta
                if 0 <= neighbor < n - 1 and is_prime(above + neighbor):
                    if count_prime_neighbors(n - 1, neighbor) >= 2:
                        found = True
                        break

        if not found:
            below = row_start(n + 1)
            for delta in (-1, 0, 1):
                neighbor = pos + delta
                if 0 <= neighbor < n + 1 and is_prime(below + neighbor):
                    if count_prime_neighbors(n + 1, neighbor) >= 2:
                        found = True
                        break

        if found:
            total += value

    return total


def main():
    assert solve(8) == 60
    assert solve(9) == 37
    assert solve(10000) == 950007619
    print(solve(5678027) + solve(7208785))


if __name__ == "__main__":
    main()
