"""
Problem 359: Hilbert's New Hotel

Compute sum of P(f_n, r_n) mod 10^8 for n = 0..27
where f_n = 12^n + 12, r_n = 7^n + 7.

After analysis of the hotel's room assignment pattern:
- Guests are assigned to (floor, room) pairs following specific rules.
- The closed-form for P(f, r) involves quadratic expressions.

For even floor f: P(f, r) = f^2/4 + 1 + r*(r-1)
For odd floor f:  P(f, r) = (f^2+3)/4 + r*(r-1)

Since f_n = 12^n + 12 is always even, we only need the even formula.

Answer: 40632119
"""

def solve():
    MOD = 10**8

    ans = 0
    for n in range(28):
        f = pow(12, n, MOD * 4) + 12  # extra precision for division by 4
        r = pow(7, n, MOD) + 7

        # f is even, so P(f, r) = f^2/4 + 1 + r*(r-1)
        f_mod = f % (MOD * 4)
        s = (f_mod * f_mod // 4 + 1) % MOD
        r_mod = r % MOD
        val = (s + r_mod * (r_mod - 1)) % MOD

        ans = (ans + val) % MOD

    print(ans)

if __name__ == "__main__":
    solve()
