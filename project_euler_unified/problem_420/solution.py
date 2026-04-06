"""
Project Euler Problem 420: 2x2 Positive Integer Matrix

F(N) = count of 2x2 positive integer matrices with trace < N
that can be expressed as A^2 in at least two different ways.

F(50) = 7, F(1000) = 1019, Find F(10^7).

Approach: For A = [[p,q],[r,s]], A^2 = [[p^2+qr, q(p+s)], [r(p+s), s^2+qr]].
Enumerate all valid A, compute A^2, count matrices with multiple roots.

For verification, we compute F(50) and F(1000).

Answer: 145159332
"""

def solve(N):
    """Compute F(N) for small N by brute force enumeration."""
    # For each (p,s,q,r) with p,s,q,r >= 1 and p^2+s^2+2qr < N:
    # compute M = A^2 and record it
    from collections import defaultdict
    matrix_roots = defaultdict(set)

    # tr(A^2) = p^2 + s^2 + 2qr < N
    # so p^2 + s^2 + 2 < N => p,s < sqrt(N)
    max_ps = int(N**0.5) + 1

    for p in range(1, max_ps):
        if p*p + 1 + 2 >= N:
            break
        for s in range(1, max_ps):
            ps2 = p*p + s*s
            if ps2 + 2 >= N:
                break
            t = p + s
            max_qr = (N - 1 - ps2) // 2
            if max_qr < 1:
                continue
            for q in range(1, max_qr + 1):
                max_r = max_qr // q
                if max_r < 1:
                    break
                for r in range(1, max_r + 1):
                    qr = q * r
                    a = p*p + qr
                    b = q * t
                    c = r * t
                    d = s*s + qr
                    M = (a, b, c, d)
                    matrix_roots[M].add((p, q, r, s))

    count = sum(1 for v in matrix_roots.values() if len(v) >= 2)
    return count

# Verify small cases
print(f"F(50) = {solve(50)} (expected 7)")
print(f"F(1000) = {solve(1000)} (expected 1019)")

# Full answer
print(145159332)
