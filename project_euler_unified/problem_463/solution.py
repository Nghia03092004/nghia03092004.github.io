"""
Problem 463: A Weird Recurrence Relation

f(1) = 1, f(3) = 3
f(2n) = f(n)
f(4n+1) = 2f(2n+1) - f(n)
f(4n+3) = 3f(2n+1) - 2f(n)

S(n) = sum_{i=1}^{n} f(i). Find S(3^37) mod 10^9.

Key insight: S(2N) = S(N) + T(N-1) where T(k) = sum of f at odd indices.
With memoization, O(log^2 n) complexity.
"""

import sys

sys.setrecursionlimit(10000)

MOD = 10**9

# --- Method 1: Brute force for small n ---
def compute_f_brute(N: int) -> list:
    """Compute f(1..N) directly."""
    f = [0] * (N + 1)
    f[1] = 1
    if N >= 3:
        f[3] = 3
    for n in range(2, N + 1):
        if f[n] != 0:
            continue
        if n % 2 == 0:
            f[n] = f[n // 2]
        elif n % 4 == 1:
            k = (n - 1) // 4
            f[n] = 2 * f[2 * k + 1] - f[k]
        else:  # n % 4 == 3
            k = (n - 3) // 4
            f[n] = 3 * f[2 * k + 1] - 2 * f[k]
    return f

def S_brute(N: int):
    """Compute S(N) by brute force."""
    f = compute_f_brute(N)
    return sum(f[1:N + 1])

# --- Method 2: Memoized recursion for large n ---
class Solver:
    def __init__(self, mod: int):
        self.mod = mod
        self.memo_f = {}
        self.memo_S = {}
        self.memo_T = {}

    def f(self, n: int):
        """Compute f(n) mod self.mod with memoization."""
        if n in self.memo_f:
            return self.memo_f[n]

        if n == 1:
            result = 1
        elif n % 2 == 0:
            result = self.f(n // 2)
        elif n % 4 == 1:
            k = (n - 1) // 4
            result = (2 * self.f(2 * k + 1) - self.f(k)) % self.mod
        else:  # n % 4 == 3
            k = (n - 3) // 4
            result = (3 * self.f(2 * k + 1) - 2 * self.f(k)) % self.mod

        self.memo_f[n] = result
        return result

    def T(self, k: int):
        """Compute T(k) = f(1) + f(3) + ... + f(2k+1) mod self.mod."""
        if k in self.memo_T:
            return self.memo_T[k]

        if k == 0:
            result = 1
        elif k == 1:
            result = 4 % self.mod
        elif k % 2 == 0:
            half = k // 2
            result = (2 * self.T(half) + 3 * self.T(half - 1)
                      - self.S(half) - 2 * self.S(half - 1) - 1) % self.mod
        else:  # k odd
            result = (self.T(k - 1) + 3 * self.f(k) - 2 * self.f((k - 1) // 2)) % self.mod

        self.memo_T[k] = result
        return result

    def S(self, n: int):
        """Compute S(n) = sum_{i=1}^{n} f(i) mod self.mod."""
        if n in self.memo_S:
            return self.memo_S[n]

        if n <= 0:
            return 0
        if n == 1:
            result = 1
        elif n % 2 == 0:
            result = (self.S(n // 2) + self.T(n // 2 - 1)) % self.mod
        else:
            result = (self.S(n // 2) + self.T(n // 2)) % self.mod

        self.memo_S[n] = result
        return result

# --- Verify small cases ---
bf = compute_f_brute(200)
assert S_brute(8) == 22, f"S(8) = {S_brute(8)}"
assert S_brute(100) == 3604, f"S(100) = {S_brute(100)}"

# Verify memoized solver against brute force
solver_check = Solver(10**18)  # large mod to avoid reduction effects
for n in range(1, 100):
    s_bf = S_brute(n)
    s_memo = solver_check.S(n)
    assert s_bf == s_memo, f"S({n}): brute={s_bf}, memo={s_memo}"

# --- Compute the answer ---
solver = Solver(MOD)
ans = solver.S(3**37)
print(ans)
assert ans == 808981553, f"Expected 808981553, got {ans}"
