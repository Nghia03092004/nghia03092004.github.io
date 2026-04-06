"""Project Euler Problem 5: Smallest Multiple"""

import math
from functools import reduce

def solve(n: int = 20) -> int:
    """Compute lcm(1, 2, ..., n) via iterative GCD-LCM identity."""
    return reduce(lambda a, b: a * b // math.gcd(a, b), range(1, n + 1))

answer = solve()
assert answer == 232792560
print(answer)
