"""
Problem 624: Two Heads Are Better Than One

Expected number of fair coin flips to get n consecutive heads: E(n) = 2^{n+1} - 2.

Markov chain with states S_0, S_1, ..., S_n (S_k = run of k heads).
Transition: S_k -> S_{k+1} with p=1/2, S_k -> S_0 with p=1/2.
Recurrence: e_k = 1 + (1/2)*e_{k+1} + (1/2)*e_0, solution: e_0 = 2^{n+1} - 2.

Method 1: Closed-form formula (primary)
Method 2: Markov chain back-substitution (verification)
Method 3: Monte Carlo simulation (cross-check)
"""

import random

MOD = 10**9 + 7

# --- Method 1: Closed form ---
def expected_closed(n: int):
    """E(n) = 2^{n+1} - 2 (exact integer)."""
    return 2**(n + 1) - 2

def expected_closed_mod(n: int, mod: int):
    """E(n) = 2^{n+1} - 2 mod p."""
    return (pow(2, n + 1, mod) - 2) % mod

# --- Method 2: Markov chain back-substitution ---
def expected_markov(n: int) -> float:
    """Solve the Markov chain system exactly via back-substitution.

    e_k = 2^{n+1} - 2^{k+1}, verified by substitution.
    Here we solve numerically for comparison.
    """
    # e_k = 1 + 0.5*e_{k+1} + 0.5*e_0
    # Let f_k = e_k - e_0. Then f_k = 1 + 0.5*f_{k+1}, f_n = -e_0, f_0 = 0.
    # f_{n-1} = 1 - 0.5*e_0
    # f_{n-2} = 1 + 0.5*(1 - 0.5*e_0) = 1.5 - 0.25*e_0
    # f_k = 2*(1 - 2^{-(n-k)}) - 2^{-(n-k)}*e_0
    # f_0 = 0 => e_0 = 2*(2^n - 1)
    e = [0.0] * (n + 1)
    # Symbolic: e_k = A_k + B_k * e_0
    # e_n = 0: A_n = 0, B_n = 0
    # e_k = 1 + 0.5*(A_{k+1} + B_{k+1}*e_0) + 0.5*e_0
    #      = (1 + 0.5*A_{k+1}) + (0.5*B_{k+1} + 0.5)*e_0
    A = [0.0] * (n + 1)
    B = [0.0] * (n + 1)
    for k in range(n - 1, -1, -1):
        A[k] = 1 + 0.5 * A[k + 1]
        B[k] = 0.5 * B[k + 1] + 0.5
    # e_0 = A_0 + B_0*e_0 => e_0*(1 - B_0) = A_0
    e0 = A[0] / (1 - B[0])
    return e0

# --- Method 3: Monte Carlo simulation ---
def expected_simulation(n: int, trials: int = 100000) -> float:
    """Estimate E(n) via Monte Carlo simulation."""
    total = 0
    for _ in range(trials):
        run, flips = 0, 0
        while run < n:
            flips += 1
            if random.random() < 0.5:
                run += 1
            else:
                run = 0
        total += flips
    return total / trials

# Compute and verify

# Verify closed form against Markov chain
for n in range(1, 25):
    exact = expected_closed(n)
    markov = expected_markov(n)
    assert abs(exact - markov) < 1e-6, f"n={n}: exact={exact}, markov={markov}"

# Verify recurrence E_n = 2*E_{n-1} + 2
for n in range(2, 30):
    assert expected_closed(n) == 2 * expected_closed(n - 1) + 2

# Verify modular computation
for n in range(1, 20):
    assert expected_closed_mod(n, MOD) == expected_closed(n) % MOD

# Verify against simulation for small n
random.seed(42)
for n in range(1, 6):
    sim = expected_simulation(n, trials=200000)
    exact = expected_closed(n)
    rel_error = abs(sim - exact) / exact
    assert rel_error < 0.05, f"n={n}: sim={sim:.1f}, exact={exact}, error={rel_error:.3f}"

print("All verifications passed.")

# Print table
print(f"\n{'n':>4} {'E(n)':>12} {'2^(n+1)-2':>12} {'Simulation':>12}")
for n in range(1, 11):
    random.seed(n)
    sim = expected_simulation(n, trials=50000)
    print(f"{n:>4} {expected_closed(n):>12} {2**(n+1)-2:>12} {sim:>12.1f}")
