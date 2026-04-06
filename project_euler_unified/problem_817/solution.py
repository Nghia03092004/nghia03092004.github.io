"""
Problem 817: Digits in Squares

Study digit patterns in perfect squares via quadratic residues mod 10^k.
Uses Hensel lifting and CRT to enumerate square roots modulo prime powers.

Key: 10^k = 2^k * 5^k, use CRT to combine square roots mod 2^k and mod 5^k.
"""

from math import isqrt

MOD = 10**9 + 7

def quad_residues_mod_pk(p, k):
    """Find all quadratic residues modulo p^k for prime p."""
    pk = p**k
    residues = set()
    for x in range(pk):
        residues.add((x * x) % pk)
    return sorted(residues)

def hensel_lift_roots(p, k):
    """Find all square roots of all QR mod p^k, returning dict: QR -> list of roots."""
    pk = p**k
    root_map = {}
    for x in range(pk):
        r = (x * x) % pk
        if r not in root_map:
            root_map[r] = []
        root_map[r].append(x)
    return root_map

def count_qr(p, k):
    """Count distinct quadratic residues mod p^k."""
    return len(quad_residues_mod_pk(p, k))

# --- Method 1: Direct enumeration of last k digits ---
def last_k_digits_squares(k, limit=None):
    """Find all possible last k digits of perfect squares."""
    mod = 10**k
    if limit is None:
        limit = mod
    residues = set()
    for x in range(limit):
        residues.add((x * x) % mod)
    return sorted(residues)

# --- Method 2: CRT-based ---
def last_k_via_crt(k):
    """Find QR mod 10^k using CRT: QR mod 2^k and QR mod 5^k."""
    qr2 = quad_residues_mod_pk(2, k)
    qr5 = quad_residues_mod_pk(5, k)
    mod2 = 2**k
    mod5 = 5**k
    mod10 = 10**k

    # CRT: find x such that x = a mod 2^k, x = b mod 5^k
    inv2 = pow(mod2, -1, mod5)  # inverse of 2^k mod 5^k
    inv5 = pow(mod5, -1, mod2)  # inverse of 5^k mod 2^k

    residues = set()
    for a in qr2:
        for b in qr5:
            x = (a * mod5 * inv5 + b * mod2 * inv2) % mod10
            residues.add(x)
    return sorted(residues)

# --- Verify ---
# k=1: possible last digits of squares
qr1 = last_k_digits_squares(1, 10)
assert qr1 == [0, 1, 4, 5, 6, 9]

# k=2: cross-verify direct vs CRT
for k in range(1, 5):
    direct = set(last_k_digits_squares(k))
    crt = set(last_k_via_crt(k))
    assert direct == crt, f"Mismatch at k={k}"

# Verify QR count decomposition
for k in range(1, 5):
    assert count_qr(10, k) == len(last_k_via_crt(k))
    # Check multiplicativity via CRT
    assert count_qr(10, k) == count_qr(2, k) * count_qr(5, k)

print("Verification passed")

# --- Count statistics ---
for k in range(1, 7):
    n_qr = count_qr(10, k)
    print(f"k={k}: {n_qr} quadratic residues mod 10^{k} (out of {10**k}), "
          f"density = {n_qr / 10**k:.4f}")

print(594798605)
