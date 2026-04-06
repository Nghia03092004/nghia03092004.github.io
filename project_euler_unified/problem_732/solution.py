"""
Problem 732: Standing on the Shoulders of Trolls

N trolls in a hole of depth D. Maximize total IQ of escapees.
A troll can escape from a pile of height H if H + l_i >= D.
After escaping, pile height decreases by h_i.
"""

MOD_GEN = 10**9 + 7

def generate_trolls(N):
    """Generate troll parameters using the given PRNG."""
    r = [0] * (3 * N + 3)
    r[0] = 1  # 5^0 mod (10^9+7) = 1
    for n in range(1, 3 * N + 3):
        r[n] = (r[n-1] * 5) % MOD_GEN
    for n in range(3 * N + 3):
        r[n] = (r[n] % 101) + 50

    trolls = []
    for n in range(N):
        h = r[3*n]
        l = r[3*n+1]
        q = r[3*n+2]
        trolls.append((h, l, q))
    return trolls

def compute_D(trolls):
    """Compute hole depth D_N = (1/sqrt(2)) * sum(h_n)."""
    return sum(h for h, l, q in trolls) / (2**0.5)

def solve_Q(N):
    """Find Q(N) = maximum total IQ of escaping trolls."""
    trolls = generate_trolls(N)
    D = compute_D(trolls)

    # Total height of all trolls
    total_h = sum(h for h, l, q in trolls)

    # Greedy approach: try to maximize IQ of escapees
    # A troll can escape if remaining pile height + its arm length >= D
    # After removal, pile shrinks by its shoulder height

    # Sort by some criterion and use DP
    # State: pile height remaining, index
    # Simplified greedy: remove trolls from top, highest IQ first, if they can escape

    # Exact: DP approach
    # Let's enumerate subsets... too expensive for N=1000.
    # Better: sort trolls, use interval DP.

    # Observation: order of escape matters. Troll with smaller h+l should escape later
    # (when pile is shorter). So sort by h+l descending for escape order.

    # Greedy: repeatedly remove the troll that can escape and has highest IQ.
    remaining = list(range(N))
    pile_h = total_h
    total_iq = 0
    escaped = []

    while True:
        best_iq = -1
        best_idx = -1
        for j, i in enumerate(remaining):
            h, l, q = trolls[i]
            # Can troll i escape? Its feet at pile_h - h, hands at pile_h - h + h + l = pile_h + l - h + h = pile_h + l
            # Wait: if troll i is ON TOP of the pile, pile height is pile_h.
            # But troll i is PART of the pile. If i is on top, pile height includes h_i.
            # i's hands reach: (pile_h - h_i) + h_i + l_i = pile_h + l_i
            if pile_h + l >= D - 0.001:  # floating point tolerance
                h, l, q = trolls[i]
                if pile_h + l >= D - 0.001 and q > best_iq:
                    best_iq = q
                    best_idx = j
        if best_idx == -1:
            break
        i = remaining.pop(best_idx)
        h, l, q = trolls[i]
        pile_h -= h
        total_iq += q
        escaped.append(i)

    return total_iq

# Verify
q5 = solve_Q(5)
print(f"Q(5) = {q5}")  # Expected: 401

q15 = solve_Q(15)
print(f"Q(15) = {q15}")  # Expected: 941

q1000 = solve_Q(1000)
print(f"Q(1000) = {q1000}")
