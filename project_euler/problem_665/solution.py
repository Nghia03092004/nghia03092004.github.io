"""
Problem 665: Proportionate Nim

Sprague-Grundy analysis for Nim variant with proportional moves.
From pile n, remove floor(n/k) stones for allowed k values.
"""

def compute_grundy(N, allowed_ks=None):
    """Compute Grundy values for pile sizes 0..N.

    Default move: from pile n, can remove floor(n/k) for k=2,3,...
    (leaving n - floor(n/k) stones).
    """
    grundy = [0] * (N + 1)

    for n in range(1, N + 1):
        reachable = set()
        # Can remove floor(n/k) stones for k >= 2
        seen_removals = set()
        for k in range(2, n + 1):
            removal = n // k
            if removal == 0:
                break
            if removal not in seen_removals:
                seen_removals.add(removal)
                new_size = n - removal
                reachable.add(grundy[new_size])
        # Also can remove all (k=1)
        reachable.add(grundy[0])

        # Compute mex
        mex = 0
        while mex in reachable:
            mex += 1
        grundy[n] = mex

    return grundy

# Compute Grundy values
N = 500
grundy = compute_grundy(N)

print("Problem 665: Proportionate Nim")
print(f"Grundy values for n=0..20: {grundy[:21]}")

# Find P-positions (Grundy = 0)
p_positions = [n for n in range(N+1) if grundy[n] == 0]
print(f"P-positions up to {N}: {p_positions[:20]}...")
print(f"Number of P-positions: {len(p_positions)}")

# Detect periodicity
def find_period(seq, min_period=1, max_period=200):
    """Detect period in sequence."""
    n = len(seq)
    for p in range(min_period, max_period + 1):
        is_periodic = True
        for i in range(p, min(n, 3 * p)):
            if seq[i] != seq[i - p]:
                is_periodic = False
                break
        if is_periodic:
            return p
    return None

period = find_period(grundy[50:])  # skip initial transient
if period:
    print(f"Detected period: {period}")
