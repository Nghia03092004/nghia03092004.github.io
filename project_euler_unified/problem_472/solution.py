#!/usr/bin/env python3
"""
Project Euler Problem 472: Comfortable Distance II

N seats in a row. People arrive one at a time.
Rule 1: No person sits beside another (adjacent seats forbidden).
Rule 2: First person chooses any seat.
Rule 3: Each subsequent person chooses the seat maximizing minimum distance
         to any occupied seat (leftmost if tie), subject to rule 1.

f(N) = number of first-seat choices that maximize the number of occupants.

Find sum f(N) for 1 <= N <= 10^12, last 8 digits.
"""

from functools import lru_cache

def simulate_seating(N, first_pos):
    """
    Simulate the greedy seating process for N seats with first person at first_pos.
    Returns the total number of people seated.

    Seats are 1-indexed: 1..N.
    Rule: each new person sits at the position with maximum min-distance to any
    occupied seat, not adjacent to any occupied seat. Ties: leftmost.
    """
    if N == 0:
        return 0
    if N == 1:
        return 1

    occupied = set()
    occupied.add(first_pos)

    while True:
        best_pos = -1
        best_dist = -1

        for pos in range(1, N + 1):
            if pos in occupied:
                continue
            # Check adjacency
            if (pos - 1) in occupied or (pos + 1) in occupied:
                continue
            # Compute min distance to any occupied seat
            min_dist = min(abs(pos - o) for o in occupied)
            if min_dist > best_dist:
                best_dist = min_dist
                best_pos = pos
            elif min_dist == best_dist and pos < best_pos:
                best_pos = pos

        if best_pos == -1:
            break
        occupied.add(best_pos)

    return len(occupied)

def compute_f(N):
    """Compute f(N): number of first-seat choices maximizing occupancy."""
    if N == 0:
        return 0

    max_occ = 0
    counts = {}

    for p in range(1, N + 1):
        occ = simulate_seating(N, p)
        if occ > max_occ:
            max_occ = occ
        counts[p] = occ

    f_val = sum(1 for p in counts if counts[p] == max_occ)
    return f_val

def compute_f_detailed(N):
    """Compute f(N) with details about each starting position."""
    if N == 0:
        return 0, {}

    results = {}
    max_occ = 0

    for p in range(1, N + 1):
        occ = simulate_seating(N, p)
        results[p] = occ
        if occ > max_occ:
            max_occ = occ

    f_val = sum(1 for p in results if results[p] == max_occ)
    return f_val, results

def verify_small():
    """Verify against known values."""
    print("=== Verification ===")
    print(f"f(1) = {compute_f(1)} (expected 1)")
    print(f"f(15) = {compute_f(15)} (expected 9)")
    print(f"f(20) = {compute_f(20)} (expected 6)")

    # Compute sum for N=1..20
    total = 0
    f_values = []
    for N in range(1, 21):
        fN = compute_f(N)
        f_values.append(fN)
        total += fN
        print(f"  f({N}) = {fN}")
    print(f"Sum f(N) for N=1..20 = {total} (expected 83)")

    return f_values

def compute_sum_500():
    """Compute sum f(N) for N=1..500."""
    print("\n=== Computing sum for N=1..500 ===")
    total = 0
    for N in range(1, 501):
        fN = compute_f(N)
        total += fN
        if N % 50 == 0:
            print(f"  Sum up to N={N}: {total}")
    print(f"Sum f(N) for N=1..500 = {total} (expected 13343)")
    return total

def analyze_pattern():
    """Analyze the pattern of f(N) to find recurrence."""
    print("\n=== Pattern Analysis ===")
    f_values = []
    for N in range(1, 101):
        fN = compute_f(N)
        f_values.append(fN)

    # Print f values
    for i in range(0, len(f_values), 10):
        chunk = f_values[i:i+10]
        ns = list(range(i+1, i+11))
        print(f"N={ns[0]:3d}-{ns[-1]:3d}: {chunk}")

    # Look for differences
    diffs = [f_values[i+1] - f_values[i] for i in range(len(f_values)-1)]
    print(f"\nFirst differences: {diffs[:30]}")

    # Look for pattern based on binary representation
    print("\nPattern by N mod various:")
    for mod in [2, 3, 4, 6, 8]:
        print(f"  mod {mod}:")
        for r in range(mod):
            vals = [f_values[i] for i in range(r, len(f_values), mod)]
            print(f"    r={r}: {vals[:15]}")

    # Check if f(N) relates to binary structure of N
    print("\nBinary structure analysis:")
    for N in range(1, 31):
        print(f"  N={N:3d} ({N:07b}): f(N)={f_values[N-1]}")

    return f_values

def find_recurrence(f_values):
    """Try to find a recurrence for f(N)."""
    print("\n=== Recurrence Search ===")
    n = len(f_values)

    # Check: f(2N) = ? and f(2N+1) = ?
    print("f(2N) pattern:")
    for N in range(1, min(20, n//2)):
        print(f"  f({2*N}) = {f_values[2*N-1]}, f({N}) = {f_values[N-1]}")

    print("f(2N+1) pattern:")
    for N in range(1, min(20, (n-1)//2)):
        print(f"  f({2*N+1}) = {f_values[2*N]}, f({N}) = {f_values[N-1]}")

    # Check: f(N) related to f(ceil(N/2)) or similar
    print("\nRelation to half-values:")
    for N in range(1, min(40, n)):
        h = (N + 1) // 2
        if h <= n:
            print(f"  N={N:3d}: f(N)={f_values[N-1]}, f(ceil(N/2))={f_values[h-1]}, "
                  f"diff={f_values[N-1]-f_values[h-1]}")

def sum_f_large(limit, mod=10**8):
    """
    Compute sum f(N) for N=1..limit, modulo mod.

    For N up to 10^12, we need to find a pattern or recurrence.
    After analyzing small values, implement the efficient computation.

    This is a placeholder that works for small limits.
    For the full 10^12 solution, the recurrence must be identified first.
    """
    if limit <= 1000:
        total = 0
        for N in range(1, limit + 1):
            total += compute_f(N)
        return total % mod
    else:
        print("Large limit computation not yet implemented.")
        print("Need to identify recurrence from pattern analysis first.")
        return None

def create_visualization():
    """Create visualization for Problem 472."""
