"""
Problem 508: Integers in Base i-1
Represent Gaussian integers in base (i-1) with digits {0, 1}.
Compute digit sums and their aggregates.
"""

def to_base_im1(n_real: int, n_imag: int = 0) -> list:
    """
    Convert Gaussian integer n_real + n_imag*i to base (i-1).
    Returns list of digits (LSB first), each in {0, 1}.

    Division by (i-1):
    (a + bi) / (i - 1) = (a + bi)(-i - 1) / |i-1|^2
                        = (a + bi)(-1 - i) / 2
                        = (-a - ai - bi - bi^2) / 2
                        = (-a + b)/2 + (-a - b)i/2
    """
    re, im = n_real, n_imag
    digits = []
    seen = set()

    while re != 0 or im != 0:
        state = (re, im)
        if state in seen:
            break  # safety: avoid infinite loops
        seen.add(state)

        # Digit is the parity of the real part
        d = re % 2
        if d < 0:
            d += 2
        digits.append(d)

        # Subtract digit and divide by (i-1)
        re_new = (-(re - d) + im) // 2
        im_new = (-(re - d) - im) // 2
        re, im = re_new, im_new

    if not digits:
        digits = [0]

    return digits

def digit_sum(n_real: int, n_imag: int = 0):
    """Sum of digits in the base-(i-1) representation."""
    return sum(to_base_im1(n_real, n_imag))

def verify_representation(n_real: int, n_imag: int = 0) -> bool:
    """Verify that the base-(i-1) representation reconstructs the original number."""
    digits = to_base_im1(n_real, n_imag)

    # Reconstruct: sum d_k * (i-1)^k
    # (i-1)^k computed iteratively
    re_sum, im_sum = 0, 0
    pow_re, pow_im = 1, 0  # (i-1)^0 = 1

    for d in digits:
        re_sum += d * pow_re
        im_sum += d * pow_im
        # Multiply power by (i-1): (a+bi)(i-1) = ai - a + bi^2 - bi = -(a+b) + (a-b)i
        new_re = -(pow_re + pow_im)
        new_im = pow_re - pow_im
        pow_re, pow_im = new_re, new_im

    return re_sum == n_real and im_sum == n_imag

def sum_digit_sums(N: int):
    """Compute sum of f(n) for n = 1 to N where f(n) is the digit sum in base i-1."""
    total = 0
    for n in range(1, N + 1):
        total += digit_sum(n)
    return total

# Verify representations for small integers
print("Verification of base-(i-1) representations:")
all_ok = True
for a in range(-20, 21):
    for b in range(-20, 21):
        if not verify_representation(a, b):
            print(f"  FAILED: {a} + {b}i")
            all_ok = False
if all_ok:
    print("  All verifications passed for -20..20 x -20..20")

# Show some representations
print("\nBase-(i-1) representations of small positive integers:")
for n in range(1, 21):
    digits = to_base_im1(n)
    binary_str = "".join(str(d) for d in reversed(digits))
    ds = sum(digits)
    print(f"  {n:3d} = {binary_str} (digit sum = {ds})")

# Compute aggregate
N = 1000
total = sum_digit_sums(N)
print(f"\nSum of digit sums for n=1..{N}: {total}")
