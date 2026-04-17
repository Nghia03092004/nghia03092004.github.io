"""
Problem 811: Bitwise Recursion

f(n, k) = n              if k = 0
         = f(n XOR k, k >> 1)  if k > 0

Key insight: f(n, k) = n XOR g(k) where g(k) = XOR of all right-shifts of k.
Bit j of g(k) = XOR of bits j, j+1, j+2, ... of k (suffix XOR parity).

We compute S(N) = sum_{k=0}^{N} sum_{n=0}^{N} f(n, k) mod (10^9 + 7)
             = sum_{k=0}^{N} sum_{n=0}^{N} (n XOR g(k))
"""

MOD = 10**9 + 7

def g(k):
    """Compute g(k) = XOR of k, k>>1, k>>2, ... (suffix XOR parity)."""
    result = 0
    while k:
        result ^= k
        k >>= 1
    return result

def xor_sum(N, c):
    """Compute sum_{n=0}^{N} (n XOR c) using bitwise analysis."""
    if N < 0:
        return 0
    total = 0
    # For each bit position j, count how many n in [0..N] have bit j set in (n XOR c)
    for j in range(61):
        bit_c = (c >> j) & 1
        # Count of n in [0..N] with bit j set
        full_cycles = (N + 1) >> (j + 1)
        remainder = (N + 1) & ((1 << (j + 1)) - 1)
        ones_in_bit_j = full_cycles * (1 << j) + max(0, remainder - (1 << j))

        if bit_c == 1:
            # XOR flips: ones become zeros and vice versa
            count_set = (N + 1) - ones_in_bit_j
        else:
            count_set = ones_in_bit_j
        total += count_set * (1 << j)
    return total

# --- Method 1: Brute force for small N ---
def solve_brute(N):
    """Brute force: directly compute S(N)."""
    total = 0
    for k in range(N + 1):
        gk = g(k)
        for n in range(N + 1):
            total += n ^ gk
    return total

# --- Method 2: Using xor_sum with g(k) enumeration (medium N) ---
def solve_medium(N):
    """Compute S(N) by iterating over k and using xor_sum for inner sum."""
    total = 0
    for k in range(N + 1):
        gk = g(k)
        total += xor_sum(N, gk)
    return total

# --- Method 3: Digit DP for large N ---
def solve_digit_dp(N, mod):
    """
    Compute S(N) = sum_{k=0}^{N} sum_{n=0}^{N} (n XOR g(k)) mod `mod`
    using digit DP on both k and n simultaneously.

    For each bit position j, we need to count:
      For how many (n, k) pairs in [0,N]x[0,N] is bit j of (n XOR g(k)) equal to 1?
    Then S(N) = sum_j 2^j * count_j.

    For bit j of (n XOR g(k)): this is n_j XOR g(k)_j.
    g(k)_j = k_j XOR k_{j+1} XOR k_{j+2} XOR ...

    We compute per-bit contributions.
    """
    if N < 0:
        return 0

    B = N.bit_length()
    total = 0

    for j in range(B + 1):
        # Count pairs (n, k) in [0..N]^2 where n_j XOR suffix_xor_j(k) = 1

        # Count of n in [0..N] with bit j = 1
        full_n = (N + 1) >> (j + 1)
        rem_n = (N + 1) & ((1 << (j + 1)) - 1)
        ones_n = full_n * (1 << j) + max(0, rem_n - (1 << j))
        zeros_n = (N + 1) - ones_n

        # Count of k in [0..N] with g(k)_j = 1
        # g(k)_j = suffix XOR parity from bit j onward
        # We need to count k in [0..N] where bits j,j+1,...,B-1 of k have odd parity
        # This requires digit DP on k
        ones_k = count_suffix_xor_odd(N, j, B)
        zeros_k = (N + 1) - ones_k

        # Pairs where n_j XOR g(k)_j = 1: ones_n * zeros_k + zeros_n * ones_k
        count_j = (ones_n % mod) * (zeros_k % mod) + (zeros_n % mod) * (ones_k % mod)
        total = (total + (count_j % mod) * pow(2, j, mod)) % mod

    return total

def count_suffix_xor_odd(N, j, B):
    """
    Count k in [0..N] where XOR of bits j, j+1, ..., B-1 of k is 1.
    Uses digit DP on bits from MSB down.
    """
    if j >= B:
        return 0

    # Extract bits of N
    bits = []
    for i in range(B - 1, -1, -1):
        bits.append((N >> i) & 1)

    # DP states: (position, tight, xor_parity_of_bits_from_j_onward_so_far)
    # We process bits from MSB (position 0) to LSB (position B-1)
    # Position i corresponds to bit (B-1-i)
    # We track parity of bits at positions >= j

    # dp[tight][parity] = count
    dp = {}
    dp[(True, 0)] = 1  # start: tight, parity 0

    for i in range(B):
        actual_bit_pos = B - 1 - i
        new_dp = {}
        for (tight, par), cnt in dp.items():
            max_d = bits[i] if tight else 1
            for d in range(max_d + 1):
                new_tight = tight and (d == bits[i])
                new_par = par
                if actual_bit_pos >= j:
                    new_par = par ^ d
                key = (new_tight, new_par)
                new_dp[key] = new_dp.get(key, 0) + cnt
        dp = new_dp

    # Count entries with parity = 1
    result = 0
    for (tight, par), cnt in dp.items():
        if par == 1:
            result += cnt
    return result

# --- Verify small cases ---
N_small = 15
brute_ans = solve_brute(N_small)
medium_ans = solve_medium(N_small)
dp_ans = solve_digit_dp(N_small, 10**18 + 9)  # use large mod to get exact

assert brute_ans == medium_ans, f"Brute vs medium mismatch: {brute_ans} vs {medium_ans}"
assert brute_ans == dp_ans, f"Brute vs DP mismatch: {brute_ans} vs {dp_ans}"

# Verify g is an involution
for k in range(100):
    assert g(g(k)) == k, f"g is not involution at k={k}"

# Verify g values
assert g(0) == 0
assert g(1) == 1
assert g(2) == 3
assert g(3) == 2
assert g(7) == 5
assert g(13) == 9

# Verify concrete f values
assert 5 ^ g(6) == 1  # f(5, 6) = 1
assert 5 ^ g(4) == 2  # f(5, 4) = 2

# --- Compute answer for large N ---
N_large = 10**18
answer = solve_digit_dp(N_large, MOD)
print(answer)
