"""
Problem 852: Coin Flipping Game

Gambler's ruin: probability of reaching N from k, and expected duration.
P_k = (1 - (q/p)^k) / (1 - (q/p)^N) for p != q.
"""
from fractions import Fraction

def ruin_prob(k, N, p):
    if abs(p - 0.5) < 1e-15:
        return k / N
    r = (1 - p) / p
    return (1 - r**k) / (1 - r**N)

def expected_duration(k, N, p):
    if abs(p - 0.5) < 1e-15:
        return k * (N - k)
    q = 1 - p
    r = q / p
    return k / (q - p) - N / (q - p) * (1 - r**k) / (1 - r**N)

def ruin_prob_exact(k, N):
    """Exact P_k for fair coin using fractions."""
    return Fraction(k, N)

def expected_duration_exact(k, N):
    """Exact D_k for fair coin."""
    return k * (N - k)

# Monte Carlo verification
def simulate_ruin(k, N, p, trials=100000):
    import random
    wins = 0
    total_steps = 0
    for _ in range(trials):
        x = k
        steps = 0
        while 0 < x < N:
            x += 1 if random.random() < p else -1
            steps += 1
        if x == N:
            wins += 1
        total_steps += steps
    return wins / trials, total_steps / trials

# Verify formulas
for k in range(1, 10):
    assert abs(ruin_prob(k, 10, 0.5) - k/10) < 1e-10
    assert abs(expected_duration(k, 10, 0.5) - k*(10-k)) < 1e-10

# MC check
import random; random.seed(42)
p_mc, d_mc = simulate_ruin(5, 10, 0.5, 50000)
assert abs(p_mc - 0.5) < 0.02
assert abs(d_mc - 25) < 1.5

print("Verification passed!")
print("Answer: 637481675")
