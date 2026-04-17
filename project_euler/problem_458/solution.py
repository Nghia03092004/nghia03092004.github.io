#!/usr/bin/env python3
"""
Project Euler Problem 458: Permutations of Project

T(n) = number of strings of length n over {p,r,o,j,e,c,t} that do NOT contain
any permutation of "project" as a contiguous substring.

T(7) = 7^7 - 7! = 818503.
Find T(10^12) mod 10^9.

Approach: State machine + matrix exponentiation.
State s = length of longest suffix of all-distinct characters (0..7).
State 7 is absorbing (forbidden). We want sum of states 0..6 after n steps.
"""

MOD = 10**9

def build_transition_matrix():
    """
    Build 8x8 transition matrix.
    M[to][from] = number of characters that cause transition from 'from' to 'to'.

    From state s (0 <= s <= 6):
      - To state (s+1): (7-s) ways (new distinct character)
      - To state i for i in 1..s: 1 way each (duplicate char at position i from right)
    From state s = 0:
      - To state 1: 7 ways (any character)
    From state 7 (absorbing):
      - To state 7: 7 ways

    Note: from state s, the only way to go to state 0 is... never (adding any char gives
    at least state 1). Except the initial state is 0 before any character.
    """
    M = [[0] * 8 for _ in range(8)]

    for s in range(7):  # from state s
        # New distinct char -> state s+1
        M[s + 1][s] = 7 - s
        # Duplicate at position i -> state i
        for i in range(1, s + 1):
            M[i][s] += 1

    # Absorbing state 7
    M[7][7] = 7

    return M

def mat_mul(A, B, mod):
    """Multiply two 8x8 matrices mod m."""
    n = len(A)
    C = [[0] * n for _ in range(n)]
    for i in range(n):
        for k in range(n):
            if A[i][k] == 0:
                continue
            for j in range(n):
                C[i][j] = (C[i][j] + A[i][k] * B[k][j]) % mod
    return C

def mat_pow(M, exp, mod):
    """Matrix exponentiation: M^exp mod m."""
    n = len(M)
    # Identity matrix
    result = [[0] * n for _ in range(n)]
    for i in range(n):
        result[i][i] = 1

    base = [row[:] for row in M]
    while exp > 0:
        if exp & 1:
            result = mat_mul(result, base, mod)
        base = mat_mul(base, base, mod)
        exp >>= 1
    return result

def solve(n, mod=MOD):
    """Compute T(n) mod mod."""
    M = build_transition_matrix()
    Mn = mat_pow(M, n, mod)

    # Initial state vector: all weight in state 0.
    # Final state = Mn * v0, where v0 = (1,0,...,0).
    # T(n) = sum of Mn[s][0] for s = 0..6
    total = 0
    for s in range(7):
        total = (total + Mn[s][0]) % mod

    return total

def verify():
    """Verify T(7) = 818503."""
    t7 = solve(7, 10**18)  # Use large mod to get exact value
    expected = 7**7 - 5040  # 818503
    print(f"T(7) = {t7}, expected = {expected}, match = {t7 == expected}")

    # Also verify small cases by brute force
    from itertools import product

    def has_perm_substring(s, length=7):
        """Check if string s contains a substring that is a permutation of 7 distinct chars."""
        for i in range(len(s) - length + 1):
            if len(set(s[i:i+length])) == length:
                return True
        return False

    for n in range(1, 10):
        count = 0
        for combo in product(range(7), repeat=n):
            if not has_perm_substring(combo):
                count += 1
        t_n = solve(n, 10**18)
        status = "OK" if count == t_n else "FAIL"
        print(f"T({n}) = {t_n}, brute force = {count} [{status}]")

def generate_visualization():
    """Generate visualization of the state machine and T(n) growth."""
