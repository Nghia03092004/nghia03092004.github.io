"""
Problem 171: Finding Numbers for Which the Sum of the Squares of the Digits
Is a Perfect Square

Find the last nine digits of the sum of all n, 0 < n < 10^20, such that
f(n) = sum of squares of digits of n is a perfect square.

Method: Digit DP tracking (count, value_sum) keyed by digit-square-sum.
"""

def solve():
    MOD = 10**9
    D = 20
    S_MAX = D * 81  # 1620

    cnt = [0] * (S_MAX + 1)
    tot = [0] * (S_MAX + 1)
    cnt[0] = 1

    for _ in range(D):
        nc = [0] * (S_MAX + 1)
        nt = [0] * (S_MAX + 1)
        for s in range(S_MAX + 1):
            if cnt[s] == 0:
                continue
            for d in range(10):
                ns = s + d * d
                if ns > S_MAX:
                    break
                nc[ns] = (nc[ns] + cnt[s]) % MOD
                nt[ns] = (nt[ns] + 10 * tot[s] + d * cnt[s]) % MOD
        cnt, tot = nc, nt

    ans = 0
    j = 1
    while j * j <= S_MAX:
        ans = (ans + tot[j * j]) % MOD
        j += 1

    print(ans)

if __name__ == "__main__":
    solve()
