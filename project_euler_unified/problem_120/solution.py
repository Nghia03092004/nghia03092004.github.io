"""
Problem 120: Square Remainders

For 3 <= a <= 1000, find the sum of r_max where r_max is the maximum
remainder when (a-1)^n + (a+1)^n is divided by a^2.

Key insight: r_max = a*(a-1) for odd a, a*(a-2) for even a.
"""

def r_max(a):
    if a % 2 == 1:
        return a * (a - 1)
    else:
        return a * (a - 2)

answer = sum(r_max(a) for a in range(3, 1001))
print(answer)
