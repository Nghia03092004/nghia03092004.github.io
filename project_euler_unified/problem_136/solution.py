"""
Problem 136: Singleton Difference

Find how many values of n < 50,000,000 have exactly one solution to
x^2 - y^2 - z^2 = n where x, y, z are consecutive terms of an AP.

n = y*(4d - y) = y*u where u = 4d - y, 0 < u < 3y, (u+y) % 4 == 0.
"""

import numpy as np

def solve(N=50_000_000):
    cnt = np.zeros(N, dtype=np.int32)

    for y in range(1, N):
        rem = (4 - y % 4) % 4
        u_start = rem if rem != 0 else 4
        u_max = min(3 * y - 1, (N - 1) // y)
        if u_start > u_max:
            continue
        # Increment cnt[y*u] for u = u_start, u_start+4, ..., <= u_max
        for u in range(u_start, u_max + 1, 4):
            cnt[y * u] += 1

    answer = int(np.sum(cnt == 1))
    return answer

if __name__ == "__main__":
    print(solve())
