"""
Project Euler Problem 394: Eating Pie

Jeff eats a circular pie by repeatedly choosing one of two exposed edges
and cutting at a random angle. E(n) = expected slices until remaining < 1/n.
Find ceil(E(10)) + ceil(E(100)) + ceil(E(1000)).

Key insight: each cut multiplies the remaining fraction by U ~ Uniform(0,1).
Equivalently, we need the sum of Exp(1) variables to exceed ln(n).
Exact result: E(n) = 1 + ln(n).
"""

import math
import random

def E_exact(n):
    """Exact expected number of slices: E(n) = 1 + ln(n)."""
    return 1.0 + math.log(n)

def E_simulation(n, trials=1_000_000, seed=42):
    """Monte Carlo estimate of E(n)."""
    rng = random.Random(seed)
    total_cuts = 0
    for _ in range(trials):
        remaining = 1.0
        cuts = 0
        threshold = 1.0 / n
        while remaining >= threshold:
            # Pick an edge (by symmetry, choice doesn't matter)
            # Cut at uniform random angle from that edge
            alpha = rng.uniform(0, remaining)
            remaining -= alpha
            cuts += 1
        total_cuts += cuts
    return total_cuts / trials

def E_via_gamma_sum(n, max_k=200):
    """Compute E(n) = sum_{k=0}^{inf} P(Gamma(k,1) <= ln(n)) using the Poisson identity."""
    from scipy.special import gammainc
    t = math.log(n)
    total = 0.0
    for k in range(max_k):
        if k == 0:
            prob = 1.0  # P(S_0 = 0 <= t) = 1 for t > 0
        else:
            prob = gammainc(k, t)  # regularized lower incomplete gamma
        if prob < 1e-18 and k > 5:
            break
        total += prob
    return total

# --- Analytical computation ---
ns = [10, 100, 1000]
exact_values = {n: E_exact(n) for n in ns}
gamma_values = {n: E_via_gamma_sum(n) for n in ns}

print("=" * 55)
print("Project Euler Problem 394: Eating Pie")
print("=" * 55)
print(f"\nExact formula: E(n) = 1 + ln(n)\n")
print(f"{'n':>6} | {'E(n) exact':>14} | {'E(n) gamma sum':>14} | {'ceil':>5}")
print("-" * 50)
for n in ns:
    print(f"{n:>6} | {exact_values[n]:>14.6f} | {gamma_values[n]:>14.6f} | {math.ceil(exact_values[n]):>5}")

answer = sum(math.ceil(exact_values[n]) for n in ns)
print(f"\nAnswer: {' + '.join(str(math.ceil(exact_values[n])) for n in ns)} = {answer}")

# --- Monte Carlo verification ---
print("\nMonte Carlo verification (1,000,000 trials each):")
print(f"{'n':>6} | {'E(n) simulated':>14} | {'E(n) exact':>14} | {'error':>10}")
print("-" * 55)
for n in ns:
    sim = E_simulation(n)
    err = abs(sim - exact_values[n])
    print(f"{n:>6} | {sim:>14.6f} | {exact_values[n]:>14.6f} | {err:>10.6f}")

# --- Assertions ---
assert answer == 18, f"Expected 18, got {answer}"
for n in ns:
    assert abs(exact_values[n] - gamma_values[n]) < 1e-8, "Gamma sum doesn't match exact formula"
    sim = E_simulation(n, trials=500_000, seed=123)
    assert abs(sim - exact_values[n]) < 0.05, f"Simulation for n={n} too far from exact"

print(f"\nAll assertions passed. Final answer: {answer}")
