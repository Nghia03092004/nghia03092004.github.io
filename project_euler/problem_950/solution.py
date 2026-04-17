"""
Problem 950: Chromatic Polynomial of Grid Graphs

Compute P(G_{3,10}, 4) mod 10^9 + 7.

The chromatic polynomial P(G, k) counts the number of proper k-colorings
of graph G. For a 3-by-n grid graph, we use the transfer matrix method:
  1. Enumerate all valid column colorings (proper colorings of a path P_3)
  2. Build a transfer matrix T where T[i][j] = 1 iff column coloring i
     is compatible with column coloring j (no same color in adjacent cells)
  3. P(G_{3,n}, k) = sum of all entries of T^{n-1}

Key results:
  - Valid 3-row colorings with k colors: k*(k-1)^2 columns
  - Transfer matrix captures column-to-column compatibility
  - Matrix exponentiation gives the answer in O(C^3 * log n)

Methods:
  1. Column coloring enumeration
  2. Transfer matrix construction
  3. Matrix exponentiation mod p
  4. Verification with small grids via brute-force
"""
from itertools import product

MOD = 10 ** 9 + 7

def valid_column_colorings(rows, k):
    """Generate all proper colorings of a path graph on 'rows' vertices
    using k colors. Adjacent vertices must have different colors."""
    colors = list(range(k))
    result = []
    for col in product(colors, repeat=rows):
        if all(col[i] != col[i + 1] for i in range(rows - 1)):
            result.append(col)
    return result

def build_transfer_matrix(valid_cols, rows):
    """Build compatibility matrix: T[i][j]=1 iff col i and col j
    have different colors in every row."""
    C = len(valid_cols)
    T = [[0] * C for _ in range(C)]
    for i, c1 in enumerate(valid_cols):
        for j, c2 in enumerate(valid_cols):
            if all(c1[r] != c2[r] for r in range(rows)):
                T[i][j] = 1
    return T

def mat_mul(A, B, mod):
    n = len(A)
    R = [[0] * n for _ in range(n)]
    for i in range(n):
        for kk in range(n):
            if A[i][kk] == 0:
                continue
            for jj in range(n):
                R[i][jj] = (R[i][jj] + A[i][kk] * B[kk][jj]) % mod
    return R

def mat_pow(M, p, mod):
    n = len(M)
    R = [[1 if i == j else 0 for j in range(n)] for i in range(n)]
    while p > 0:
        if p & 1:
            R = mat_mul(R, M, mod)
        M = mat_mul(M, M, mod)
        p >>= 1
    return R

def solve(rows, cols, k, mod=MOD):
    """Compute P(G_{rows x cols}, k) mod mod."""
    valid_cols = valid_column_colorings(rows, k)
    T = build_transfer_matrix(valid_cols, rows)
    Tn = mat_pow(T, cols - 1, mod)
    C = len(valid_cols)
    ans = 0
    for i in range(C):
        for j in range(C):
            ans = (ans + Tn[i][j]) % mod
    return ans

def chromatic_brute(rows, cols, k):
    """Brute-force count proper colorings of a rows x cols grid."""
    from itertools import product as it_product
    count = 0
    for coloring in it_product(range(k), repeat=rows * cols):
        grid = [coloring[r * cols:(r + 1) * cols] for r in range(rows)]
        valid = True
        for r in range(rows):
            for c in range(cols):
                if c + 1 < cols and grid[r][c] == grid[r][c + 1]:
                    valid = False
                    break
                if r + 1 < rows and grid[r][c] == grid[r + 1][c]:
                    valid = False
                    break
            if not valid:
                break
        if valid:
            count += 1
    return count

# Verification with assertions
# 3x1 grid: just P_3 with k colors = k*(k-1)^2
assert solve(3, 1, 3) == 3 * 2 * 2  # 12
assert solve(3, 1, 4) == 4 * 3 * 3  # 36

# 3x2 grid: verify against brute force
for k_test in [3, 4]:
    brute = chromatic_brute(3, 2, k_test)
    transfer = solve(3, 2, k_test, mod=10**18)  # large mod to get exact
    assert brute == transfer, f"Mismatch 3x2 k={k_test}: {brute} vs {transfer}"

# 2x3 grid: verify
for k_test in [3, 4]:
    brute = chromatic_brute(2, 3, k_test)
    transfer = solve(2, 3, k_test, mod=10**18)
    assert brute == transfer, f"Mismatch 2x3 k={k_test}: {brute} vs {transfer}"

# Compute answer
answer = solve(3, 10, 4)
print(f"P(G_{{3,10}}, 4) mod 10^9+7 = {answer}")
print(answer)
