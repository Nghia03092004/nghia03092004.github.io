"""
Problem 80: Square Root Digital Expansion

For the first 100 natural numbers, find the total of the digital sums
of the first 100 decimal digits for all irrational square roots.
Answer: 40886
"""

from decimal import Decimal, getcontext

def main():
    # Set precision high enough (120 digits to be safe)
    getcontext().prec = 120

    perfect_squares = {i * i for i in range(1, 11)}
    total = 0
    digit_sums = {}

    for n in range(1, 101):
        if n in perfect_squares:
            continue

        # Compute sqrt(n) to high precision
        root = Decimal(n).sqrt()

        # Convert to string, remove the decimal point, take first 100 digits
        s = str(root)
        # Remove '0.' prefix if present, or handle integer part
        digits_only = s.replace('.', '')
        # Remove leading zeros that might appear (shouldn't for sqrt)
        # Take first 100 digits
        first_100 = digits_only[:100]
        dsum = sum(int(c) for c in first_100)
        digit_sums[n] = dsum
        total += dsum

    print(total)
