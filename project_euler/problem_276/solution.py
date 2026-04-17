#!/usr/bin/env python3
"""
Problem 276: Primitive Triangles

Count primitive triangles (gcd(a,b,c)=1) with a<=b<=c and perimeter <= 10^7.

Method:
1. Compute t(s) for all s using the linear recurrence from the GF
   G(x) = x^3 / ((1-x^2)(1-x^3)(1-x^4)):
   t(n) = t(n-2)+t(n-3)+t(n-4)-t(n-5)-t(n-6)-t(n-7)+t(n-9) for n >= 12

2. Apply Mobius sieve: for n from 1 to N/2, subtract t[n] from t[kn] for k>=2.

3. Sum primitive counts.
"""

def solve(N=10**7):
    # Step 1: Compute triangle counts using recurrence
    t = [0] * (N + 1)
    # Initial values (OEIS A005044)
    if N >= 3: t[3] = 1
    if N >= 5: t[5] = 1
    if N >= 6: t[6] = 1
    if N >= 7: t[7] = 2
    if N >= 8: t[8] = 1
    if N >= 9: t[9] = 3
    if N >= 10: t[10] = 2
    if N >= 11: t[11] = 4

    for n in range(12, N + 1):
        t[n] = t[n-2] + t[n-3] + t[n-4] - t[n-5] - t[n-6] - t[n-7] + t[n-9]

    # Step 2: Mobius sieve
    for n in range(1, N // 2 + 1):
        if t[n] == 0:
            continue
        for k in range(2 * n, N + 1, n):
            t[k] -= t[n]

    # Step 3: Sum
    answer = sum(t[n] for n in range(3, N + 1))
    return answer

if __name__ == "__main__":
    N = 10**7
    answer = solve(N)
    print(answer)
