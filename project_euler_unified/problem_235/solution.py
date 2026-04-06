"""
Problem 235: An Arithmetic Geometric Sequence

Find r (to 12 decimal places) such that
s(r) = sum_{k=1}^{5000} (900 - 3k) * r^{k-1} = -600000000000.
"""

from decimal import Decimal, getcontext

def solve():
    getcontext().prec = 50

    def s_func(r):
        """Compute s(r) using closed form with Decimal for precision."""
        r5000 = r ** 5000
        r5001 = r5000 * r
        rm1 = r - 1

        geo = (r5000 - 1) / rm1
        ageo = (5000 * r5001 - 5001 * r5000 + 1) / (rm1 * rm1)
        return 900 * geo - 3 * ageo

    target = Decimal("-600000000000")

    # Bisection
    lo = Decimal("1.0")
    hi = Decimal("1.01")

    for _ in range(200):
        mid = (lo + hi) / 2
        val = s_func(mid)
        if val > target:
            lo = mid
        else:
            hi = mid

    # Format to 12 decimal places
    result = f"{lo:.12f}"
    print(result)

if __name__ == "__main__":
    solve()
