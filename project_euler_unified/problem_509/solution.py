"""
Problem 509: Divisor Nim
A Nim variant where from pile n, you remove d stones where d | n and d < n.
Uses Sprague-Grundy theory to classify winning/losing positions.
"""

def compute_grundy(N: int) -> list:
    """
    Compute Grundy values for Divisor Nim for positions 0..N.
    g(n) = mex{g(n-d) : d | n, 1 <= d < n}
    """
    g = [0] * (N + 1)

    # Precompute proper divisors for each n using sieve
    # proper_divs[n] = {d : d | n, 1 <= d < n}
    proper_divs = [[] for _ in range(N + 1)]
    for d in range(1, N + 1):
        for multiple in range(2 * d, N + 1, d):
            proper_divs[multiple].append(d)

    for n in range(2, N + 1):
        reachable = set()
        for d in proper_divs[n]:
            reachable.add(g[n - d])
        # mex
        mex = 0
        while mex in reachable:
            mex += 1
        g[n] = mex

    return g

def count_winning(N: int):
    """Count winning positions (g(n) > 0) for n = 1..N."""
    g = compute_grundy(N)
    return sum(1 for n in range(1, N + 1) if g[n] > 0)

def solve_brute_force(N: int) -> list:
    """Classify positions as W(in) or L(ose) for verification."""
    # P-position: all moves lead to N-positions (g = 0)
    # N-position: some move leads to P-position (g > 0)
    is_losing = [True] * (N + 1)  # Start assuming all losing

    for n in range(2, N + 1):
        # Find proper divisors of n
        for d in range(1, n):
            if n % d == 0:
                if is_losing[n - d]:  # Can move to a losing position
                    is_losing[n] = False
                    break

    return is_losing

# Compute Grundy values
N = 500
g = compute_grundy(N)

# Display small values
print("Position: Grundy value (W/L)")
for n in range(1, 31):
    status = "W" if g[n] > 0 else "L"
    print(f"  n={n:3d}: g={g[n]:2d} [{status}]")

# Count winning positions
W = count_winning(N)
print(f"\nWinning positions for n=1..{N}: {W}")
print(f"Losing positions for n=1..{N}: {N - W}")

# Verify against brute force for small N
N_small = 100
g_small = compute_grundy(N_small)
is_losing_bf = solve_brute_force(N_small)
match = all((g_small[n] == 0) == is_losing_bf[n] for n in range(1, N_small + 1))
print(f"\nBrute-force verification (n=1..{N_small}): {'PASS' if match else 'FAIL'}")

# Identify pattern: which numbers are losing positions?
losing = [n for n in range(1, N + 1) if g[n] == 0]
print(f"\nFirst 30 losing positions: {losing[:30]}")
