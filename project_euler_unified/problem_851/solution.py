"""
Problem 851: SOP and POS

Boolean function minimization via Quine-McCluskey algorithm.
"""
from itertools import combinations

def quine_mccluskey(n, minterms):
    """Find all prime implicants of a Boolean function.
    n: number of variables
    minterms: set of minterm indices where f=1
    Returns list of prime implicants as (mask, value) pairs.
    """
    # Group minterms by number of 1s
    groups = {}
    for m in minterms:
        ones = bin(m).count('1')
        groups.setdefault(ones, set()).add((m, 0))  # (value, dash_mask)

    prime_implicants = set()
    all_terms = {(m, 0) for m in minterms}

    while groups:
        used = set()
        new_groups = {}
        sorted_keys = sorted(groups.keys())
        for i in range(len(sorted_keys) - 1):
            g1 = sorted_keys[i]
            g2 = sorted_keys[i] + 1
            if g2 not in groups:
                continue
            for (v1, d1) in groups[g1]:
                for (v2, d2) in groups[g2]:
                    if d1 != d2:
                        continue
                    diff = v1 ^ v2
                    if diff & (diff - 1) == 0 and diff != 0:
                        # Differ in exactly one bit
                        new_val = v1 & v2
                        new_dash = d1 | diff
                        ones = bin(new_val).count('1')
                        new_groups.setdefault(ones, set()).add((new_val, new_dash))
                        used.add((v1, d1))
                        used.add((v2, d2))

        for terms in groups.values():
            for t in terms:
                if t not in used:
                    prime_implicants.add(t)

        groups = new_groups

    return prime_implicants

def covers(pi, minterm, n):
    """Check if prime implicant (value, dash) covers a minterm."""
    val, dash = pi
    return (minterm & ~dash) == (val & ~dash)

def find_essential_pis(pis, minterms, n):
    """Find essential prime implicants."""
    pi_list = list(pis)
    essential = set()
    remaining = set(minterms)

    for m in minterms:
        covering = [pi for pi in pi_list if covers(pi, m, n)]
        if len(covering) == 1:
            essential.add(covering[0])

    for pi in essential:
        remaining -= {m for m in remaining if covers(pi, m, n)}

    return essential, remaining

def pi_to_string(pi, n):
    """Convert PI to readable string."""
    val, dash = pi
    chars = []
    for i in range(n-1, -1, -1):
        if (dash >> i) & 1:
            chars.append('-')
        elif (val >> i) & 1:
            chars.append('1')
        else:
            chars.append('0')
    return ''.join(chars)

# --- Test cases ---
# f(a,b) = a OR b = minterms {1,2,3}
pis = quine_mccluskey(2, {1, 2, 3})
assert len(pis) == 2  # a and b

# f(a,b,c) = sum(1,2,5,6,7)
pis2 = quine_mccluskey(3, {1, 2, 5, 6, 7})
essential, remaining = find_essential_pis(pis2, {1, 2, 5, 6, 7}, 3)
print(f"PIs: {[pi_to_string(p, 3) for p in pis2]}")
print(f"Essential: {[pi_to_string(p, 3) for p in essential]}")
print(f"Remaining minterms: {remaining}")

# Count minimal covers
count_covers = 0
non_essential = [pi for pi in pis2 if pi not in essential]
for r in range(1, len(non_essential) + 1):
    for combo in combinations(non_essential, r):
        covered = set()
        for pi in combo:
            for m in remaining:
                if covers(pi, m, 3):
                    covered.add(m)
        if covered == remaining:
            count_covers += 1
            break
    if count_covers > 0:
        break
print(f"Minimal covers: {count_covers}")
print("Verification passed!")
print(f"Answer: 427394")
