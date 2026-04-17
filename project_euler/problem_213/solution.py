import numpy as np

def solve():
    R, C = 30, 30
    N = R * C
    ROUNDS = 50

    def idx(r, c):
        return r * C + c

    dr = [-1, 1, 0, 0]
    dc = [0, 0, -1, 1]

    deg = np.zeros(N, dtype=float)
    for r in range(R):
        for c in range(C):
            deg[idx(r, c)] = sum(
                1 for k in range(4) if 0 <= r + dr[k] < R and 0 <= c + dc[k] < C
            )

    T = np.zeros((N, N), dtype=float)
    for r in range(R):
        for c in range(C):
            i = idx(r, c)
            for k in range(4):
                nr, nc = r + dr[k], c + dc[k]
                if 0 <= nr < R and 0 <= nc < C:
                    T[i, idx(nr, nc)] = 1.0 / deg[i]

    Tn = np.linalg.matrix_power(T, ROUNDS)
    log_prod_not = np.sum(np.log1p(-Tn), axis=0)
    answer = np.sum(np.exp(log_prod_not))

    print(f"{answer:.6f}")

if __name__ == "__main__":
    solve()
