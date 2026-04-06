"""
Problem 193: Squarefree Numbers

Count squarefree numbers below 2^50.
Uses: Q(N) = sum_{k=1}^{sqrt(N)} mu(k) * floor(N/k^2)
"""

def solve():
    N = (1 << 50) - 1
    SQ = 1 << 25  # 2^25 = 33554432

    # Sieve Mobius function up to SQ
    mu = [0] * (SQ + 1)
    mu[1] = 1

    # Use a sieve: for each prime p, multiply mu by -1 for multiples,
    # set mu=0 for multiples of p^2
    is_prime = bytearray(b'\x01') * (SQ + 1)
    is_prime[0] = is_prime[1] = 0
    mu = [1] * (SQ + 1)

    for i in range(2, SQ + 1):
        if is_prime[i]:
            for j in range(i, SQ + 1, i):
                if j != i:
                    is_prime[j] = 0
                mu[j] *= -1
            i2 = i * i
            for j in range(i2, SQ + 1, i2):
                mu[j] = 0

    ans = 0
    for k in range(1, SQ + 1):
        if mu[k] != 0:
            ans += mu[k] * (N // (k * k))

    return ans

print(solve())
