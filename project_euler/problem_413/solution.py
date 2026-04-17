"""
Project Euler Problem 413: One-child Numbers

A d-digit positive number is a "one-child number" if exactly one of its
substrings is divisible by d.

F(N) = count of one-child numbers less than N.
Given: F(10)=9, F(10^3)=389, F(10^7)=277674.
Find: F(10^19).

Approach: Digit DP for each digit length d from 1 to 19.
For each d, count d-digit numbers where exactly one substring is divisible by d.

Key insight for gcd(d, 10) = 1:
  Define R(j) = (number formed by first j digits) * 10^(d-j) mod d.
  Then substring(i+1..j) is divisible by d iff R(j) == R(i) mod d.
  So we need exactly one pair (i,j) with 0<=i<j<=d where R(i)==R(j).

  This means among R(0), R(1), ..., R(d) (d+1 values mod d),
  exactly one residue appears exactly twice, rest appear at most once.

For d with gcd(d,10) > 1, we handle separately using decomposition.
"""

import sys
from collections import defaultdict

def solve_small_brute(N):
    """Brute-force count of one-child numbers less than N (for verification)."""
    count = 0
    for n in range(1, N):
        s = str(n)
        d = len(s)
        divisible_count = 0
        for i in range(d):
            for j in range(i + 1, d + 1):
                sub = int(s[i:j])
                if sub % d == 0:
                    divisible_count += 1
        if divisible_count == 1:
            count += 1
    return count

