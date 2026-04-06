"""
Problem 194: Coloured Configurations

Count proper n-colourings of graph formed by K_a and K_b sharing one edge.
(a, b, n) = (25, 75, 1984), mod 10^8.

P(G, n) = n^{down a} * n^{down b} / n^{down 2} = (n-2)^{down (a-2)} * n^{down b}
"""

def solve():
    MOD = 10**8
    a, b, n = 25, 75, 1984

    result = 1
    # (n-2) falling factorial (a-2) terms
    for i in range(a - 2):
        result = result * (n - 2 - i) % MOD

    # n falling factorial b terms
    for i in range(b):
        result = result * (n - i) % MOD

    return result

answer = solve()
assert answer == 0
print(answer)
