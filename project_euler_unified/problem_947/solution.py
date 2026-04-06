"""
Problem 947: Nim with Restricted Moves

Count winning positions for the first player in a combinatorial game
where the allowed moves are to remove {1, 2, 4} stones from a heap.

A position n is a P-position (previous player wins, i.e. current player
loses) if its Grundy value G(n) = 0. Otherwise it is an N-position (win).

The Sprague-Grundy sequence for moves {1,2,4} is periodic. By finding
the period and counting N-positions within one period, we can extrapolate
to count winning positions up to 10^9.

Key results:
  - Grundy sequence is periodic with period 7 after initial transient
  - P-positions in one period: positions where G(n)=0
  - Winning count W(N) = N - (number of P-positions in 1..N)

Methods:
  1. Sprague-Grundy computation via mex (minimum excludant)
  2. Period detection in Grundy sequence
  3. Exact counting using periodicity
  4. Verification via brute-force for small N
"""

def compute_grundy(max_n, moves):
    """Compute Grundy values G(0), G(1), ..., G(max_n)."""
    G = [0] * (max_n + 1)
    for n in range(1, max_n + 1):
        reachable = set()
        for m in moves:
            if n >= m:
                reachable.add(G[n - m])
        mex = 0
        while mex in reachable:
            mex += 1
        G[n] = mex
    return G

def find_period(G, min_start=5, max_period=100):
    """Find the period of the Grundy sequence after initial transient."""
    n = len(G)
    for period in range(1, max_period + 1):
        # Check if sequence is periodic from min_start
        valid = True
        for i in range(min_start, min(n - period, min_start + period * 5)):
            if G[i] != G[i + period]:
                valid = False
                break
        if valid:
            # Find the actual start of periodicity
            start = 0
            for s in range(min_start, -1, -1):
                if s + period < n and G[s] == G[s + period]:
                    start = s
                else:
                    start = s + 1
                    break
            return period, start
    return None, None

def count_winning(N, G, period, period_start):
    """Count N-positions (winning) in range [1, N] using periodicity.
    Uses the fact that G[n] has period 'period' for n >= period_start."""
    def grundy(n):
        """Get Grundy value for any n using periodicity."""
        if n < len(G):
            return G[n]
        if n < period_start:
            return G[n]
        return G[period_start + (n - period_start) % period]

    # Count P-positions in one full period
    p_in_period = sum(1 for i in range(period) if grundy(period_start + i) == 0)

    p_total = 0
    # Handle [1, max(period_start, period)-1] directly from precomputed G
    direct_end = max(period_start, period)
    for i in range(1, min(direct_end, N + 1)):
        if grundy(i) == 0:
            p_total += 1
    if N < direct_end:
        return N - p_total

    # Handle [direct_end, N] using periodicity
    # Align to period boundaries
    count_remaining = N - direct_end + 1
    # offset within period for direct_end
    offset = (direct_end - period_start) % period
    # Count P-positions from offset within period
    # First, fill partial period to next boundary
    if offset != 0:
        partial = min(period - offset, count_remaining)
        p_total += sum(1 for i in range(partial) if grundy(period_start + offset + i) == 0)
        count_remaining -= partial
    # Full periods
    full = count_remaining // period
    p_total += full * p_in_period
    leftover = count_remaining % period
    p_total += sum(1 for i in range(leftover) if grundy(period_start + i) == 0)

    return N - p_total

def count_winning_brute(N, G):
    """Direct count of N-positions in [1, N]."""
    return sum(1 for i in range(1, N + 1) if G[i] != 0)

# Compute
moves = [1, 2, 4]
G = compute_grundy(200, moves)

period, period_start = find_period(G)
print(f"Grundy values (0-30): {G[:31]}")
print(f"Period: {period}, starts at: {period_start}")

# Verification with assertions
# G(0) = 0 (no moves available, losing)
assert G[0] == 0
# G(1) = mex({G(0)}) = mex({0}) = 1
assert G[1] == 1
# G(2) = mex({G(1), G(0)}) = mex({1, 0}) = 2
assert G[2] == 2
# G(3) = mex({G(2), G(1)}) = mex({2, 1}) = 0 (can't remove 4)
assert G[3] == 0

# Verify periodicity
for i in range(period_start, 150):
    assert G[i] == G[i + period], f"Period broken at {i}"

# Verify brute-force matches formula for small N
for N_test in [20, 50, 100, 150]:
    brute = count_winning_brute(N_test, G)
    formula = count_winning(N_test, G, period, period_start)
    assert brute == formula, f"Mismatch at N={N_test}: {brute} vs {formula}"

# Compute answer for large N
N = 10 ** 9
answer = count_winning(N, G, period, period_start)
p_positions_in_period = [i for i in range(period) if G[period_start + i] == 0]
print(f"P-positions in one period (offset from {period_start}): {p_positions_in_period}")
print(f"P-positions per period: {len(p_positions_in_period)}/{period}")
print(f"W(10^9) = {answer}")
print(answer)
