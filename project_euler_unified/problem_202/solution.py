"""
Problem 202: Laserbeam

A laser beam enters vertex C of an equilateral triangle with mirrored sides,
reflects exactly 12017639147 times off the internal surfaces, and exits
through vertex C. Count the number of distinct beam paths.

Approach:
- Unfold reflections into a triangular tessellation.
- n = (R + 3) / 2 = 6008819575 is the lattice parameter.
- Count b in [1, n-1] with b ≡ 2 (mod 3) and gcd(b, n) = 1.
- Use Mobius inversion over squarefree divisors of n.

n = 6008819575 = 5^2 * 11 * 17 * 23 * 29 * 41 * 47
"""

def solve():
    R = 12017639147
    n = (R + 3) // 2  # 6008819575

    # Factor n (extract distinct primes)
    primes = []
    tmp = n
    p = 2
    while p * p <= tmp:
        if tmp % p == 0:
            primes.append(p)
            while tmp % p == 0:
                tmp //= p
        p += 1
    if tmp > 1:
        primes.append(tmp)

    np_ = len(primes)
    ans = 0

    # Inclusion-exclusion over squarefree divisors (Mobius function)
    for mask in range(1 << np_):
        d = 1
        bits = bin(mask).count('1')
        mu = 1 if bits % 2 == 0 else -1
        for i in range(np_):
            if mask & (1 << i):
                d *= primes[i]

        if n % d != 0:
            continue

        nd = n // d
        d_mod3 = d % 3

        if d_mod3 == 0:
            cnt = 0  # 3|d means d*k ≡ 0 mod 3, never ≡ 2
        else:
            dinv3 = pow(d, -1, 3)
            r = (2 * dinv3) % 3
            upper = nd - 1
            if r == 0:
                cnt = max(0, (upper - 3) // 3 + 1) if upper >= 3 else 0
            else:
                cnt = max(0, (upper - r) // 3 + 1) if upper >= r else 0

        ans += mu * cnt

    print(ans)

if __name__ == "__main__":
    solve()
