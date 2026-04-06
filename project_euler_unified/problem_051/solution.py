"""
Problem 51: Prime Digit Replacements

Find the smallest prime which, by replacing part of the number
(not necessarily adjacent digits) with the same digit, is part
of an eight prime value family.
"""
from itertools import combinations


def sieve(limit):
    is_prime = [False, False] + [True] * (limit - 2)
    for i in range(2, int(limit**0.5) + 1):
        if is_prime[i]:
            for j in range(i * i, limit, i):
                is_prime[j] = False
    return is_prime


def solve():
    LIMIT = 1_000_000
    is_prime = sieve(LIMIT)

    for n in range(100000, LIMIT):
        if not is_prime[n]:
            continue
        s = str(n)

        # Group positions by digit value
        digit_positions = {}
        for i, ch in enumerate(s):
            digit_positions.setdefault(ch, []).append(i)

        # Only check digits 0, 1, 2 (Theorem 2: smallest member constraint)
        for digit in ('0', '1', '2'):
            if digit not in digit_positions:
                continue
            positions = digit_positions[digit]
            for k in range(3, len(positions) + 1, 3):
                for combo in combinations(positions, k):
                    count = 0
                    smallest = None
                    for d in range(10):
                        t = list(s)
                        for pos in combo:
                            t[pos] = str(d)
                        if t[0] == '0':
                            continue
                        num = int(''.join(t))
                        if num >= 100000 and is_prime[num]:
                            count += 1
                            if smallest is None:
                                smallest = num
                    if count == 8:
                        print(smallest)
                        return

solve()
