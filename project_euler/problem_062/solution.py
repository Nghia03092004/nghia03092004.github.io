"""
Problem 62: Cubic Permutations

Find the smallest cube for which exactly five permutations of its digits are also cubes.
"""

from collections import defaultdict


def solve():
    groups = defaultdict(list)
    prev_digits = 0

    for n in range(1, 100000):
        cube = n ** 3
        s = str(cube)
        d = len(s)

        if d > prev_digits and prev_digits > 0:
            # Finalize all groups with prev_digits digits
            candidates = []
            for key, cubes in groups.items():
                if len(key) == prev_digits and len(cubes) == 5:
                    candidates.append(min(cubes))
            if candidates:
                return min(candidates)
            prev_digits = d
        if prev_digits == 0:
            prev_digits = d

        key = ''.join(sorted(s))
        groups[key].append(cube)

    return None


answer = solve()
assert answer == 127035954683
print(answer)
