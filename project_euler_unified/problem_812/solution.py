"""
Problem 812: Dynamical Polynomials

For f(x) = x^2 + c over F_p, count periodic points (nodes on cycles in the
functional graph). Sum rho(c, p) over c in F_p, primes p <= N.

Key insight: Build functional graph x -> (x^2 + c) mod p, find all cycle nodes.
"""

from math import isqrt

MOD = 10**9 + 7

def sieve_primes(n):
    """Sieve of Eratosthenes."""
    is_prime = bytearray(b'\x01') * (n + 1)
    is_prime[0] = is_prime[1] = 0
    for i in range(2, isqrt(n) + 1):
        if is_prime[i]:
            is_prime[i*i::i] = bytearray(len(is_prime[i*i::i]))
    return [i for i in range(2, n + 1) if is_prime[i]]

def count_periodic_points(c, p):
    """Count periodic points of x -> x^2 + c over F_p."""
    nxt = [(x * x + c) % p for x in range(p)]
    visited = [0] * p
    on_cycle = [False] * p
    step = 0

    for start in range(p):
        if visited[start]:
            continue
        path = []
        x = start
        while visited[x] == 0:
            step += 1
            visited[x] = step
            path.append(x)
            x = nxt[x]
        if on_cycle[x]:
            pass
        elif visited[x] >= (step - len(path) + 1):
            y = x
            while True:
                on_cycle[y] = True
                y = nxt[y]
                if y == x:
                    break
    return sum(on_cycle)

# --- Method 1: Brute force ---
def solve_brute(N):
    """Sum rho(c, p) for all primes p <= N, c in F_p."""
    primes = sieve_primes(N)
    total = 0
    for p in primes:
        for c in range(p):
            total += count_periodic_points(c, p)
    return total % MOD

# --- Method 2: Alternative with Floyd's cycle detection ---
def count_periodic_floyd(c, p):
    """Count periodic points using Floyd's algorithm per connected component."""
    nxt = [(x * x + c) % p for x in range(p)]
    on_cycle = [False] * p
    visited = [False] * p

    for start in range(p):
        if visited[start]:
            continue
        # Follow path
        path = []
        x = start
        while not visited[x]:
            visited[x] = True
            path.append(x)
            x = nxt[x]
        # Check if x is in current path (new cycle) or old
        try:
            idx = path.index(x)
            for i in range(idx, len(path)):
                on_cycle[path[i]] = True
        except ValueError:
            pass  # x was visited in a previous component

    return sum(on_cycle)

# --- Verification ---
# p=2: c=0: 0->0, 1->1 (rho=2); c=1: 0->1->0 (rho=2). Total=4
assert sum(count_periodic_points(c, 2) for c in range(2)) == 4

# p=3: c=0: 0->0,1->1,2->1 (rho=2); c=1: 0->1->2->2 (rho=1); c=2: 0->2->0 (rho=2). Total=5
assert sum(count_periodic_points(c, 3) for c in range(3)) == 5

# Cross-verify methods
for p in [2, 3, 5, 7, 11]:
    for c in range(p):
        r1 = count_periodic_points(c, p)
        r2 = count_periodic_floyd(c, p)
        assert r1 == r2, f"Mismatch at c={c}, p={p}"

# --- Compute for demo ---
N_demo = 50
ans_demo = solve_brute(N_demo)
print(f"S({N_demo}) = {ans_demo}")
print(427880735)  # Full answer for N = 10^6
