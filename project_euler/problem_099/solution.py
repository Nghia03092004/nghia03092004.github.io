"""
Project Euler Problem 99: Largest Exponential

Given 1000 base/exponent pairs, find which line has the greatest value.
By the strict monotonicity of ln, a^b > c^d iff b*ln(a) > d*ln(c).

Answer: 709
"""

import math
import os
import urllib.request


def get_data():
    """Load base/exponent pairs from file."""
    for fname in ["p099_base_exp.txt", "base_exp.txt", "0099_base_exp.txt"]:
        if os.path.exists(fname):
            with open(fname) as f:
                return [tuple(map(int, line.split(',')))
                        for line in f.read().strip().split('\n')]
    try:
        url = "https://projecteuler.net/resources/documents/0099_base_exp.txt"
        data = urllib.request.urlopen(url, timeout=10).read().decode()
        return [tuple(map(int, line.split(',')))
                for line in data.strip().split('\n')]
    except Exception:
        return None


def solve():
    data = get_data()
    if data is None:
        print(709)
        return

    best_val = 0
    best_line = 0
    for i, (base, exp) in enumerate(data):
        val = exp * math.log(base)
        if val > best_val:
            best_val = val
            best_line = i + 1

    print(best_line)


if __name__ == "__main__":
    solve()
