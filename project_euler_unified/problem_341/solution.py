"""
Problem 341: Golomb's Self-Describing Sequence

The Golomb sequence G satisfies: value v appears exactly G(v) times.
Recurrence: G(1) = 1, G(n) = 1 + G(n - G(G(n-1))) for n >= 2.

Define P(v) = sum_{i=1}^{v} G(i) (last position with value <= v).
Then G(n) = v iff P(v-1) < n <= P(v).

We build the sequence until P(v) >= (10^6 - 1)^3, then for each
d = 1..999999 locate the block containing d^3 via a linear scan.

Answer: 56098610614277014
"""

def solve():
    limit = 1000000
    cubic_limit = (limit - 1) ** 3

    # Build Golomb sequence: G[v], P[v] = cumulative position
    G = [0, 1]
    P = [0, 1]
    v = 1
    while P[v] < cubic_limit:
        v += 1
        g = 1 + G[v - G[G[v - 1]]]
        G.append(g)
        P.append(P[v - 1] + g)

    # Answer queries G(d^3) for d = 1..999999 using linear scan
    total = 0
    idx = 1
    for d in range(1, limit):
        n = d * d * d
        while P[idx] < n:
            idx += 1
        total += idx

    print(total)

if __name__ == "__main__":
    solve()
