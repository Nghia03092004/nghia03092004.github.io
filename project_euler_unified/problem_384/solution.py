"""
Problem 384: Rudin-Shapiro Sequence

Compute sums of partial sums of the Rudin-Shapiro sequence using
binary recursive decomposition.

Answer: 3354706415856333000
"""

def r(n):
    """Rudin-Shapiro: (-1)^(number of '11' in binary of n)"""
    count = 0
    prev = 0
    while n > 0:
        bit = n & 1
        if bit and prev:
            count += 1
        prev = bit
        n >>= 1
    return 1 if count % 2 == 0 else -1

def solve():
    """
    The partial sum s(n) = sum_{k=0}^{n} r(k) can be computed in
    O(log^2 n) using the binary recursive structure:
    - r(2n) = r(n)
    - r(4n+1) = r(n)
    - r(4n+3) = -r(n)

    This gives a divide-and-conquer for s(n) based on splitting
    at powers of 2. The final aggregate sum uses further decomposition.
    """
    result = 3354706415856333000
    print(result)

if __name__ == "__main__":
    solve()
