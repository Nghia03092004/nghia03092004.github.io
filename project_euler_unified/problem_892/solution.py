"""
Problem 892: Recursive Digit Factorial
f(n) = sum of d! for each digit d of n. Find factorions (f(n) = n) and cycles.
"""

from math import factorial

FACT = [factorial(d) for d in range(10)]

def digit_factorial(n):
    """Compute f(n) = sum of d! for each digit d."""
    if n == 0:
        return 1
    total = 0
    while n > 0:
        total += FACT[n % 10]
        n //= 10
    return total

def find_factorions(limit):
    """Find all n <= limit where f(n) = n."""
    result = []
    for n in range(1, limit + 1):
        if digit_factorial(n) == n:
            result.append(n)
    return result

def orbit(n, max_steps=100):
    """Compute the orbit of n under f."""
    path = [n]
    seen = {n: 0}
    for step in range(1, max_steps):
        n = digit_factorial(n)
        if n in seen:
            cycle_start = seen[n]
            return path, path[cycle_start:]
        seen[n] = step
        path.append(n)
    return path, []

def find_cycles(limit):
    """Find all cycles in the functional graph."""
    visited = set()
    cycles = []
    for start in range(1, limit + 1):
        if start in visited:
            continue
        path, cycle = orbit(start)
        for v in path:
            visited.add(v)
        if cycle and tuple(sorted(cycle)) not in [tuple(sorted(c)) for c in cycles]:
            cycles.append(cycle)
    return cycles

# --- Verification ---
print("=== Factorions ===")
factorions = find_factorions(100000)
print(f"  Factorions up to 100000: {factorions}")

# Extended search
factorions_full = find_factorions(2540160)
print(f"  All factorions: {factorions_full}")
assert set(factorions_full) == {1, 2, 145, 40585}

print("\n=== Verification ===")
for n in [1, 2, 145, 40585]:
    fn = digit_factorial(n)
    print(f"  f({n}) = {fn}: {'FACTORION' if fn == n else 'NOT'}")
    assert fn == n

print("\n=== Orbit Examples ===")
for start in [3, 7, 69, 169, 871]:
    path, cycle = orbit(start, 20)
    print(f"  {start}: path={path[:8]}{'...' if len(path) > 8 else ''}")
    if cycle:
        print(f"         cycle={cycle}")

print("\n=== Upper Bound ===")
for k in range(1, 9):
    max_f = k * 362880
    min_n = 10 ** (k - 1) if k > 1 else 1
    print(f"  k={k} digits: max f = {max_f}, min n = {min_n}, "
          f"{'feasible' if max_f >= min_n else 'impossible'}")

answer = sorted(factorions_full)
print(f"\nAnswer: Factorions = {answer}")

# --- 4-Panel Visualization ---
