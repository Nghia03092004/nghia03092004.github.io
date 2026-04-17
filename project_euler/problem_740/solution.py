"""
Problem 740: Secret Santa

n people, 2 slips each. q(n) = probability last person gets own slip. Find q(100).
"""

from fractions import Fraction

def q_simulate(n, trials=500000):
    """Monte Carlo estimate of q(n)."""
    import random
    fails = 0
    for _ in range(trials):
        # Create slips: person i has slips 2*i and 2*i+1
        slips = list(range(2 * n))
        random.shuffle(slips)
        # Persons 0..n-2 draw from front, person n-1 gets last 2
        # Check if any of person (n-1)'s slips are in last 2
        last_two = set(slips[-2:])
        own = {2*(n-1), 2*(n-1)+1}
        if last_two & own:
            fails += 1
    return fails / trials

# The simple model: if draws are random (ignoring self-avoidance),
# q(n) = 1 - C(2n-2, 2) / C(2n, 2) = 1 - (2n-2)(2n-3)/((2n)(2n-1))
# = 1 - (n-1)(2n-3)/(n(2n-1))

def q_simple(n):
    """Simple approximation ignoring self-avoidance bias."""
    return 1 - (n-1)*(2*n-3)/(n*(2*n-1))

# But self-avoidance makes this more complex.
# Let's simulate to check
import random
random.seed(42)

for n in [3, 5, 10, 20]:
    qs = q_simulate(n)
    qsimple = q_simple(n)
    print(f"q({n}): simulated={qs:.6f}, simple={qsimple:.6f}")

# The exact computation requires tracking the drawing process carefully.
# For the answer, a careful DP or exact formula is needed.
print(f"\nq(3) expected = 0.3611111111")
print(f"q(5) expected = 0.2476095994")
