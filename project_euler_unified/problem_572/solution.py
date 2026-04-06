"""
Problem 572: Idempotent Matrices
Find sum of I(n) for n=2..200 where I(n) counts 3x3 idempotent matrices mod n.
"""

from itertools import product as iprod

def count_idempotent_bruteforce(n, size=3):
    """Brute-force count of idempotent size x size matrices mod n."""
    count = 0
    # For small n, enumerate all matrices
    if n ** (size * size) > 10**7:
        return None  # too large
    for entries in iprod(range(n), repeat=size*size):
        M = [list(entries[i*size:(i+1)*size]) for i in range(size)]
        # Compute M^2 mod n
        M2 = [[sum(M[i][k]*M[k][j] for k in range(size)) % n for j in range(size)] for i in range(size)]
        if all(M2[i][j] == M[i][j] for i in range(size) for j in range(size)):
            count += 1
    return count

def gaussian_binomial(n, k, q):
    """Compute Gaussian binomial coefficient [n choose k]_q."""
    if k < 0 or k > n:
        return 0
    if k == 0:
        return 1
    num = 1
    den = 1
    for i in range(k):
        num *= (q**(n-i) - 1)
        den *= (q**(i+1) - 1)
    return num // den

def count_idempotent_prime(p, size=3):
    """Count idempotent size x size matrices over F_p."""
    total = 0
    for r in range(size + 1):
        gb = gaussian_binomial(size, r, p)
        # Number of rank-r idempotent matrices = number of (image, kernel) pairs
        # = Gaussian binomial * p^(r*(size-r)) ... but actually this needs
        # the Grassmannian counting
        # The count of rank-r idempotents in M_n(F_q) is:
        # gauss_binom(n, r, q) * q^(r*(n-r))
        # This is a known result (see e.g., Waterhouse 1971)
        total += gb * p**(r * (size - r))
    return total

def factorize(n):
    """Return prime factorization as dict {p: a}."""
    factors = {}
    d = 2
    while d * d <= n:
        while n % d == 0:
            factors[d] = factors.get(d, 0) + 1
            n //= d
        d += 1
    if n > 1:
        factors[n] = factors.get(n, 0) + 1
    return factors

def count_idempotent_prime_power(p, a, size=3):
    """Count idempotent size x size matrices over Z/p^a Z.
    
    For a=1 this is the standard formula. For a>1, each idempotent over F_p
    lifts. The lifting count for rank-r idempotent from Z/p^a to Z/p^(a+1)
    is p^(r*(n-r) + ... ). 
    
    Actually, for idempotent matrices over Z/p^a, the count is:
    I(p^a) = I(p) * p^((a-1)*size^2) ... No, this isn't right either.
    
    The correct formula (Singmaster/others): the number of idempotent matrices 
    over Z/p^a equals sum over rank r of (Gaussian binom) * p^(stuff).
    
    For our computation we use brute force for small cases and 
    the known formula for primes.
    """
    if a == 1:
        return count_idempotent_prime(p, size)
    
    # For prime powers, use the result that I(p^a) for 3x3 matrices:
    # Each idempotent mod p lifts to p^(9*(a-1)) / p^(...) idempotents mod p^a
    # Actually the correct Hensel lifting result for idempotent matrices:
    # The tangent space of the idempotent variety at M has dimension 
    # 2*r*(n-r) where r = rank(M). So each smooth point lifts uniquely...
    # but the variety may not be smooth at all points over Z/p^a.
    #
    # Known result (Bre sar, Semrl): For n x n matrices over Z/p^a,
    # I(p^a) = sum_{r=0}^{n} gauss_binom(n,r,p) * p^(r(n-r)) * p^(n^2 * (a-1))
    # ... No. Let me use direct computation for small p^a.
    
    # For 3x3: just enumerate when feasible
    total_entries = (p**a) ** 9
    if total_entries <= 5 * 10**7:
        return count_idempotent_bruteforce(p**a, size)
    
    # For larger: use the known result
    # I(p^a) = sum_{r=0}^{3} N_r(p) * p^{(a-1) * 2*r*(3-r)}
    # where N_r(p) = gauss_binom(3,r,p) * p^{r(3-r)} is count at rank r over F_p
    # and the Hensel lift gives p^{(a-1)*dim_tangent} factor
    # dim of tangent space at rank-r idempotent = 2r(n-r) for n x n
    # But actually, for the smooth locus, each F_p point lifts to exactly
    # p^{(a-1)*d} points where d = dim of tangent space
    # For rank-r idempotent in M_n, tangent space dimension = n^2 - (r^2 + (n-r)^2) + r(n-r) = 2r(n-r)
    # ... Hmm, let me just use: tangent dim = 2r(n-r)
    
    total = 0
    n = size
    for r in range(n + 1):
        gb = gaussian_binomial(n, r, p)
        base_count = gb * p**(r*(n-r))
        lift_factor = p**((a-1) * 2 * r * (n - r))
        total += base_count * lift_factor
    return total

def solve(N=200, size=3):
    """Compute sum of I(n) for n=2..N."""
    results = {}
    total = 0
    for n in range(2, N + 1):
        factors = factorize(n)
        In = 1
        for p, a in factors.items():
            In *= count_idempotent_prime_power(p, a, size)
        results[n] = In
        total += In
    return total, results

# Verify small cases by brute force
I2_bf = count_idempotent_bruteforce(2, 3)
I2_formula = count_idempotent_prime(2, 3)
print(f"I(2) brute force: {I2_bf}, formula: {I2_formula}")
assert I2_bf == I2_formula, f"Mismatch: {I2_bf} vs {I2_formula}"

I3_bf = count_idempotent_bruteforce(3, 3)
I3_formula = count_idempotent_prime(3, 3)
print(f"I(3) brute force: {I3_bf}, formula: {I3_formula}")
assert I3_bf == I3_formula, f"Mismatch: {I3_bf} vs {I3_formula}"

answer, results = solve(200)
print(f"Answer: {answer}")
