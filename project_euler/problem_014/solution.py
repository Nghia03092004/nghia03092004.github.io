"""Project Euler Problem 14: Longest Collatz Sequence"""


def solve():

    LIMIT = 1_000_000
    cache = [0] * (LIMIT + 1)
    cache[1] = 1

    for i in range(2, LIMIT):
        n, steps = i, 0
        while n >= LIMIT or cache[n] == 0:
            n = n // 2 if n % 2 == 0 else 3 * n + 1
            steps += 1
        cache[i] = steps + cache[n]

    return max(range(1, LIMIT), key=lambda x: cache[x])


answer = solve()
assert answer == 837799
print(answer)
