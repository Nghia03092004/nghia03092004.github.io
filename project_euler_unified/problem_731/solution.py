"""
Problem 731: A Stoneham Number

A = sum_{i=1}^{inf} 1/(3^i * 10^{3^i})
A(n) = 10 digits of A starting at position n.

Key: digits at position n come from terms with 3^i <= n+9.
For each term, compute digits of 1/3^i at offset n - 3^i using modular exponentiation.
"""

def digits_of_inv3i(i, start, count):
    """Get 'count' decimal digits of 1/3^i starting at position 'start' (0-indexed after decimal point).

    The j-th digit (0-indexed) of 1/3^i is floor(10^{j+1} / 3^i) mod 10.
    Equivalently, compute 10^{start+k} mod (10 * 3^i) for k = 0..count-1.
    """
    mod = 3 ** i
    result = 0
    for k in range(count):
        pos = start + k
        # digit at position pos (1-indexed) = floor(10^pos mod (10*mod)) / mod
        # = (10^pos mod (10*mod)) // mod ... no, simpler:
        # digit = (10^pos * 10 // mod) ... nah.
        # 1/3^i = 0.d1 d2 d3 ...
        # d_j (1-indexed) = floor(10^j / 3^i) - 10 * floor(10^{j-1} / 3^i)
        # = floor(10 * (10^{j-1} mod 3^i) / 3^i)
        # So: remainder r = 10^{j-1} mod 3^i, then digit = (10*r) // 3^i
        # and new remainder = (10*r) mod 3^i
        pass

    # Better approach: compute starting remainder, then iterate
    mod = 3 ** i
    r = pow(10, start, mod)  # 10^start mod 3^i
    digits = []
    for k in range(count):
        d = (10 * r) // mod
        r = (10 * r) % mod
        digits.append(d)
    return digits

def stoneham_A(n, num_digits=10):
    """Compute A(n) = 10 decimal digits of the Stoneham number starting at position n."""
    # Find all i with 3^i <= n + num_digits - 1
    max_i = 0
    power3 = 3
    while power3 <= n + num_digits - 1:
        max_i += 1
        power3 *= 3

    # Accumulate digit contributions
    # We work with a large integer: sum of contributions * 10^extra for carries
    extra = 20  # extra digits for carry safety
    total = 0

    power3 = 3
    for i in range(1, max_i + 1):
        offset = n - power3  # position in 1/3^i expansion
        if offset < 0:
            power3 *= 3
            continue
        digs = digits_of_inv3i(i, offset, num_digits + extra)
        # Convert to integer
        val = 0
        for d in digs:
            val = val * 10 + d
        total += val
        power3 *= 3

    # Extract top num_digits digits
    s = str(total)
    # The result is the first num_digits digits of the sum
    # But we need to handle carries: the sum might produce more digits
    result = s[:num_digits]
    return int(result)

# Verify
a100 = stoneham_A(100)
print(f"A(100) = {a100}")  # Expected: 4938271604
assert a100 == 4938271604, f"Got {a100}"

# A(10^8) - this works but takes a moment
a_1e8 = stoneham_A(10**8)
print(f"A(10^8) = {a_1e8}")  # Expected: 2584642393

# A(10^16)
a_1e16 = stoneham_A(10**16)
print(f"A(10^16) = {a_1e16}")
