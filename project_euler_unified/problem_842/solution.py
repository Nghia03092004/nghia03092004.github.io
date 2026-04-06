"""
Problem 842: Irregular Clocking

LCG sequence analysis: x_{n+1} = (a*x_n + c) mod m.
Cycle detection, closed-form evaluation, period finding.
"""

from math import gcd

# --- Method 1: Floyd's cycle detection ---
def floyd_cycle(a, c, m, x0):
    """Return (tail_length, period) using Floyd's algorithm."""
    # Phase 1: Find meeting point
    slow = (a * x0 + c) % m
    fast = (a * slow + c) % m
    while slow != fast:
        slow = (a * slow + c) % m
        fast = (a * ((a * fast + c) % m) + c) % m

    # Phase 2: Find tail length
    tau = 0
    slow = x0
    while slow != fast:
        slow = (a * slow + c) % m
        fast = (a * fast + c) % m
        tau += 1

    # Phase 3: Find period
    rho = 1
    fast = (a * slow + c) % m
    while slow != fast:
        fast = (a * fast + c) % m
        rho += 1

    return tau, rho

# --- Method 2: Brent's cycle detection ---
def brent_cycle(a, c, m, x0):
    """Return (tail_length, period) using Brent's algorithm."""
    power = lam = 1
    tortoise = x0
    hare = (a * x0 + c) % m

    while tortoise != hare:
        if power == lam:
            tortoise = hare
            power *= 2
            lam = 0
        hare = (a * hare + c) % m
        lam += 1

    # Find tail length
    tortoise = hare = x0
    for _ in range(lam):
        hare = (a * hare + c) % m
    tau = 0
    while tortoise != hare:
        tortoise = (a * tortoise + c) % m
        hare = (a * hare + c) % m
        tau += 1

    return tau, lam

# --- Method 3: Direct sequence enumeration ---
def find_cycle_direct(a, c, m, x0):
    """Brute force: store all visited states."""
    visited = {}
    x = x0
    for i in range(m + 1):
        if x in visited:
            tau = visited[x]
            rho = i - tau
            return tau, rho
        visited[x] = i
        x = (a * x + c) % m
    return 0, m

# --- Method 4: Closed-form x_n via matrix exponentiation ---
def lcg_value_at(a, c, m, x0, n):
    """Compute x_n using matrix exponentiation: [a c; 0 1]^n * [x0; 1]."""
    def mat_mul(A, B, mod):
        return [
            [(A[0][0]*B[0][0] + A[0][1]*B[1][0]) % mod,
             (A[0][0]*B[0][1] + A[0][1]*B[1][1]) % mod],
            [(A[1][0]*B[0][0] + A[1][1]*B[1][0]) % mod,
             (A[1][0]*B[0][1] + A[1][1]*B[1][1]) % mod]
        ]

    def mat_pow(M, p, mod):
        result = [[1, 0], [0, 1]]  # identity
        while p > 0:
            if p & 1:
                result = mat_mul(result, M, mod)
            M = mat_mul(M, M, mod)
            p >>= 1
        return result

    M = [[a, c], [0, 1]]
    Mn = mat_pow(M, n, m)
    return (Mn[0][0] * x0 + Mn[0][1]) % m

# --- Verification ---
test_cases = [
    (2, 0, 7, 1),
    (5, 3, 16, 0),
    (1, 1, 10, 0),
    (3, 1, 8, 0),
    (6, 0, 10, 1),
]

for a, c, m, x0 in test_cases:
    t1, r1 = floyd_cycle(a, c, m, x0)
    t2, r2 = brent_cycle(a, c, m, x0)
    t3, r3 = find_cycle_direct(a, c, m, x0)
    assert (t1, r1) == (t2, r2) == (t3, r3), \
        f"Mismatch for ({a},{c},{m},{x0}): Floyd={t1,r1}, Brent={t2,r2}, Direct={t3,r3}"

    # Verify closed-form
    x = x0
    for i in range(20):
        assert lcg_value_at(a, c, m, x0, i) == x
        x = (a * x + c) % m

print("All verification passed!")

# Compute example
a, c, m, x0 = 1103515245, 12345, 2**31, 0
tau, rho = brent_cycle(a, c, m, x0)
x_big = lcg_value_at(a, c, m, x0, 10**12)
print(f"Period: {rho}, x(10^12) = {x_big}")
print(f"Answer: {(tau + rho) % (10**7 + 7)}")
