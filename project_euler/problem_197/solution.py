"""
Problem 197: Recursively Defined Sequence
f(x) = floor(2^(30.403243784 - x^2)) * 1e-9
u_0 = -1, u_{n+1} = f(u_n)
Converges to 2-cycle. Find floor((u_n + u_{n+1}) * 10^9).
"""
import math, matplotlib

def f(x):
    return math.floor(2 ** (30.403243784 - x * x)) * 1e-9

def solve(N=1000):
    u = -1.0
    for _ in range(N):
        u = f(u)
    u_n = u
    u_n1 = f(u)
    return math.floor((u_n + u_n1) * 1e9)

def solve_brute(N=100):
    """Same algorithm, just verify convergence at different iteration counts."""
    results = []
    u = -1.0
    for i in range(N):
        u = f(u)
        u_next = f(u)
        results.append(math.floor((u + u_next) * 1e9))
    return results

# Verify convergence
results = solve_brute(200)
assert all(r == results[-1] for r in results[50:]), "Not converged by iter 50"

answer = solve()
assert answer == 1710637717, f"Expected 1710637717, got {answer}"
print(f"{answer / 1e9:.9f}")
