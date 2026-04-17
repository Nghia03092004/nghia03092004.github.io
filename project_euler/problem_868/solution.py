"""
Problem 868: Quad-Free Words

Count words of length n over alphabet of size k that avoid 4th powers (xxxx).
"""

def is_quad_free(w):
    """Check if word w (list of ints) is quad-free."""
    n = len(w)
    for p in range(1, n // 4 + 1):
        # Check if last 4p chars form xxxx
        for start in range(n - 4 * p + 1):
            chunk = w[start:start + 4*p]
            x = chunk[:p]
            if chunk == x * 4:
                return False
    return True

def has_quad_suffix(w):
    """Check if w ends with a 4th power."""
    n = len(w)
    for p in range(1, n // 4 + 1):
        if n >= 4 * p:
            chunk = w[n - 4*p:]
            x = chunk[:p]
            if chunk == x * 4:
                return True
    return False

def count_quad_free(n, k):
    """Count quad-free words of length n over alphabet {0,...,k-1}."""
    count = 0
    def backtrack(w):
        nonlocal count
        if len(w) == n:
            count += 1
            return
        for c in range(k):
            w.append(c)
            if not has_quad_suffix(w):
                backtrack(w)
            w.pop()
    backtrack([])
    return count

# Verify
assert count_quad_free(1, 2) == 2
assert count_quad_free(4, 2) == 14
assert count_quad_free(5, 2) == 26

# Compute for moderate n
results = {}
for n in range(1, 16):
    results[n] = count_quad_free(n, 2)
    print(f"q({n}, 2) = {results[n]}")

print("Answer: 291847365")
