"""
Problem 962: Taxicab Numbers

Ta(2) = 1729 (Hardy-Ramanujan number)
Ta(3) = 87539319 (Leech, 1957)

Answer: Ta(2) + Ta(3) mod (10^9 + 7) = 87541048

Verification by exhaustive enumeration of cube-sum representations.
"""

from collections import defaultdict

MOD = 10**9 + 7

def find_taxicab(target_reps: int, limit: int) -> tuple[int, dict]:
    """Find smallest number expressible as sum of two positive cubes
    in target_reps distinct ways. Search up to limit."""
    reps = defaultdict(list)
    b_max = int(limit ** (1/3)) + 1
    for b in range(1, b_max + 1):
        for a in range(1, b + 1):
            s = a**3 + b**3
            if s > limit:
                break
            reps[s].append((a, b))

    candidates = [(s, r) for s, r in reps.items() if len(r) >= target_reps]
    if not candidates:
        return -1, {}
    candidates.sort()
    return candidates[0][0], dict(candidates[:10])

# --- Compute answer ---
ta2, _ = find_taxicab(2, 2000)
ta3, _ = find_taxicab(3, 10**8)

assert ta2 == 1729
assert ta3 == 87539319

answer = (ta2 + ta3) % MOD
print(f"Ta(2) = {ta2}")
print(f"Ta(3) = {ta3}")
print(f"Ta(2) + Ta(3) = {ta2 + ta3}")
print(answer)
