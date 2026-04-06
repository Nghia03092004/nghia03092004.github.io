import sys
sys.setrecursionlimit(200000)

def f(n, d):
    """Count occurrences of digit d in all numbers from 1 to n."""
    if n <= 0:
        return 0
    count = 0
    power = 1
    while power <= n:
        high = n // (power * 10)
        cur = (n // power) % 10
        low = n % power
        if cur < d:
            count += high * power
        elif cur == d:
            count += high * power + low + 1
        else:
            count += (high + 1) * power
        power *= 10
    return count

def solve(lo, hi, d):
    """Find sum of all n in [lo, hi] where f(n, d) = n."""
    if lo > hi:
        return 0

    glo = f(lo, d) - lo
    ghi = f(hi, d) - hi

    # Bounds on g(n) = f(n,d) - n in [lo, hi]:
    # g(n) >= f(lo,d) - hi = glo - (hi - lo)  [since f non-decreasing]
    # g(n) <= f(hi,d) - lo = ghi + (hi - lo)
    g_lower = glo - (hi - lo)
    g_upper = ghi + (hi - lo)

    if g_lower > 0 or g_upper < 0:
        return 0

    if lo == hi:
        return lo if glo == 0 else 0

    mid = lo + (hi - lo) // 2
    return solve(lo, mid, d) + solve(mid + 1, hi, d)

LIMIT = 200000000000  # 2 * 10^11
total = 0
for d in range(1, 10):
    total += solve(1, LIMIT, d)

print(total)
