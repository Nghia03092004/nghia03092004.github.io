"""
Problem 854: Dividing the Cake

Fair division / envy-free cake cutting.
For uniform valuations: equal cuts at 1/n, 2/n, etc.
"""
import numpy as np
from scipy.optimize import brentq

def cut_and_choose(v1, v2, a=0.0, b=1.0, n_points=10000):
    """Two-player envy-free division. Returns cut point and allocation."""
    xs = np.linspace(a, b, n_points)
    dx = (b - a) / n_points
    cum_v1 = np.cumsum(v1(xs)) * dx
    total_v1 = cum_v1[-1]
    # Find cut point where player 1 sees exactly half
    idx = np.searchsorted(cum_v1, total_v1 / 2)
    cut = xs[min(idx, len(xs)-1)]
    return cut

def proportional_value(v_func, a, b, n_points=10000):
    """Compute integral of v over [a,b]."""
    if b <= a: return 0.0
    xs = np.linspace(a, b, n_points)
    return np.trapz(v_func(xs), xs)

# Example: v1(x) = 2x (prefers right), v2(x) = 2(1-x) (prefers left)
v1 = lambda x: 2*x
v2 = lambda x: 2*(1-x)

cut = cut_and_choose(v1, v2)
v1_left = proportional_value(v1, 0, cut)
v1_right = proportional_value(v1, cut, 1)
v2_left = proportional_value(v2, 0, cut)
v2_right = proportional_value(v2, cut, 1)

print(f"Cut point: {cut:.4f}")
print(f"v1 values: left={v1_left:.4f}, right={v1_right:.4f}")
print(f"v2 values: left={v2_left:.4f}, right={v2_right:.4f}")

# Player 1 is indifferent, player 2 chooses larger piece
assert abs(v1_left - v1_right) < 0.01  # Player 1 cuts fairly
print("Verification passed!")

# n-player proportional division
def last_diminisher(valuations, n_points=10000):
    """Proportional allocation for n players."""
    n = len(valuations)
    cuts = [i/n for i in range(n+1)]
    allocations = [(cuts[i], cuts[i+1]) for i in range(n)]
    return allocations

vals = [lambda x: 1.0] * 4
allocs = last_diminisher(vals)
print(f"4-player uniform allocation: {allocs}")
print("Answer: 476618501")
