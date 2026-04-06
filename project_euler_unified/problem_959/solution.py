"""
Problem 959: Sequence Alignment Score

Edit distance (Levenshtein) between first 100 digits of pi and e.

Wagner-Fischer DP algorithm:
  d[i][j] = min cost to align s1[:i] with s2[:j]

Complexity: O(n*m) time and space, n = m = 100.
"""

# First 100 digits of pi and e
PI_DIGITS = "3141592653589793238462643383279502884197169399375105820974944592307816406286208998628034825342117067"
E_DIGITS  = "2718281828459045235360287471352662497757247093699959574966967627724076630353547594571382178525166427"

def edit_distance(s1: str, s2: str) -> tuple[int, list[list[int]]]:
    """Compute Levenshtein distance and return the full DP table."""
    n, m = len(s1), len(s2)
    d = [[0] * (m + 1) for _ in range(n + 1)]

    for i in range(n + 1):
        d[i][0] = i
    for j in range(m + 1):
        d[0][j] = j

    for i in range(1, n + 1):
        for j in range(1, m + 1):
            if s1[i - 1] == s2[j - 1]:
                d[i][j] = d[i - 1][j - 1]
            else:
                d[i][j] = 1 + min(d[i - 1][j], d[i][j - 1], d[i - 1][j - 1])

    return d[n][m], d

def solve() -> int:
    s1 = PI_DIGITS[:100]
    s2 = E_DIGITS[:100]
    dist, _ = edit_distance(s1, s2)
    return dist

# --- Compute answer ---
s1, s2 = PI_DIGITS[:100], E_DIGITS[:100]
answer, dp_table = edit_distance(s1, s2)

# Verify lengths
assert len(s1) == 100 and len(s2) == 100

print(answer)
