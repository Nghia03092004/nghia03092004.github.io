"""
Problem 356: Largest Roots of Cubic Polynomials

For g(x) = x^3 - 2^n * x^2 + n, find sum of floor(a(n)^987654321) mod 10^8
where a(n) is the largest real root, summed for n=1..30.

Key insight: a(n) ~ 2^n - n/4^n, so a(n)^k is just below 2^(nk),
meaning floor(a(n)^k) = 2^(nk) - 1 for most n.

Answer: 28010159
"""

def solve():
    MOD = 10**8
    k = 987654321
    ans = 0

    for n in range(1, 31):
        # floor(a(n)^k) = 2^(n*k) - 1
        nk = n * k
        val = (pow(2, nk, MOD) - 1) % MOD
        ans = (ans + val) % MOD

    print(ans)

if __name__ == "__main__":
    solve()
