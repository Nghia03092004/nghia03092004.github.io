"""
Problem 487: Sums of Power Sums
f_k(n) = sum_{i=1}^{n} i^k
Compute sums of f_k evaluated at various points modulo primes,
using Faulhaber's formulas and Bernoulli numbers.
"""

def bernoulli_mod_p(k: int, p: int) -> list:
    """Compute Bernoulli numbers B_0, B_1, ..., B_k modulo prime p."""
    B = [0] * (k + 1)
    B[0] = 1
    for m in range(1, k + 1):
        # B_m = -1/(m+1) * sum_{j=0}^{m-1} C(m+1,j) * B_j
        s = 0
        c = 1  # binomial coefficient C(m+1, j)
        for j in range(m):
            s = (s + c * B[j]) % p
            c = c * (m + 1 - j) % p * pow(j + 1, p - 2, p) % p
        B[m] = (-(s) * pow(m + 1, p - 2, p)) % p
    return B

def power_sum_mod(k: int, n: int, p: int):
    """Compute f_k(n) = sum_{i=1}^{n} i^k mod p using Faulhaber's formula."""
    B = bernoulli_mod_p(k, p)
    inv_k1 = pow(k + 1, p - 2, p)
    result = 0
    c = 1  # C(k+1, j)
    n_pow = pow(n, k + 1, p)  # n^(k+1-j), starting with j=0
    for j in range(k + 1):
        result = (result + c * B[j] % p * n_pow) % p
        # Update for next j
        c = c * (k + 1 - j) % p * pow(j + 1, p - 2, p) % p
        n_pow = n_pow * pow(n, p - 2, p) % p  # divide by n
    return result * inv_k1 % p

def sum_of_power_sums_mod(k: int, p: int):
    """Compute S_k(p) = sum_{i=1}^{p-1} f_k(i) mod p."""
    total = 0
    for i in range(1, p):
        total = (total + power_sum_mod(k, i, p)) % p
    return total

def sum_of_power_sums_fast(k: int, p: int):
    """
    Compute S_k(p) = sum_{i=1}^{p-1} f_k(i) mod p using Faulhaber + Fermat.
    sum_{i=1}^{p-1} i^m = -1 mod p if (p-1)|m and m>0, else 0 mod p (for m>0).
    """
    B = bernoulli_mod_p(k, p)
    inv_k1 = pow(k + 1, p - 2, p)

    result = 0
    c = 1  # C(k+1, j)
    for j in range(k + 1):
        m = k + 1 - j
        # sum_{i=1}^{p-1} i^m mod p
        if m == 0:
            inner = (p - 1) % p
        elif m % (p - 1) == 0:
            inner = p - 1  # = -1 mod p
        else:
            inner = 0
        result = (result + c * B[j] % p * inner) % p
        c = c * (k + 1 - j) % p * pow(j + 1, p - 2, p) % p

    return result * inv_k1 % p

# Verify: compare brute-force vs fast for small cases
p = 17
for k in range(1, 10):
    bf = sum_of_power_sums_mod(k, p)
    fast = sum_of_power_sums_fast(k, p)
    assert bf == fast, f"Mismatch at k={k}, p={p}: {bf} vs {fast}"

print(f"Verification passed for p={p}, k=1..9")

# Show some values
for p in [5, 7, 11, 13]:
    for k in [1, 2, 3, 4]:
        val = sum_of_power_sums_fast(k, p)
        print(f"S_{k}({p}) = {val}")
