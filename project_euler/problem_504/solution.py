from math import gcd, isqrt

def solve():
    M = 100

    # Precompute GCD table
    g = [[0] * (M + 1) for _ in range(M + 1)]
    for i in range(M + 1):
        for j in range(M + 1):
            g[i][j] = gcd(i, j)

    # Precompute perfect squares
    max_i = 2 * M * M + 1
    is_sq = set()
    k = 0
    while k * k <= max_i:
        is_sq.add(k * k)
        k += 1

    count = 0
    for a in range(1, M + 1):
        for b in range(1, M + 1):
            for c in range(1, M + 1):
                gab = g[a][b]
                gbc = g[b][c]
                sum_ac = a + c
                sum_bd_base = b
                for d in range(1, M + 1):
                    boundary = gab + gbc + g[c][d] + g[d][a]
                    twice_area = sum_ac * (b + d)
                    twice_i = twice_area - boundary + 2
                    if twice_i % 2 != 0:
                        continue
                    interior = twice_i // 2
                    if interior in is_sq:
                        count += 1

    print(count)

solve()
