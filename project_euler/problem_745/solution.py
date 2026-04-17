"""
Project Euler Problem 745: Sum of Squares II

For a positive integer n, g(n) = max perfect square dividing n.
S(N) = sum of g(n) for n=1..N.
Find S(10^14) mod 10^9+7.

Uses Jordan totient function J_2 sieve approach.
"""

def solve():
    MOD = 1000000007
    N = 10**14
    L = int(N**0.5)  # 10^7

    # Sieve J_2 values
    # J_2(n) = n^2 * prod_{p|n} (1 - 1/p^2)
    # We work modulo MOD

    j2 = [0] * (L + 1)
    for i in range(1, L + 1):
        j2[i] = (i * i) % MOD

    # Sieve of Eratosthenes style
    is_prime = bytearray(b'\x01') * (L + 1)
    is_prime[0] = is_prime[1] = 0

    for p in range(2, L + 1):
        if is_prime[p]:
            p2 = (p * p) % MOD
            inv_p2 = pow(p2, MOD - 2, MOD)
            factor = (p2 - 1) * inv_p2 % MOD
            for m in range(p, L + 1, p):
                if m != p:
                    is_prime[m] = 0
                j2[m] = j2[m] * factor % MOD

    # Compute S(N)
    ans = 0
    for e in range(1, L + 1):
        q = (N // (e * e)) % MOD
        ans = (ans + j2[e] * q) % MOD

    print(ans)

if __name__ == "__main__":
    solve()
