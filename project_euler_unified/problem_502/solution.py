"""
Problem 502: Counting Castles
Count castle-shaped profiles on a grid using profile dynamic programming.
A castle profile is a sequence of column heights with battlement constraints.
"""

MOD = 10**9 + 7

def solve_simple_battlement(w: int, h: int):
    """Count profiles of width w, max height h, where no two adjacent columns
    have the same height. Each column height in [1, h].

    Result: h * (h-1)^(w-1)
    """
    if w == 0:
        return 0
    return h * pow(h - 1, w - 1, MOD) % MOD

def solve_dp(w: int, h: int):
    """Profile DP for battlement counting.
    dp[j] = number of valid profiles ending with height j.
    """
    if w == 0:
        return 0

    dp = [1] * (h + 1)  # dp[j] for j = 1..h, dp[0] unused
    dp[0] = 0

    for col in range(2, w + 1):
        total = sum(dp) % MOD
        new_dp = [0] * (h + 1)
        for j in range(1, h + 1):
            new_dp[j] = (total - dp[j]) % MOD
        dp = new_dp

    return sum(dp) % MOD

def solve_full_castle(w: int, h: int):
    """Full castle profile DP with additional constraints:
    - Column heights in [1, h]
    - No two adjacent columns have the same height (battlement)
    - At least one column reaches height h (touches the top)
    - The profile is connected (always true for contiguous columns from bottom)

    Count = (profiles with no same-adjacent) - (profiles with no same-adjacent and all < h)
    = h*(h-1)^(w-1) - (h-1)*(h-2)^(w-1)
    """
    if w == 0:
        return 0
    if h == 1:
        return 1 if w >= 1 else 0

    # Total with max height h: no adjacent same
    total = h * pow(h - 1, w - 1, MOD) % MOD
    # Those not reaching height h: heights in [1, h-1], no adjacent same
    without_top = (h - 1) * pow(h - 2, w - 1, MOD) % MOD if h >= 3 else (h - 1)
    if h == 2 and w > 1:
        without_top = 0  # only height 1, but w > 1 means all same -> 0 from battlement

    result = (total - without_top) % MOD
    return result

def solve_profile_dp_full(w: int, h: int):
    """Full profile DP tracking whether max height h has been reached.
    State: (last_height, reached_top)
    """
    if w == 0:
        return 0

    # dp[j][t] = count of profiles of current length ending at height j,
    # where t=1 means at least one column reached height h
    dp = {}
    for j in range(1, h + 1):
        t = 1 if j == h else 0
        dp[(j, t)] = 1

    for col in range(2, w + 1):
        new_dp = {}
        # Compute total for each 'reached_top' value
        total = [0, 0]
        for (j, t), cnt in dp.items():
            total[t] = (total[t] + cnt) % MOD

        for j2 in range(1, h + 1):
            for t2 in range(2):
                # t_new: if t2=1 (already reached) or j2=h
                t_new = 1 if (t2 == 1 or j2 == h) else 0
                # Sum over all j != j2 with reached_top = t2
                # = total[t2] - dp.get((j2, t2), 0)
                contrib = (total[t2] - dp.get((j2, t2), 0)) % MOD
                key = (j2, t_new)
                new_dp[key] = (new_dp.get(key, 0) + contrib) % MOD
        dp = new_dp

    # Sum all states with reached_top = 1
    result = 0
    for (j, t), cnt in dp.items():
        if t == 1:
            result = (result + cnt) % MOD
    return result

# Compute results
print("Counting Castles - Profile DP Results:")
print()

print("Simple battlement (no adjacent same heights):")
for w in range(1, 15):
    for h in [5, 10]:
        c = solve_simple_battlement(w, h)
        print(f"  C({w}, {h}) = {c}")

print()
print("Full castle (must reach top, no adjacent same):")
for w in range(1, 15):
    for h in [5, 10]:
        c = solve_profile_dp_full(w, h)
        print(f"  C({w}, {h}) = {c}")

print()
print("Specific case C(13, 10):")
print(f"  Simple: {solve_simple_battlement(13, 10)}")
print(f"  Must reach top: {solve_profile_dp_full(13, 10)}")
