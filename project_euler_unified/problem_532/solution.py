"""
Problem 532: Nanoscale Strips

Compute f(10^7, 10^7) mod 999999937, where f(W,H) = C(W+H, W)^2 mod p.

Method: precompute factorials and inverse factorials mod p, then
evaluate the binomial coefficient in O(1).
"""

MOD = 999999937

def solve(W, H, mod):
    n = W + H
    # Precompute factorials
    fact = [1] * (n + 1)
    for i in range(1, n + 1):
        fact[i] = fact[i - 1] * i % mod
    # Inverse factorials via Fermat's little theorem
    inv_fact = [1] * (n + 1)
    inv_fact[n] = pow(fact[n], mod - 2, mod)
    for i in range(n - 1, -1, -1):
        inv_fact[i] = inv_fact[i + 1] * (i + 1) % mod
    # C(n, W) mod p
    c = fact[n] * inv_fact[W] % mod * inv_fact[H] % mod
    return c * c % mod

print(solve(10**7, 10**7, MOD))