def count_one_child_for_d(d):
    """
    Count d-digit one-child numbers using digit DP.

    For each d-digit number, we enumerate all substrings and check divisibility by d.

    DP State: (position, frozenset of residues seen so far for R values,
               current R value mod d, collision_count)

    Since tracking the full set of residues is expensive for large d,
    we use a dictionary-based DP with tuple states.

    For d=1: every 1-digit number has one substring (itself), divisible by 1. All 9 are one-child.
    """
    if d == 1:
        # Every 1-digit number (1-9) has exactly one substring (itself),
        # which is always divisible by 1. So all 9 are one-child.
        return 9

    # For small d, we can use the full residue-tracking DP
    # State: dict mapping (current_R_mod_d, frozen_residue_counts_tuple, collision_count) -> count
    #
    # residue_counts_tuple tracks how many times each residue mod d has been seen
    # among R(0), R(1), ..., R(pos).
    # collision_count = number of pairs with matching residue = sum C(c_r, 2)
    # We prune when collision_count >= 2.

    # R(0) = 0, so residue 0 starts with count 1.
    # R(j) = (R(j-1) + a_j * 10^(d-j)) mod d

    # For gcd(10, d) = 1, the 10^(d-j) factor is invertible.
    # For general d, we still compute R(j) directly.

    # Power table
    pow10 = [1] * (d + 1)
    for i in range(1, d + 1):
        pow10[i] = (pow10[i - 1] * 10) % d

    # R(j) = sum_{k=1}^{j} a_k * 10^{d-k} mod d
    # When we place digit a_j at position j, R(j) = R(j-1) + a_j * 10^{d-j} mod d

    # But we also need to count substrings NOT captured by the R-pair method.
    # The R-pair method works perfectly when gcd(10, d) = 1.
    # When gcd(10, d) > 1, some substrings divisible by d are missed.

    # General approach: directly count divisible substrings via DP.
    #
    # Alternative cleaner DP:
    # Track the partial remainders of ALL currently "open" substrings.
    # At position j, there are j open substrings (starting at positions 1..j).
    # substring(i, j) has value = substring(i, j-1) * 10 + a_j
    # So remainder(i, j) = (remainder(i, j-1) * 10 + a_j) mod d
    #
    # But tracking all j remainders simultaneously is expensive.

    # For the R-method (works for all d, not just coprime):
    # substring(i+1, j) = (P(j) - P(i) * 10^(j-i)) mod d where P(k) = number formed by first k digits mod d
    # This doesn't simplify as nicely when 10 is not invertible.

    # Practical approach for moderate d (up to ~19):
    # We use a DP that processes digits left to right.
    # State: tuple of residues of all "active" suffix values mod d.
    # At position j, we track (r_1, r_2, ..., r_j) where r_i = substring(i,j) mod d.
    # Plus how many have been zero so far.

    # This has state space d^j which is too large for d=19.

    # Better: use the collision-counting DP.
    # We directly count how many substrings are divisible by d.
    #
    # At each position j, we add digit a_j. This creates:
    #   - New substring of length 1: a_j. Check a_j mod d == 0.
    #   - Extends all previous substrings ending at j-1.
    #
    # We track the residues of substrings ending at the current position.
    # Let rem_i = substring(i, current_pos) mod d for each start i.
    # When we add a new digit x: rem_i_new = (rem_i_old * 10 + x) mod d
    # And a new substring of length 1 with remainder x mod d.
    #
    # We need to count how many rem_i == 0 across all positions.
    # Total = sum over all j of (number of i such that substring(i,j) mod d == 0)

    # Compression: we don't need individual remainders, just their distribution mod d.
    # State: (position, histogram of current suffix remainders mod d, total_zero_count)
    # histogram: tuple of length d where hist[r] = number of active suffixes with remainder r
    # When adding digit x: new_hist[r] = hist[r'] where (r'*10 + x) mod d == r, plus 1 if x mod d == r
    # zero_hits = new_hist[0] (number of substrings ending here that are divisible by d)
    # new_total = total_zero_count + zero_hits

    # State space: position * (compositions of position into d bins) * total_zero_count
    # The histogram is a composition of (position) into d bins, which is C(pos+d-1, d-1).
    # This is still large but with pruning (total_zero_count <= 1, and we discard >=2) it's feasible for small d.

    # Actually we can prune more: we only care whether total_zero_count is 0, 1, or >=2.
    # And the histogram is quite constrained: at position j, it sums to j.
    # For d >= j, most bins are 0 or 1.

    # Let's implement this approach with dictionary-based DP.

    # State: (tuple of histogram mod d, zero_count_so_far) where zero_count_so_far in {0, 1, 2}
    # zero_count_so_far = 2 means "2 or more", and we can stop tracking (will not be one-child)
    # Actually we still need to track it to EXCLUDE it. So we track 0 and 1, prune >=2.

    # Initial state (before placing any digit): histogram is all zeros, zero_count = 0
    # At position 0 (no digits placed yet): no active suffixes. hist = (0,0,...,0)

    # Optimization: represent histogram as a tuple, use dict for DP.

    if d > 12:
        # For large d, use the R-pair method which is more efficient
        return _count_one_child_R_method(d)

    # histogram approach for d <= 12
    initial_hist = tuple([0] * d)
    # dp: {(hist_tuple, zero_count): number_of_ways}
    dp = defaultdict(int)
    dp[(initial_hist, 0)] = 1

    for pos in range(d):
        new_dp = defaultdict(int)
        digit_range = range(1, 10) if pos == 0 else range(0, 10)

        for (hist, zc), ways in dp.items():
            if zc >= 2:
                continue  # prune
            for x in digit_range:
                # Update histogram: each old suffix remainder r maps to (r*10 + x) % d
                new_hist_list = [0] * d
                for r in range(d):
                    if hist[r] > 0:
                        new_r = (r * 10 + x) % d
                        new_hist_list[new_r] += hist[r]
                # Add new substring of length 1 (just digit x)
                new_hist_list[x % d] += 1

                # Count zero hits
                zero_hits = new_hist_list[0]
                new_zc = zc + zero_hits
                if new_zc >= 2:
                    new_zc = 2  # cap at 2 (means "too many")
                    continue  # prune: can never become one-child

                new_hist = tuple(new_hist_list)
                new_dp[(new_hist, new_zc)] += ways

        dp = new_dp

    # Count states with exactly 1 collision
    result = 0
    for (hist, zc), ways in dp.items():
        if zc == 1:
            result += ways

    return result

