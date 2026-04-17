"""
Problem 557: Cutting Triangles

Find S(10000) where S(n) = sum of total areas for all valid
triangle cutting quadruples (a,b,c,d) with a+b+c+d <= n.

Constraint: ad = bc, b <= c, all positive integers.

Parameterization: Let g = gcd(a,b), a = g*a', b = g*b', gcd(a',b')=1.
Then c = a'*t, d = b'*t for integer t >= 1.
Total area T = (a'+b')*(g+t).
"""

import math

def solve(N):
    S = 0
    # a' + b' = apb, iterate
    for apb in range(2, N + 1):
        for bp in range(1, apb):
            ap = apb - bp
            if ap < 1:
                continue
            if math.gcd(ap, bp) != 1:
                continue

            max_g = N // apb - 1
            for g in range(1, max_g + 1):
                t_min = max(1, (g * bp + ap - 1) // ap)
                t_max = N // apb - g
                if t_max < t_min:
                    continue

                cnt = t_max - t_min + 1
                sum_t = (t_min + t_max) * cnt // 2
                S += apb * (cnt * g + sum_t)

    return S

# Verify with known value
print(f"S(20) = {solve(20)}")

# Compute the answer
print(f"S(10000) = {solve(10000)}")
