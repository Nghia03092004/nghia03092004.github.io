"""
Problem 124: Ordered Radicals

Compute rad(n) for 1 <= n <= 100000 via sieve, sort by (rad(n), n),
return E(10000).
"""

def solve():
    N = 100000
    K = 10000

    rad = [1] * (N + 1)
    for i in range(2, N + 1):
        if rad[i] == 1:  # i is prime
            for j in range(i, N + 1, i):
                rad[j] *= i

    sorted_list = sorted(range(1, N + 1), key=lambda n: (rad[n], n))
    print(sorted_list[K - 1])

solve()
