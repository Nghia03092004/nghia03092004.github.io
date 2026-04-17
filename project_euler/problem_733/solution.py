"""
Problem 733: Ascending Subsequences

Sum of all elements in all ascending subsequences of length 4.
Uses Fenwick trees for efficient prefix sum queries.
"""

MOD = 10**9 + 7
SEQ_MOD = 10_000_019

class BIT:
    """Fenwick tree for prefix sum queries and point updates."""
    def __init__(self, n):
        self.n = n
        self.tree = [0] * (n + 1)

    def update(self, i, delta):
        while i <= self.n:
            self.tree[i] = (self.tree[i] + delta) % MOD
            i += i & (-i)

    def query(self, i):
        s = 0
        while i > 0:
            s = (s + self.tree[i]) % MOD
            i -= i & (-i)
        return s

def solve(n):
    # Generate sequence
    a = [0] * (n + 1)
    a[1] = 153 % SEQ_MOD
    for i in range(2, n + 1):
        a[i] = (a[i-1] * 153) % SEQ_MOD

    # Coordinate compression
    vals = sorted(set(a[1:n+1]))
    compress = {v: i+1 for i, v in enumerate(vals)}
    sz = len(vals)

    # Fenwick trees for counts and sums at each length k=1..4
    cnt = [BIT(sz) for _ in range(5)]   # cnt[k]
    sml = [BIT(sz) for _ in range(5)]   # sum[k]

    total = 0

    for j in range(1, n + 1):
        v = compress[a[j]]
        for k in range(4, 0, -1):
            if k == 1:
                ck = 1
                sk = a[j] % MOD
            else:
                ck = cnt[k-1].query(v - 1)
                sk = (sml[k-1].query(v - 1) + a[j] * ck) % MOD
            cnt[k].update(v, ck)
            sml[k].update(v, sk)
            if k == 4:
                total = (total + sk) % MOD

    return total

# Verify small case
s6 = solve(6)
print(f"S(6) = {s6}")  # Expected: 94513710

s100 = solve(100)
print(f"S(100) = {s100}")  # Expected: 4465488724217 mod MOD

# Full answer
# s_1m = solve(1000000)
# print(f"S(10^6) mod 10^9+7 = {s_1m}")
