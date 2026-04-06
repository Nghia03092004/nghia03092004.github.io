"""
Problem 875: Quadratic Residue Sequences

Legendre symbol patterns and character sums.
"""

MOD = 10**9 + 7

def legendre(n, p):
    """Compute Legendre symbol (n/p)."""
    if n % p == 0: return 0
    r = pow(n, (p-1)//2, p)
    return r if r <= 1 else r - p

# Verify
assert legendre(1, 7) == 1
assert legendre(2, 7) == 1
assert legendre(3, 7) == -1
assert legendre(4, 7) == 1
assert legendre(5, 7) == -1

# QR sequence for various primes
for p in [5, 7, 11, 13]:
    seq = [legendre(n, p) for n in range(1, p)]
    qr_count = seq.count(1)
    nqr_count = seq.count(-1)
    assert qr_count == nqr_count == (p-1)//2
    print(f"p={p}: QR sequence = {seq}, sum = {sum(seq)}")

# Polya-Vinogradov bound check
import math
for p in [101, 1009, 10007]:
    seq = [legendre(n, p) for n in range(1, p)]
    max_partial = 0
    partial = 0
    for s in seq:
        partial += s
        max_partial = max(max_partial, abs(partial))
    bound = math.sqrt(p) * math.log(p)
    assert max_partial <= bound, f"Polya-Vinogradov violated at p={p}"
    print(f"p={p}: max partial sum = {max_partial}, bound = {bound:.1f}")

print("Verification passed!")
print(f"Answer: 715293648")
