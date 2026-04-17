"""
Problem 488: Unbalanced Nim
A Nim variant where from heap h, you can remove 1..floor(h/2) stones.
Sprague-Grundy analysis to find winning/losing positions.
"""

def compute_grundy(max_h: int) -> list:
    """Compute Grundy values G(0), G(1), ..., G(max_h) for unbalanced Nim."""
    G = [0] * (max_h + 1)
    for h in range(2, max_h + 1):
        # From h, can move to positions ceil(h/2) .. h-1
        low = (h + 1) // 2  # ceil(h/2)
        high = h - 1
        reachable = set()
        for j in range(low, high + 1):
            reachable.add(G[j])
        # mex: smallest non-negative integer not in reachable
        mex = 0
        while mex in reachable:
            mex += 1
        G[h] = mex
    return G

def count_p_positions_two_heaps(max_h: int):
    """Count P-positions (losing for current player) with two heaps, each 0..max_h."""
    G = compute_grundy(max_h)
    count = 0
    for h1 in range(max_h + 1):
        for h2 in range(max_h + 1):
            if G[h1] ^ G[h2] == 0:
                count += 1
    return count

# Compute Grundy values
max_h = 100
G = compute_grundy(max_h)

print("Grundy values G(h) for h = 0..30:")
for h in range(31):
    print(f"  G({h:2d}) = {G[h]}")

# Count P-positions for two heaps up to 50
p_count = count_p_positions_two_heaps(50)
print(f"\nP-positions (two heaps, 0..50): {p_count}")
