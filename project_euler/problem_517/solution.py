"""
Project Euler Problem 517: A Real Recursion

For real a > 1: g_a(x) = 1 for x < a, g_a(x) = g_a(x-1) + g_a(x-a) for x >= a.
G(n) = g_{sqrt(n)}(n).
Find sum of G(p) mod 10^9+7 for primes p with 10^7 < p < 10^7+10^4.
"""
import math

MOD = 1000000007

def solve():
    HI = 10010000
    LO = 10000001

    # Sieve primes up to HI
    is_prime = bytearray([1]) * (HI + 1)
    is_prime[0] = is_prime[1] = 0
    for i in range(2, int(HI**0.5) + 1):
        if is_prime[i]:
            is_prime[i*i::i] = bytearray(len(is_prime[i*i::i]))

    # Precompute factorials and inverse factorials
    MAXN = HI + 1
    fac = [1] * MAXN
    for i in range(1, MAXN):
        fac[i] = fac[i-1] * i % MOD
    inv_fac = [1] * MAXN
    inv_fac[MAXN-1] = pow(fac[MAXN-1], MOD-2, MOD)
    for i in range(MAXN-2, -1, -1):
        inv_fac[i] = inv_fac[i+1] * (i+1) % MOD

    def C(n, k):
        if k < 0 or k > n:
            return 0
        return fac[n] * inv_fac[k] % MOD * inv_fac[n-k] % MOD

    ans = 0
    for p in range(LO, HI):
        if not is_prime[p]:
            continue
        a = math.sqrt(p)
        K = int(math.floor(a))

        gp = 0
        for k in range(K + 1):
            nk = p - 1 - int(math.floor(k * (a - 1.0)))
            if nk < k:
                break
            gp = (gp + C(nk, k)) % MOD

        ans = (ans + gp) % MOD

    print(ans)

if __name__ == "__main__":
    solve()
