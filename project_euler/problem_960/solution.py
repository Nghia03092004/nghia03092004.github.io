"""
Problem 960: Modular Combinatorics

Compute sum_{k=0}^{n} C(n,k) * k^3 mod (10^9 + 7), n = 10^6.

Using Stirling numbers: k^3 = k + 3k(k-1) + k(k-1)(k-2)
sum C(n,k)*k^{r_down} = n^{r_down} * 2^{n-r}

Result: n*2^{n-1} + 3*n*(n-1)*2^{n-2} + n*(n-1)*(n-2)*2^{n-3}  mod p

Complexity: O(log n) for modular exponentiation.
"""

MOD = 10**9 + 7

def solve(n: int = 10**6) -> int:
    """Compute sum of C(n,k)*k^3 for k=0..n, mod 10^9+7."""
    p = MOD
    pow2_n1 = pow(2, n - 1, p)
    pow2_n2 = pow(2, n - 2, p)
    pow2_n3 = pow(2, n - 3, p)

    term1 = n % p * pow2_n1 % p
    term2 = 3 * (n % p) % p * ((n - 1) % p) % p * pow2_n2 % p
    term3 = (n % p) * ((n - 1) % p) % p * ((n - 2) % p) % p * pow2_n3 % p

    return (term1 + term2 + term3) % p

# --- Compute answer ---
n = 10**6
answer = solve(n)

# Verify with small cases by brute force
from math import comb
for test_n in [1, 2, 3, 5, 10]:
    brute = sum(comb(test_n, k) * k**3 for k in range(test_n + 1))
    formula = solve(test_n)
    assert brute % MOD == formula, f"n={test_n}: {brute % MOD} != {formula}"

print(answer)
