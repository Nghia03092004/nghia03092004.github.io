"""
Project Euler Problem 516: 5-Smooth Totients

Find S(10^12) mod 2^32, where S(L) is the sum of all n <= L
such that phi(n) is a 5-smooth (Hamming) number.
"""

from sympy import isprime

LIMIT = 10**12
MOD = 2**32

def solve():
    # Step 1: Generate all Hamming numbers <= LIMIT
    hamming = []
    a = 1
    while a <= LIMIT:
        b = a
        while b <= LIMIT:
            c = b
            while c <= LIMIT:
                hamming.append(c)
                c *= 5
            b *= 3
        a *= 2
    hamming.sort()

    # Step 2: Find Hamming primes (h+1 is prime, h is Hamming, excluding 2,3,5)
    hprimes = []
    for h in hamming:
        if h >= 2 and isprime(h + 1):
            p = h + 1
            if p not in (2, 3, 5):
                hprimes.append(p)
    hprimes.sort()

    # Step 3: DFS over subsets of Hamming primes, multiply by Hamming numbers
    ans = 0

    def dfs(idx, prod):
        nonlocal ans
        max_h = LIMIT // prod
        for h in hamming:
            if h > max_h:
                break
            ans = (ans + prod * h) % MOD

        for i in range(idx, len(hprimes)):
            if prod > LIMIT // hprimes[i]:
                break
            dfs(i + 1, prod * hprimes[i])

    dfs(0, 1)
    print(ans)

if __name__ == "__main__":
    solve()
