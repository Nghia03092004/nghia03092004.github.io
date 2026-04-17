"""
Problem 186: Connectedness of a Network

Using a Lagged Fibonacci Generator for phone calls, find how many successful
calls are needed for 99% of subscribers to be connected to subscriber 524287.
"""

class DSU:
    def __init__(self, n):
        self.parent = list(range(n))
        self.rank = [0] * n
        self.size = [1] * n

    def find(self, x):
        while self.parent[x] != x:
            self.parent[x] = self.parent[self.parent[x]]
            x = self.parent[x]
        return x

    def union(self, a, b):
        a, b = self.find(a), self.find(b)
        if a == b:
            return
        if self.rank[a] < self.rank[b]:
            a, b = b, a
        self.parent[b] = a
        self.size[a] += self.size[b]
        if self.rank[a] == self.rank[b]:
            self.rank[a] += 1

def solve():
    N = 1_000_000
    PM = 524287
    TARGET = 990_000

    # Initialize LFG
    S = [0] * 56
    for k in range(1, 56):
        S[k] = (100003 - 200003 * k + 300007 * k * k * k) % N

    # Circular buffer
    cb = [S[k] for k in range(1, 56)]
    p = 0

    def next_s():
        nonlocal p
        val = cb[p]
        cb[p] = (cb[p] + cb[(p + 31) % 55]) % N
        p = (p + 1) % 55
        return val

    dsu = DSU(N)
    calls = 0

    while dsu.size[dsu.find(PM)] < TARGET:
        caller = next_s()
        called = next_s()
        if caller != called:
            calls += 1
            dsu.union(caller, called)

    print(calls)

if __name__ == "__main__":
    solve()
