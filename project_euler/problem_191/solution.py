"""
Problem 191: Prize Strings

Count 30-character strings over {O, A, L} with at most one L
and no three consecutive A's.
"""

def solve(n=30):
    # g[i] = number of valid strings of length i with no L and no "AAA"
    g = [0] * (n + 1)
    g[0] = 1
    g[1] = 2
    g[2] = 4
    for i in range(3, n + 1):
        g[i] = g[i-1] + g[i-2] + g[i-3]

    # No L: g[n]
    ans = g[n]

    # Exactly one L at position k: g[k] * g[n-1-k]
    for k in range(n):
        ans += g[k] * g[n - 1 - k]

    return ans

answer = solve()
assert answer == 1918080160
print(answer)
