"""
Problem 168: Number Rotations

Find all n with 10 <= n < 10^100 such that rotating the last digit to
the front gives a multiple of n. Sum the last 5 digits of all such n.

For each multiplier k (1..9) and last digit r (1..9), generate digits
right-to-left using the recurrence from the multiplication condition.
"""

def solve():
    MOD = 100000
    total = 0

    for k in range(1, 10):
        for r in range(1, 10):
            digit = r
            carry = 0
            last5 = 0
            pow10 = 1  # 10^step mod MOD

            for step in range(100):
                # Place current digit at position 'step' (from right)
                if step < 5:
                    last5 = (last5 + digit * pow10) % MOD
                    pow10 = (pow10 * 10) % MOD

                # Compute next digit and carry
                val = k * digit + carry
                next_digit = val % 10
                next_carry = val // 10

                d = step + 1  # number of digits so far
                if d >= 2 and next_digit == r and next_carry == 0:
                    # Valid d-digit number; check leftmost digit nonzero
                    if digit != 0:
                        total = (total + last5) % MOD

                digit = next_digit
                carry = next_carry

    print(total)

solve()
