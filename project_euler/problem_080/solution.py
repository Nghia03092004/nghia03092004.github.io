"""
Problem 80: Square Root Digital Expansion

For the first 100 natural numbers, find the total of the digital sums
of the first 100 decimal digits for all irrational square roots.
Answer: 40886
"""

from math import isqrt

def main():
    scale = 10**198
    perfect_squares = {i * i for i in range(1, 11)}
    total = 0

    for n in range(1, 101):
        if n in perfect_squares:
            continue

        root = isqrt(n * scale)
        digits = str(root)[:100]
        total += sum(int(c) for c in digits)

    print(total)


if __name__ == "__main__":
    main()
