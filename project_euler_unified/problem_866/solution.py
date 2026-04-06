"""
Project Euler 866: Tidying Up B

A caterpillar of N=100 pieces is assembled randomly. When a segment of
length k is formed, H_k = k*(2k-1) is recorded. We want the expected
value of the product of all recorded hexagonal numbers, mod 987654319.

Key insight: Consider which piece is placed LAST in each interval [a,b].
If piece at position i is placed last in interval [a,b] of length L=b-a+1,
then it contributes H_L to the product. The sub-intervals [a,i-1] and
[i+1,b] are filled independently.

E(L) = H_L / L * sum_{i=0}^{L-1} E(i) * E(L-1-i)
E(0) = 1
"""

MOD = 987654319

def solve():
    N = 100

    def H(k):
        return k * (2 * k - 1) % MOD

    E = [0] * (N + 1)
    E[0] = 1

    for L in range(1, N + 1):
        s = 0
        for i in range(L):
            s = (s + E[i] * E[L - 1 - i]) % MOD
        E[L] = H(L) * s % MOD * pow(L, MOD - 2, MOD) % MOD

    print(E[N])

solve()
