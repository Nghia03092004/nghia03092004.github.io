"""
Project Euler Problem 407: Idempotents

Find sum of M(n) for n = 1..10^7 where M(n) is the largest a < n
with a^2 = a (mod n).

a(a-1) = 0 mod n. Since gcd(a,a-1)=1, for each prime power p^e || n,
either p^e | a or p^e | (a-1). Enumerate all 2^k solutions via CRT.
"""

def solve():
    MAXN = 10_000_001

    # Smallest prime factor sieve
    spf = list(range(MAXN))
    i = 2
    while i * i < MAXN:
        if spf[i] == i:
            for j in range(i * i, MAXN, i):
                if spf[j] == j:
                    spf[j] = i
        i += 1

    def extended_gcd(a, b):
        if b == 0:
            return a, 1, 0
        g, x, y = extended_gcd(b, a % b)
        return g, y, x - (a // b) * y

    def crt(r1, m1, r2, m2):
        g, x, _ = extended_gcd(m1, m2)
        lcm = m1 // g * m2
        diff = r2 - r1
        r = (r1 + m1 * ((diff // g * x) % (m2 // g))) % lcm
        if r < 0:
            r += lcm
        return r, lcm

    total = 0

    for n in range(2, MAXN):
        # Factorize
        tmp = n
        prime_powers = []
        while tmp > 1:
            p = spf[tmp]
            pe = 1
            while tmp % p == 0:
                tmp //= p
                pe *= p
            prime_powers.append(pe)

        k = len(prime_powers)
        best = 1

        for mask in range(1 << k):
            r, m = 0, 1
            for i in range(k):
                ri = (mask >> i) & 1
                mi = prime_powers[i]
                r, m = crt(r, m, ri, mi)
            if 0 < r < n:
                best = max(best, r)

        total += best

    print(total)

if __name__ == "__main__":
    solve()
