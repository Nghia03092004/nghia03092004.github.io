"""
Problem 946: Hypergeometric Sums

Compute sum of C(2000, k)^2 for k = 0..1000, mod 10^9 + 7.

By the Vandermonde-Chu identity, sum_{k=0}^{n} C(2n, k)^2 can be expressed
using central binomial coefficients:
    sum_{k=0}^{2n} C(2n,k)^2 = C(4n, 2n)
For the half-sum (k=0..n):
    sum_{k=0}^{n} C(2n,k)^2 = (C(4n,2n) + C(2n,n)^2) / 2

Key result: For n=1000, the answer is computed mod 10^9+7.

Methods:
  1. Modular arithmetic with precomputed factorials
  2. Direct summation for small n (verification)
  3. Vandermonde identity closed-form
  4. Partial sum analysis and symmetry verification
"""
from math import comb as math_comb

MOD = 10 ** 9 + 7

def modinv(a, m=MOD):
    return pow(a, m - 2, m)

def precompute_factorials(n, m=MOD):
    fact = [1] * (n + 1)
    for i in range(1, n + 1):
        fact[i] = fact[i - 1] * i % m
    inv_fact = [1] * (n + 1)
    inv_fact[n] = modinv(fact[n], m)
    for i in range(n - 1, -1, -1):
        inv_fact[i] = inv_fact[i + 1] * (i + 1) % m
    return fact, inv_fact

def comb_mod(n, k, fact, inv_fact, m=MOD):
    if k < 0 or k > n:
        return 0
    return fact[n] * inv_fact[k] % m * inv_fact[n - k] % m

def direct_half_sum(n):
    """Compute sum_{k=0}^{n} C(2n, k)^2 exactly for small n."""
    return sum(math_comb(2 * n, k) ** 2 for k in range(n + 1))

def half_sum_formula(n, fact, inv_fact):
    """sum_{k=0}^{n} C(2n,k)^2 = (C(4n,2n) + C(2n,n)^2) / 2 mod p"""
    c4n_2n = comb_mod(4 * n, 2 * n, fact, inv_fact)
    c2n_n = comb_mod(2 * n, n, fact, inv_fact)
    return (c4n_2n + c2n_n * c2n_n % MOD) * modinv(2) % MOD

# Verification with assertions
# n=1: sum C(2,k)^2 for k=0..1 = C(2,0)^2 + C(2,1)^2 = 1 + 4 = 5
assert direct_half_sum(1) == 5
# Check formula: (C(4,2) + C(2,1)^2)/2 = (6 + 4)/2 = 5
assert (math_comb(4, 2) + math_comb(2, 1) ** 2) // 2 == 5

# n=2: sum C(4,k)^2 for k=0..2 = 1 + 16 + 36 = 53
assert direct_half_sum(2) == 53
assert (math_comb(8, 4) + math_comb(4, 2) ** 2) // 2 == 53

# n=3: verify mod arithmetic matches exact
fact_test, inv_fact_test = precompute_factorials(100)
for nn in [1, 2, 3, 5, 10]:
    exact = direct_half_sum(nn) % MOD
    formula = half_sum_formula(nn, fact_test, inv_fact_test)
    assert exact == formula, f"Mismatch at n={nn}: {exact} vs {formula}"

# Compute answer
fact, inv_fact = precompute_factorials(4001)
answer = half_sum_formula(1000, fact, inv_fact)

# Cross-verify with direct summation (mod)
direct_ans = 0
for k in range(1001):
    c = comb_mod(2000, k, fact, inv_fact)
    direct_ans = (direct_ans + c * c) % MOD
assert direct_ans == answer

print(f"Answer: {answer}")
print(answer)
