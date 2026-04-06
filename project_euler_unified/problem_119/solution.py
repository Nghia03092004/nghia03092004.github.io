"""
Problem 119: Digit Power Sum

Find the 30th term of the sequence where a number >= 10 equals
a power of its own digit sum.
"""

def digit_sum(n):
    return sum(int(d) for d in str(n))

def solve():
    LIMIT = 10**18
    results = []

    for s in range(2, 501):
        power = s * s  # start at s^2
        k = 2
        while power <= LIMIT:
            if power >= 10 and digit_sum(power) == s:
                results.append(power)
            k += 1
            power *= s

    results = sorted(set(results))
    return results[29]  # 0-indexed, so index 29 = a_30

answer = solve()
assert answer == 248155780267521
print(answer)
