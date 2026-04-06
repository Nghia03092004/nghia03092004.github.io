"""
Problem 223: Almost Right-angled Triangles I

Count ordered triples (a, b, c) with a <= b <= c,
a^2 + b^2 = c^2 + 1, and a + b + c <= 25,000,000.

Key: (c-b)(c+b) = (a-1)(a+1). Let d = c-b, e = c+b, d*e = (a-1)(a+1).
b = (e-d)/2, c = (e+d)/2, perimeter = a + e.
Need d,e same parity, d <= e, b >= a (e-d >= 2a), a+e <= L.

For a even: n = a^2-1 is odd, so d,e both odd. n = (a-1)(a+1), gcd=1.
For a odd: n = a^2-1 divisible by 4. d,e both even. n/4 = ((a-1)/2)*((a+1)/2), gcd=1.

Factor using coprime factorization to enumerate divisors efficiently.
"""

import math

def get_divisors_via_spf(n, spf):
    """Get all divisors of n using smallest prime factor array."""
    if n == 0:
        return []
    if n == 1:
        return [1]
    divs = [1]
    tmp = n
    while tmp > 1:
        p = spf[tmp]
        exp = 0
        while tmp % p == 0:
            tmp //= p
            exp += 1
        new_divs = []
        pk = 1
        for e in range(exp):
            pk *= p
            for d in divs:
                new_divs.append(d * pk)
        divs.extend(new_divs)
    return divs

def solve():
    L = 25_000_000
    count = 0

    # Case a = 1: b = c, perimeter = 1 + 2b <= L
    count += (L - 1) // 2

    # Sieve smallest prime factor up to L/3 + 2
    SMAX = L // 3 + 2
    spf = list(range(SMAX + 1))  # spf[i] = i initially
    for i in range(2, int(math.isqrt(SMAX)) + 1):
        if spf[i] == i:  # i is prime
            for j in range(i * i, SMAX + 1, i):
                if spf[j] == j:
                    spf[j] = i

    def get_divisors(n):
        if n <= 1:
            return [1] if n == 1 else []
        divs = [1]
        tmp = n
        while tmp > 1:
            p = spf[tmp] if tmp <= SMAX else tmp  # for large tmp, it's prime
            exp = 0
            while tmp % p == 0:
                tmp //= p
                exp += 1
            sz = len(divs)
            pk = 1
            for e in range(exp):
                pk *= p
                for j in range(sz):
                    divs.append(divs[j] * pk)
        return divs

    # Case a even (a = 2, 4, 6, ...):
    # n = (a-1)*(a+1) is odd, gcd(a-1, a+1) = 1.
    # Divisors of n = divisors of (a-1) * divisors of (a+1).
    for a in range(2, L // 3 + 1, 2):
        s = a - 1  # odd
        s2 = a + 1  # odd
        divs_s = get_divisors(s)
        divs_s2 = get_divisors(s2)
        for d1 in divs_s:
            for d2 in divs_s2:
                d = d1 * d2
                e = (s // d1) * (s2 // d2)
                if d > e:
                    continue
                if e - d < 2 * a:
                    continue
                if a + e > L:
                    continue
                count += 1

    # Case a odd (a = 3, 5, 7, ...):
    # n = (a-1)*(a+1) divisible by 4. d,e both even.
    # u = (a-1)//2, u1 = (a+1)//2, gcd(u, u1) = 1.
    # d' * e' = u * u1, d = 2d', e = 2e'.
    for a in range(3, L // 3 + 1, 2):
        u = (a - 1) // 2
        u1 = (a + 1) // 2
        divs_u = get_divisors(u)
        divs_u1 = get_divisors(u1)
        for d1 in divs_u:
            for d2 in divs_u1:
                dp = d1 * d2
                ep = (u // d1) * (u1 // d2)
                if dp > ep:
                    continue
                d = 2 * dp
                e = 2 * ep
                if e - d < 2 * a:
                    continue
                if a + e > L:
                    continue
                count += 1

    print(count)

if __name__ == "__main__":
    solve()
