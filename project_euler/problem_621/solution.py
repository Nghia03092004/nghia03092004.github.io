"""
Problem 621: Expressing n as Sum of Triangular Numbers
Count ordered triples (a,b,c) with t_a + t_b + t_c = n.
"""

def is_triangular(m):
    """Check if m is a triangular number."""
    if m < 0:
        return False
    d = 8 * m + 1
    s = int(d**0.5)
    for c in [s - 1, s, s + 1]:
        if c >= 0 and c * c == d:
            return True
    return False

def triangular(k):
    return k * (k + 1) // 2

def count_representations(n):
    """Count ordered triples (a,b,c) with t_a + t_b + t_c = n."""
    count = 0
    a = 0
    while triangular(a) <= n:
        ta = triangular(a)
        b = 0
        while triangular(b) <= n - ta:
            tb = triangular(b)
            if is_triangular(n - ta - tb):
                count += 1
            b += 1
        a += 1
    return count

# Compute for small range
N = 80
counts = [count_representations(k) for k in range(N + 1)]
total = sum(counts)
print(f"Sum of T(k) for k=0..{N}: {total}")
for k in range(min(20, N + 1)):
    print(f"  T({k}) = {counts[k]}")
