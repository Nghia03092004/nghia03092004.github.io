"""
Problem 288: An Enormous Factorial

Find NF(61, 10^7) mod 61^10, where NF(p,q) = v_p(N(p,q)!)
and N(p,q) = sum_{n=0}^{q} T_n * p^n with T_n from a pseudo-random generator.

Key insight: Since T_n are the base-p digits of N(p,q), Legendre's formula gives:
  v_p(N!) = sum_{j=1}^{q} T_j * (1 + p + ... + p^{j-1})
Mod p^e, the geometric sum caps at e terms.
"""

def solve():
    p = 61
    q = 10_000_000
    e = 10

    mod = p ** e  # 61^10

    # Precompute geometric sums G[k] = 1 + p + p^2 + ... + p^{k-1} mod p^e
    G = [0] * (e + 1)
    pw = 1
    for k in range(1, e + 1):
        G[k] = (G[k - 1] + pw) % mod
        if k < e:
            pw = pw * p % mod

    # Generate T_n and accumulate Legendre sum
    s = 290797
    result = 0

    for n in range(q + 1):
        t = s % p
        if n >= 1:
            idx = min(n, e)
            result = (result + t * G[idx]) % mod
        # Advance RNG
        s = s * s % 50515093

    print(result)

if __name__ == "__main__":
    solve()
