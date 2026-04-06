"""
Problem 479: Roots on the Rise

S(n) = sum_{k=1}^{n} sum_{p=1}^{n} (1-k^2)^p  mod 10^9+7

Key insight: (a_k+b_k)(b_k+c_k)(c_k+a_k) = 1 - k^2 from Vieta's formulas.

For each k >= 2, the inner sum is a geometric series.
For k = 1, 1-k^2 = 0 so contribution is 0.
"""

MOD = 1000000007

def solve(n):
    ans = 0

    for k in range(2, n + 1):
        k2 = k * k % MOD
        q = (1 - k2) % MOD  # q = 1 - k^2 mod MOD

        # sum_{p=1}^{n} q^p = (q^{n+1} - q) / (q - 1)
        # q - 1 = -k^2
        # So sum = (q^{n+1} - q) / (-k^2) = -(q^{n+1} - q) * inv(k^2)

        qn1 = pow(q, n + 1, MOD)
        num = (qn1 - q) % MOD

        inv_k2 = pow(k2, MOD - 2, MOD)
        term = (-num * inv_k2) % MOD

        ans = (ans + term) % MOD

    return ans % MOD

if __name__ == "__main__":
    # Verify S(4) = 51160
    print(f"S(4) = {solve(4)}")

    # Compute S(10^6)
    result = solve(1000000)
    print(f"S(10^6) mod 10^9+7 = {result}")
