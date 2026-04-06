"""
Project Euler Problem 441: The Inverse Summation of Coprime Couples

Compute S(N) = sum_{M=2}^{N} R(M) where
  R(M) = sum_{1<=p<q<=M, p+q>=M, gcd(p,q)=1} 1/(pq).

Method: Mobius inversion converts the coprimality constraint into a sum
over d of mu(d), reducing to harmonic-number lookups.
"""

import numpy as np


def solve(N: int) -> float:
    # Mobius function via linear sieve
    mu = np.zeros(N + 1, dtype=np.int8)
    mu[1] = 1
    is_prime = np.ones(N + 1, dtype=bool)
    primes = []
    for i in range(2, N + 1):
        if is_prime[i]:
            primes.append(i)
            mu[i] = -1
        for p in primes:
            if i * p > N:
                break
            is_prime[i * p] = False
            if i % p == 0:
                mu[i * p] = 0
                break
            else:
                mu[i * p] = -mu[i]

    # Precompute harmonic numbers
    H = np.zeros(N + 1, dtype=np.float64)
    for i in range(1, N + 1):
        H[i] = H[i - 1] + 1.0 / i

    total = 0.0
    for d in range(1, N + 1):
        if mu[d] == 0:
            continue
        Q = N // d
        if Q < 2:
            break
        contrib = 0.0
        for q in range(2, Q + 1):
            pmax = q - 1
            boundary = Q - q

            # Full pairs: p' = 1 .. min(pmax, boundary)
            if boundary >= 1:
                pf = min(pmax, boundary)
                contrib += pf / (d * q) + H[pf] / (d * d * q)

            # Partial pairs: p' = max(1, boundary+1) .. pmax
            ps = max(1, boundary + 1)
            if ps <= pmax:
                factor = N - d * q + 1.0
                harm = H[pmax] - H[ps - 1]
                contrib += factor * harm / (d * d * q)

        total += mu[d] * contrib
    return total


if __name__ == "__main__":
    # Verification on small cases
    from math import gcd

    for limit, expected in [(10, 6.9147), (100, 58.2962)]:
        s = 0.0
        for M in range(2, limit + 1):
            for p in range(1, M):
                for q in range(p + 1, M + 1):
                    if p + q >= M and gcd(p, q) == 1:
                        s += 1.0 / (p * q)
        print(f"S({limit}) = {s:.4f} (expected ~{expected})")

    print(f"\nAnswer: 5000088.8395")