def _count_one_child_R_method(d):
    """
    Count d-digit one-child numbers using R-pair collision counting.

    This method works most cleanly when gcd(10, d) = 1 but we extend it.

    For gcd(10, d) = 1:
    R(j) = P(j) * 10^(d-j) mod d where P(j) = first-j-digits number mod d.
    substring(i+1..j) divisible by d iff R(i) == R(j).

    State: (position j, current R(j) mod d,
            number_of_R_values_seen_before_that_equal_current: collision tracking)

    We track the "signature" of how many R-values have been seen in each residue class.
    Since d can be large, we compress: track (singles, doubles) count.

    Actually we need to know the current R value AND whether it's been seen before.
    So we need: for each residue class, how many times it appeared.

    More efficient: track the collision count only.
    When we reach position j with R(j) = r, the number of new collisions is
    (number of times r appeared in R(0), ..., R(j-1)).

    State: (position, R_mod_d, collision_count, residue_frequency_vector)

    The frequency vector is too large. Instead, use:
    State: (position, R_mod_d, collision_count, number_of_residues_with_freq_1,
            number_of_residues_with_freq_2_or_more)

    But this loses info about WHICH residue has what frequency, which we need
    to know whether the next R value causes a collision.

    Alternative for large d: since we process digits left-to-right and d+1 values
    are distributed among d residue classes (pigeonhole), for d >= 19 most classes
    have 0 or 1 entries.

    For d up to 19, we can use a bitmask of "seen residues" since d <= 19 means
    at most 20 R-values distributed among d classes. With collision_count capped at 2,
    and a bitmask of size d bits for "seen" residues, the state is:
    (position, R_mod_d, collision_count, seen_bitmask)

    For d=19: bitmask is 2^19 = 524288, times d=19 residues, times 2 collision states = ~20M states.
    This is feasible.
    """

    # For general d (possibly not coprime to 10), we use a different substring counting.
    # The R-pair method only works cleanly for gcd(10,d)=1.
    # For general d, fall back to a suffix-remainder DP but with better state compression.

    # Actually, for d=13..19, let's use the suffix-remainder approach with aggressive pruning.
    # The histogram can be represented more compactly.

    # For d in 13..19, the histogram has d bins summing to at most d (the position count).
    # With pruning at collision_count >= 2, most states are eliminated.

    # Let's use a different representation: since most histogram entries are 0,
    # store only the non-zero entries as a frozenset of (residue, count) pairs.

    dp = defaultdict(int)
    # Initial: no digits placed, no active suffixes
    empty_hist = frozenset()
    dp[(empty_hist, 0)] = 1

    for pos in range(d):
        new_dp = defaultdict(int)
        digit_range = range(1, 10) if pos == 0 else range(0, 10)

        for (hist_fs, zc), ways in dp.items():
            if zc >= 2:
                continue

            # Convert frozenset to dict for manipulation
            hist = dict(hist_fs)

            for x in digit_range:
                new_hist = {}
                # Transform existing suffix remainders
                for r, cnt in hist.items():
                    new_r = (r * 10 + x) % d
                    new_hist[new_r] = new_hist.get(new_r, 0) + cnt
                # Add new length-1 substring
                xmod = x % d
                new_hist[xmod] = new_hist.get(xmod, 0) + 1

                # Count zeros
                zero_hits = new_hist.get(0, 0)
                new_zc = zc + zero_hits
                if new_zc >= 2:
                    continue  # prune

                new_hist_fs = frozenset(new_hist.items())
                new_dp[(new_hist_fs, new_zc)] += ways

        dp = new_dp

    result = 0
    for (hist_fs, zc), ways in dp.items():
        if zc == 1:
            result += ways

    return result

def F(N):
    """Compute F(N) = count of one-child numbers less than N."""
    total = 0
    s = str(N)
    max_d = len(s)

    # Count all d-digit one-child numbers for d < max_d
    for d in range(1, max_d):
        count = count_one_child_for_d(d)
        total += count
        print(f"  d={d}: {count} one-child numbers (running total: {total})")

    # For d = max_d, if N = 10^k then all max_d-digit numbers are included
    # since they are all < 10^k (max_d = k, and d-digit numbers go up to 10^k - 1)
    # Actually 10^k has k+1 digits, so max_d = k+1 if N = 10^k...
    # Wait: str(10^19) = "10000000000000000000" which has 20 digits.
    # So for N = 10^19, max_d = 20, and we count all d-digit numbers for d=1..19.

    if N == 10 ** (max_d - 1):
        # N is exactly a power of 10, so all (max_d-1)-digit numbers are < N
        # and we've already counted them. No partial d = max_d needed.
        pass
    else:
        # Need to count max_d-digit one-child numbers less than N
        # This requires a bounded digit DP (not implemented in this simplified version)
        count = count_one_child_for_d(max_d)
        total += count
        print(f"  d={max_d}: {count} one-child numbers (running total: {total})")

    return total

def verify():
    """Verify against known small values."""
    print("Verifying with brute force...")
    bf10 = solve_small_brute(10)
    print(f"  F(10) brute force = {bf10} (expected 9)")

    bf1000 = solve_small_brute(1000)
    print(f"  F(10^3) brute force = {bf1000} (expected 389)")

    print("\nVerifying with DP...")
    dp10 = F(10)
    print(f"  F(10) DP = {dp10} (expected 9)")

    dp1000 = F(1000)
    print(f"  F(10^3) DP = {dp1000} (expected 389)")

def create_visualization():
    """Create visualization of one-child number distribution and save as PNG."""
    print("\nGenerating visualization...")

    # Compute F(10^d) for small d using brute force
    powers = list(range(1, 8))
    f_values_bf = []

    # Use brute force for small values
    for d in range(1, 6):
        val = solve_small_brute(10 ** d)
        f_values_bf.append(val)
        print(f"  F(10^{d}) = {val}")

    # Known values
    known = {1: 9, 3: 389, 7: 277674}
