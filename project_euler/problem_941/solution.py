"""
Problem 941: Binary Palindrome Sums

Compute the sum of all binary palindromes less than 2^30.

A binary palindrome is a positive integer whose binary representation reads
the same forwards and backwards (no leading zeros).

Generation strategy:
    For each bit length k (1..30), generate palindromes by choosing the
    first ceil(k/2) bits (with MSB = 1), then mirroring to produce the
    full k-bit palindrome.

Results:
    - 1-bit: {1}
    - 2-bit: {3}  (11)
    - 3-bit: {5, 7}  (101, 111)
    - Count of binary palindromes < 2^30: computed below

Methods:
    1. gen_binary_palindromes  -- generate all binary palindromes up to max_bits
    2. count_by_bitlength      -- count palindromes grouped by bit length
    3. sum_by_bitlength        -- partial sums by bit length
    4. is_binary_palindrome    -- direct check (verification)
"""

def gen_binary_palindromes(max_bits=30):
    """Generate all binary palindromes with at most max_bits bits, in sorted order."""
    pals = []
    for k in range(1, max_bits + 1):
        if k == 1:
            pals.append(1)
            continue
        half = (k + 1) // 2
        for first_half in range(1 << (half - 1), 1 << half):
            bits = bin(first_half)[2:]
            if k % 2 == 1:
                full = bits + bits[-2::-1]
            else:
                full = bits + bits[::-1]
            val = int(full, 2)
            if val < (1 << max_bits):
                pals.append(val)
    return sorted(pals)

def count_by_bitlength(pals):
    """Return dict mapping bit_length -> count."""
    counts = {}
    for p in pals:
        bl = p.bit_length()
        counts[bl] = counts.get(bl, 0) + 1
    return counts

def sum_by_bitlength(pals):
    """Return dict mapping bit_length -> sum of palindromes."""
    sums = {}
    for p in pals:
        bl = p.bit_length()
        sums[bl] = sums.get(bl, 0) + p
    return sums

def is_binary_palindrome(n):
    """Check if n is a binary palindrome."""
    b = bin(n)[2:]
    return b == b[::-1]

# Verification
assert is_binary_palindrome(1)    # 1
assert is_binary_palindrome(3)    # 11
assert is_binary_palindrome(5)    # 101
assert is_binary_palindrome(7)    # 111
assert not is_binary_palindrome(2)  # 10
assert not is_binary_palindrome(6)  # 110

# Brute-force check for small range
brute_small = [n for n in range(1, 1024) if is_binary_palindrome(n)]
gen_small = [p for p in gen_binary_palindromes(10) if p < 1024]
assert brute_small == gen_small, "Generator mismatch with brute force"

# Computation
pals = gen_binary_palindromes(30)
answer = sum(pals)
print(answer)
