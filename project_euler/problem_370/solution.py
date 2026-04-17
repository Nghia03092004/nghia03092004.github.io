"""
Project Euler Problem 370: Geometric Triangles

Count triangles with integer sides a <= b <= c, b^2 = ac (geometric
progression), and a + b + c <= L.

Parametrization: a = k*m^2, b = k*m*n, c = k*n^2 with gcd(m,n)=1, m <= n.
Triangle inequality: n/m < phi = (1+sqrt(5))/2.
Perimeter: k*(m^2 + m*n + n^2) <= L.

Sum over coprime (m,n) with m <= n < phi*m of floor(L / (m^2+m*n+n^2)).

Answer: 41791929448408
"""

import math

def solve():
    L = 2500000000000  # 2.5 * 10^12
    PHI = (1 + math.sqrt(5)) / 2

    total = 0
    max_m = int(math.isqrt(L // 3)) + 1

    for m in range(1, max_m + 1):
        # Upper bound on n from triangle inequality: n < phi * m
        n_upper_tri = int(PHI * m - 1e-9)

        # Upper bound on n from perimeter: m^2 + m*n + n^2 <= L
        disc = 4 * L - 3 * m * m
        if disc < 0:
            break
        n_upper_per = int((-m + math.isqrt(disc)) // 2)

        n_upper = min(n_upper_tri, n_upper_per)
        if n_upper < m:
            continue

        for n in range(m, n_upper + 1):
            if math.gcd(m, n) != 1:
                continue

            # Verify triangle inequality: m^2 + m*n > n^2
            if m * m + m * n <= n * n:
                continue

            q = m * m + m * n + n * n
            total += L // q

    print(total)

if __name__ == "__main__":
    solve()
