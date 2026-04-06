"""
Project Euler Problem 58: Spiral Primes

What is the side length of the square spiral for which the ratio
of primes along both diagonals first falls below 10%?

Corner values at layer k (side length s = 2k+1):
  c1 = 4k^2 - 2k + 1,  c2 = 4k^2 + 1,  c3 = 4k^2 + 2k + 1
  c4 = (2k+1)^2  (always composite, skipped)

Answer: 26241
"""
from sympy import isprime

def solve():
    prime_count = 0
    total_diag = 1  # center = 1

    k = 0
    while True:
        k += 1
        s = 2 * k + 1
        sq = s * s
        total_diag += 4

        for corner in [sq - 2 * (s - 1), sq - (s - 1), sq - 3 * (s - 1)]:
            if isprime(corner):
                prime_count += 1

        if prime_count > 0 and prime_count / total_diag < 0.10:
            return s

if __name__ == "__main__":
    answer = solve()
assert answer == 26241
print(answer)
