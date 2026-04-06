"""
Problem 316: Numbers in Decimal Expansions

g(n) for integer n with decimal string s of length d:
    g(n) = W(s) - d + 1
where W(s) = sum of 10^k for each k in 1..d where the length-k prefix of s
equals the length-k suffix of s (Conway Leading Number).

Compute: sum of g(floor(10^16 / n)) for n = 2 to 999999.
"""

def g(n):
    """Compute g(n) - expected starting position of first occurrence in random decimal expansion."""
    s = str(n)
    d = len(s)
    W = 0
    for k in range(1, d + 1):
        if s[:k] == s[d-k:]:
            W += 10**k
    return W - d + 1

def solve():
    P = 10**16
    total = 0
    for n in range(2, 1000000):
        m = P // n
        total += g(m)
    print(total)

if __name__ == "__main__":
    solve()
