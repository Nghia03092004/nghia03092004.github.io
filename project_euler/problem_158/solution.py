from math import comb

def p(n):
    """Count n-length strings from n distinct letters (out of 26) with exactly one ascent."""
    if n <= 1:
        return 0
    eulerian_1 = 2**n - n - 1
    return comb(26, n) * eulerian_1

# Find the maximum p(n) for 1 <= n <= 26
values = [(n, p(n)) for n in range(1, 27)]
best_n, best_val = max(values, key=lambda x: x[1])
