"""
Project Euler Problem 40: Champernowne's Constant

Find d_1 * d_10 * d_100 * d_1000 * d_10000 * d_100000 * d_1000000
where d_n is the n-th digit of Champernowne's constant 0.123456789101112...
"""

def champernowne_digit(n: int):
    """Find the n-th digit (1-indexed) of the Champernowne sequence."""
    k = 1
    count = 9
    start = 1

    while n > k * count:
        n -= k * count
        k += 1
        count *= 10
        start *= 10

    # n is now position within the k-digit block (1-indexed)
    number = start + (n - 1) // k
    digit_pos = (n - 1) % k

    return int(str(number)[digit_pos])

def solve():
    positions = [10**i for i in range(7)]  # 1, 10, 100, ..., 1000000
    digits = []
    product = 1

    for pos in positions:
        d = champernowne_digit(pos)
        digits.append((pos, d))
        product *= d

    return product, digits

product, digits = solve()

print("Champernowne's constant digit lookup:")
for pos, d in digits:
    print(f"  d_{pos:>7d} = {d}")

print(f"\nProduct = {' x '.join(str(d) for _, d in digits)} = {product}")

# Verification using brute force
champernowne = ""
i = 1
while len(champernowne) < 1_000_001:
    champernowne += str(i)
    i += 1

product_verify = 1
for pos in [1, 10, 100, 1000, 10000, 100000, 1000000]:
    product_verify *= int(champernowne[pos - 1])

print(f"Verification (brute force): {product_verify}")
