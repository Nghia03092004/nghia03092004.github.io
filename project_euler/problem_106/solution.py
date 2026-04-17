"""
Problem 106: Special Subset Sums: Meta-testing

For n=12, count how many subset pairs (B, C) with |B|=|C| need to be
tested for equality, given that the ordering rule already handles |B|!=|C|.

Formula: sum over k=2..n/2 of C(n,2k) * [C(2k,k)/2 - Catalan(k)]
"""

from math import comb

def catalan(k):
    """k-th Catalan number: C(2k,k) / (k+1)"""
    return comb(2 * k, k) // (k + 1)

def solve(n=12):
    total = 0
    for k in range(2, n // 2 + 1):
        choose_2k = comb(n, 2 * k)
        half_choose = comb(2 * k, k) // 2
        cat_k = catalan(k)
        contribution = choose_2k * (half_choose - cat_k)
        total += contribution
    return total

answer = solve(12)
print(answer)
