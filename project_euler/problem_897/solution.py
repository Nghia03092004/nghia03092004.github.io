"""
Problem 897: Base-b Palindromes
Count and enumerate palindromic numbers in various bases.
Key formula: C_b(k) = (b-1) * b^(floor((k-1)/2)) for k-digit palindromes.
"""

def to_base(n, b):
    """Convert n to base b, return list of digits (most significant first)."""
    if n == 0:
        return [0]
    digits = []
    while n > 0:
        digits.append(n % b)
        n //= b
    return digits[::-1]

def is_palindrome_base(n, b):
    """Check if n is a palindrome in base b."""
    digits = to_base(n, b)
    return digits == digits[::-1]

def count_k_digit_palindromes(k, b):
    """Count k-digit palindromes in base b using the formula."""
    if k <= 0:
        return 0
    return (b - 1) * (b ** ((k - 1) // 2))

def count_palindromes_up_to(N, b):
    """Count palindromes in base b that are <= N, by enumeration."""
    count = 0
    for n in range(1, N + 1):
        if is_palindrome_base(n, b):
            count += 1
    return count

def count_palindromes_formula(N, b):
    """Count palindromes <= N using digit-by-digit analysis."""
    if N <= 0:
        return 0
    digits = to_base(N, b)
    K = len(digits)
    # Count all palindromes with fewer than K digits
    total = sum(count_k_digit_palindromes(k, b) for k in range(1, K))
    # Count K-digit palindromes <= N by generating them
    half_len = (K + 1) // 2
    # Iterate over first half
    start = b ** (half_len - 1)
    end = b ** half_len
    for h in range(start, end):
        h_digits = to_base(h, b)
        if K % 2 == 0:
            pal_digits = h_digits + h_digits[::-1]
        else:
            pal_digits = h_digits + h_digits[-2::-1]
        # Convert palindrome back to number
        val = 0
        for d in pal_digits:
            val = val * b + d
        if val <= N:
            total += 1
        elif h_digits[0] > digits[0]:
            break
    return total

def generate_palindromes(max_val, b):
    """Generate all base-b palindromes up to max_val."""
    pals = []
    for k in range(1, 40):
        half_len = (k + 1) // 2
        start = 1 if half_len == 1 else b ** (half_len - 1)
        end = b ** half_len
        for h in range(start, end):
            h_digits = to_base(h, b)
            if k % 2 == 0:
                pal_digits = h_digits + h_digits[::-1]
            else:
                pal_digits = h_digits + h_digits[-2::-1]
            val = 0
            for d in pal_digits:
                val = val * b + d
            if val > max_val:
                return sorted(pals)
            pals.append(val)
    return sorted(pals)

# --- Verification ---
print("=== C_b(k) Formula Verification ===")
for b in [2, 3, 10]:
    for k in range(1, 6):
        formula = count_k_digit_palindromes(k, b)
        # Count directly
        lo = b ** (k - 1) if k > 1 else 1
        hi = b ** k - 1
        direct = sum(1 for n in range(lo, hi + 1) if is_palindrome_base(n, b))
        match = "OK" if formula == direct else "FAIL"
        print(f"  b={b}, k={k}: formula={formula}, direct={direct} {match}")
        assert formula == direct

print("\n=== Palindrome Counts P_b(N) ===")
for b in [2, 10]:
    for N in [31, 100, 999, 1000]:
        brute = count_palindromes_up_to(N, b)
        fast = count_palindromes_formula(N, b)
        match = "OK" if brute == fast else "FAIL"
        print(f"  P_{b}({N}) = {brute} (brute), {fast} (formula) {match}")

print("\n=== Base-2 Palindromes <= 31 ===")
pals = generate_palindromes(31, 2)
for p in pals:
    print(f"  {p} = {''.join(map(str, to_base(p, 2)))}_2")

print("\n=== Multi-base palindromes (base 2 and 10) <= 1000 ===")
for n in range(1, 1001):
    if is_palindrome_base(n, 2) and is_palindrome_base(n, 10):
        print(f"  {n} = {''.join(map(str, to_base(n, 2)))}_2 = {n}_{10}")

answer = count_palindromes_formula(10**6, 10)
print(f"\nAnswer: P_10(10^6) = {answer}")

# --- 4-Panel Visualization ---
