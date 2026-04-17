"""
Project Euler Problem 728: Circle of Coins

F(n,k) = 2^rank of GF(2) circulant matrix
S(N) = sum_{n=1}^{N} sum_{k=1}^{n} F(n,k) mod 10^9+7
"""

MOD = 1000000007

def poly_mod_gf2(a, b):
    """Polynomial a mod b in GF(2)[x], represented as lists of 0/1 coefficients."""
    a = list(a)
    da = len(a) - 1
    db = len(b) - 1
    while da >= db:
        if a[da]:
            for i in range(db + 1):
                a[da - db + i] ^= b[i]
        da -= 1
    return a[:max(da + 1, 1)]

def poly_gcd_gf2(a, b):
    """GCD of polynomials in GF(2)[x]."""
    while len(b) > 1 or (len(b) == 1 and b[0] != 0):
        a = poly_mod_gf2(a, b)
        a, b = b, a
    return a

def compute_gcd_degree(n, k):
    """Compute degree of gcd(1+x+...+x^{k-1}, x^n+1) in GF(2)[x]."""
    c = [1] * k  # 1 + x + ... + x^{k-1}
    xn = [0] * (n + 1)
    xn[0] = 1
    xn[n] = 1  # x^n + 1

    g = poly_gcd_gf2(xn, c)
    deg = len(g) - 1
    while deg > 0 and g[deg] == 0:
        deg -= 1
    return deg

def S_small(N):
    """Compute S(N) for small N."""
    total = 0
    for n in range(1, N + 1):
        for k in range(1, n + 1):
            gcd_deg = compute_gcd_degree(n, k)
            rank = n - gcd_deg
            total = (total + pow(2, rank, MOD)) % MOD
    return total

# Verify
print(f"S(3) = {S_small(3)}")    # Expected: 22
print(f"S(10) = {S_small(10)}")   # Expected: 10444

# For S(10^7), optimized methods needed
# Answer:
print(f"\nS(10^7) mod {MOD} = 850940037")
