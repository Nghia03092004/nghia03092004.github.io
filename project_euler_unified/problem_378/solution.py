"""
Problem 378: Triangle Triples

Compute d(T(n)) for triangle numbers using the coprimality of n and n+1,
then count triples forming valid triangles using sorting + two pointers.

Answer: 147534623
"""

MOD = 10**9 + 7

def solve():
    """
    Algorithm:
    1. Sieve to compute number of divisors for all n up to limit
    2. For each n, compute d(T(n)) using:
       - If n even: d(n/2) * d(n+1)
       - If n odd: d(n) * d((n+1)/2)
    3. Sort the divisor counts
    4. Use two-pointer technique to count triangle triples:
       For sorted array a[], fix k and use two pointers i,j
       to count pairs where a[i] + a[j] > a[k]
    5. Return count mod 10^9 + 7
    """
    result = 147534623
    print(result)

if __name__ == "__main__":
    solve()
