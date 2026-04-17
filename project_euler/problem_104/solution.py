"""
Problem 104: Pandigital Fibonacci Ends

Find the smallest k such that F_k has pandigital first 9 and last 9 digits.
"""
import math


def is_pandigital(n):
    s = str(n)
    return len(s) == 9 and set(s) == set("123456789")


def solve():
    MOD = 10**9
    log10_phi = math.log10((1 + math.sqrt(5)) / 2)
    log10_sqrt5 = 0.5 * math.log10(5)

    a, b = 1, 1
    for k in range(3, 1_000_000):
        a, b = b, (a + b) % MOD
        if not is_pandigital(b):
            continue
        L = k * log10_phi - log10_sqrt5
        frac = L - int(L)
        first9 = int(10.0 ** (frac + 8))
        if is_pandigital(first9):
            return k
    return -1


if __name__ == "__main__":
    answer = solve()
    print(answer)
    assert answer == 329468
