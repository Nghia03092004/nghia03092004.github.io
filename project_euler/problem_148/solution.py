"""
Problem 148: Exploring Pascal's Triangle

How many entries in the first 10^9 rows of Pascal's triangle are not divisible by 7?

By Lucas' theorem, C(n,k) is not divisible by 7 iff each base-7 digit of k <= that of n.
Row n contributes product(d_i + 1) where d_i are the base-7 digits of n.

S(N) = sum_{n=0}^{N-1} product(d_i(n) + 1)

Recursion (process from LSB to MSB):
    S(d * 7^k + R) = d*(d+1)/2 * 28^k + (d+1) * S(R)
"""

def solve():
    N = 10**9

    # Convert N to base 7 (least significant digit first)
    digits = []
    tmp = N
    while tmp > 0:
        digits.append(tmp % 7)
        tmp //= 7

    # Process from LSB (i=0) to MSB
    result = 0
    pow28 = 1  # 28^i
    for i in range(len(digits)):
        d = digits[i]
        result = d * (d + 1) // 2 * pow28 + (d + 1) * result
        pow28 *= 28

    print(result)

if __name__ == "__main__":
    solve()
