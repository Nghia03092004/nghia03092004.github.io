"""
Problem 887: Chained Radicals
Nested radical: sqrt(a + sqrt(a + ...)) = (1 + sqrt(1+4a))/2.
"""

from math import sqrt

def nested_radical_exact(a):
    """Closed form: (1 + sqrt(1 + 4a)) / 2."""
    return (1 + sqrt(1 + 4 * a)) / 2

def nested_radical_iterative(a, iterations=100):
    """Fixed-point iteration: x_{n+1} = sqrt(a + x_n)."""
    x = 0.0
    for _ in range(iterations):
        x = sqrt(a + x)
    return x

def convergence_history(a, iterations=20):
    """Return list of iterates."""
    x = 0.0
    history = [x]
    for _ in range(iterations):
        x = sqrt(a + x)
        history.append(x)
    return history

def integer_solutions(max_n=20):
    """Find a where R(a) is integer: a = n(n-1)."""
    return [(n, n * (n - 1)) for n in range(1, max_n + 1)]

# --- Verification ---
print("=== Nested Radical Exact vs Iterative ===")
for a in [0, 1, 2, 3, 6, 12, 20, 30, 100]:
    exact = nested_radical_exact(a)
    iterative = nested_radical_iterative(a, 200)
    diff = abs(exact - iterative)
    print(f"  a={a:>3}: exact={exact:.6f}, iter={iterative:.6f}, diff={diff:.2e}")
    assert diff < 1e-10

print("\n=== Integer Solutions ===")
for n, a in integer_solutions(10):
    R = nested_radical_exact(a)
    print(f"  R({a}) = {R:.1f} (n={n})")
    assert abs(R - n) < 1e-10

print("\n=== Golden Ratio ===")
phi = nested_radical_exact(1)
phi_true = (1 + sqrt(5)) / 2
print(f"  R(1) = {phi:.10f}")
print(f"  phi  = {phi_true:.10f}")
assert abs(phi - phi_true) < 1e-12

print("\n=== Convergence for a=2 ===")
hist = convergence_history(2, 10)
for i, x in enumerate(hist):
    print(f"  x_{i} = {x:.6f}, error = {abs(x - 2):.6f}")

answer = nested_radical_exact(1)
print(f"\nAnswer: R(1) = phi = {answer:.10f}")

# --- 4-Panel Visualization ---
