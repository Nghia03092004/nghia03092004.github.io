"""
Problem 324: Building a Tower
3x3xn tower with 2x1x1 bricks.
Find f(10^10000) mod (10^8 + 7).

Uses transfer matrix + Berlekamp-Massey + polynomial exponentiation.
"""

MOD = 10**8 + 7

def build_transfer_matrix():
    """Build 512x512 transfer matrix for 3x3 layer profiles."""
    T = [[0]*512 for _ in range(512)]

    def fill(pos, cur_mask, next_mask, start_mask):
        while pos < 9 and (cur_mask >> pos) & 1:
            pos += 1
        if pos == 9:
            T[start_mask][next_mask] += 1
            return
        r, c = pos // 3, pos % 3
        # Horizontal brick (right)
        if c + 1 < 3 and not ((cur_mask >> (pos + 1)) & 1):
            fill(pos + 1, cur_mask | (1 << pos) | (1 << (pos + 1)), next_mask, start_mask)
        # Vertical brick (down, within layer)
        if r + 1 < 3 and not ((cur_mask >> (pos + 3)) & 1):
            fill(pos + 1, cur_mask | (1 << pos) | (1 << (pos + 3)), next_mask, start_mask)
        # Brick going up to next layer
        fill(pos + 1, cur_mask | (1 << pos), next_mask | (1 << pos), start_mask)

    for s in range(512):
        fill(0, s, 0, s)
    return T

def berlekamp_massey(s, mod):
    """Find shortest LFSR for sequence s mod mod."""
    n = len(s)
    C, B = [1], [1]
    L, m, b = 0, 1, 1

    for i in range(n):
        d = s[i]
        for j in range(1, L + 1):
            d = (d + C[j] * s[i - j]) % mod
        if d == 0:
            m += 1
        elif 2 * L <= i:
            T_save = C[:]
            coef = d * pow(b, -1, mod) % mod
            while len(C) < len(B) + m:
                C.append(0)
            for j in range(len(B)):
                C[j + m] = (C[j + m] - coef * B[j]) % mod
            L = i + 1 - L
            B = T_save
            b = d
            m = 1
        else:
            coef = d * pow(b, -1, mod) % mod
            while len(C) < len(B) + m:
                C.append(0)
            for j in range(len(B)):
                C[j + m] = (C[j + m] - coef * B[j]) % mod
            m += 1
    return L, C

def poly_mul(a, b, modpoly, mod):
    """Multiply polynomials a*b mod modpoly mod mod."""
    c = [0] * (len(a) + len(b) - 1)
    for i in range(len(a)):
        if a[i] == 0:
            continue
        for j in range(len(b)):
            c[i + j] = (c[i + j] + a[i] * b[j]) % mod

    d = len(modpoly) - 1
    lc_inv = pow(modpoly[d], -1, mod)
    for i in range(len(c) - 1, d - 1, -1):
        if c[i] == 0:
            continue
        coef = c[i] * lc_inv % mod
        for j in range(d + 1):
            c[i - d + j] = (c[i - d + j] - coef * modpoly[j]) % mod

    result = c[:d]
    while len(result) < d:
        result.append(0)
    return result

def poly_pow(base, exp, modpoly, mod):
    """Compute base^exp mod modpoly mod mod."""
    d = len(modpoly) - 1
    result = [0] * d
    result[0] = 1

    if exp == 0:
        return result

    bits = []
    n = exp
    while n > 0:
        bits.append(n & 1)
        n >>= 1
    bits.reverse()

    for bit in bits:
        result = poly_mul(result, result, modpoly, mod)
        if bit:
            result = poly_mul(result, base, modpoly, mod)
    return result

def solve():
    T = build_transfer_matrix()

    # Compute sequence values via matrix-vector multiplication
    v = [0] * 512
    v[0] = 1
    vals = [v[0]]

    for step in range(200):
        nv = [0] * 512
        for i in range(512):
            if v[i] == 0:
                continue
            for j in range(512):
                if T[i][j] == 0:
                    continue
                nv[j] = (nv[j] + v[i] * T[i][j]) % MOD
        v = nv
        vals.append(v[0])

    # Find minimal polynomial
    L, C = berlekamp_massey(vals, MOD)

    # Build modular polynomial (index = degree)
    modpoly = [C[L - j] % MOD for j in range(L + 1)]

    # Compute x^(10^10000) mod modpoly
    n = pow(10, 10000)
    base = [0] * L
    base[1] = 1  # polynomial x

    result_poly = poly_pow(base, n, modpoly, MOD)

    # Evaluate: f(n) = sum result_poly[i] * vals[i]
    answer = 0
    for i in range(L):
        answer = (answer + result_poly[i] * vals[i]) % MOD

    print(answer)

if __name__ == "__main__":
    solve()
