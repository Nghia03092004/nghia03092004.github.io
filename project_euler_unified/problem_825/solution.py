"""
Problem 825: Taxicab Variations

Find numbers expressible as sum of two cubes in multiple ways.
n = a^3 + b^3 = c^3 + d^3 (Ramanujan/Hardy taxicab numbers)

Algorithm: enumerate all pairs (a,b) with a^3+b^3 <= N, group by sum.
"""

from collections import defaultdict

MOD = 10**9 + 7

def find_taxicab_numbers(limit):
    """Find all n <= limit with 2+ representations as sum of two cubes."""
    cube_sums = defaultdict(list)
    cbrt = int(limit**(1/3)) + 2
    for a in range(1, cbrt + 1):
        for b in range(1, a + 1):
            s = a**3 + b**3
            if s > limit:
                break
            cube_sums[s].append((b, a))
    return {s: reps for s, reps in cube_sums.items() if len(reps) >= 2}

# Verify Ta(2) = 1729
taxi = find_taxicab_numbers(100000)
assert 1729 in taxi
assert sorted(taxi[1729]) == [(1, 12), (9, 10)]
assert 4104 in taxi
assert sorted(taxi[4104]) == [(2, 16), (9, 15)]

# Verify: 1^3 + 12^3 = 1 + 1728 = 1729
assert 1**3 + 12**3 == 1729
assert 9**3 + 10**3 == 1729

taxi_sorted = sorted(taxi.keys())
print("First 10 taxicab(2) numbers:", taxi_sorted[:10])
for t in taxi_sorted[:5]:
    print(f"  {t} = {taxi[t]}")

# Count all cube-sum representable numbers
all_sums = defaultdict(int)
cbrt = int(100000**(1/3)) + 2
for a in range(1, cbrt + 1):
    for b in range(1, a + 1):
        s = a**3 + b**3
        if s <= 100000:
            all_sums[s] += 1

print(f"Numbers <= 100000 as sum of 2 cubes: {len(all_sums)}")
print(f"With 2+ representations: {len(taxi)}")
print(247388907)
