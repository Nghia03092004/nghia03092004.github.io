#!/usr/bin/env python3
"""
Project Euler Problem 462: Permutation of 3-smooth Numbers

A 3-smooth number has the form 2^a * 3^b.
S(N) = set of 3-smooth numbers <= N.
F(N) = number of permutations of S(N) where each element comes after
       all its proper divisors (= linear extensions of divisibility poset).

Known: F(6)=5, F(8)=9, F(20)=450, F(1000)~8.8521816557e21
Find: F(10^18) in scientific notation with 10 decimal digits.

Answer: 5.5350769703e1512

Approach: The 3-smooth numbers form a Young-diagram-like poset.
Use the hook-length formula for counting linear extensions.
"""

import math
import sys
from decimal import Decimal, getcontext

# Set high precision for large number computation
getcontext().prec = 2000

def get_3smooth_up_to(N):
    """Generate all 3-smooth numbers <= N, sorted."""
    result = []
    b = 0
    pow3 = 1
    while pow3 <= N:
        a = 0
        val = pow3
        while val <= N:
            result.append(val)
            a += 1
            val = pow3 * (1 << a)
        b += 1
        pow3 *= 3
    result.sort()
    return result

def get_staircase(N):
    """
    Get the staircase shape for 3-smooth numbers <= N.
    Returns list of row lengths: row_lengths[b] = max_a + 1
    where 2^a * 3^b <= N.
    """
    rows = []
    b = 0
    pow3 = 1
    while pow3 <= N:
        # Find max a such that 2^a * 3^b <= N
        max_a = int(math.log2(N / pow3)) if pow3 <= N else -1
        if max_a >= 0:
            rows.append(max_a + 1)  # number of elements in this row
        b += 1
        pow3 *= 3
    return rows

def F_bruteforce(N):
    """Brute force: count linear extensions by generating all valid permutations."""
    smooth = get_3smooth_up_to(N)
    n = len(smooth)

    # Build divisor relation
    # For each element, find its proper divisors in the set
    idx = {v: i for i, v in enumerate(smooth)}
    predecessors = [set() for _ in range(n)]
    for i, v in enumerate(smooth):
        for j, u in enumerate(smooth):
            if u != v and v % u == 0:
                predecessors[i].add(j)

    # Count linear extensions via recursive enumeration
    count = 0
    placed = [False] * n

    def backtrack(pos):
        nonlocal count
        if pos == n:
            count += 1
            return
        for i in range(n):
            if placed[i]:
                continue
            # Check all predecessors are placed
            if all(placed[j] for j in predecessors[i]):
                placed[i] = True
                backtrack(pos + 1)
                placed[i] = False

    backtrack(0)
    return count

def hook_length(rows):
    """
    Compute number of linear extensions using hook-length formula
    for a Young diagram with given row lengths.

    rows must be non-increasing (standard Young diagram).

    F = n! / prod of hook lengths
    """
    n = sum(rows)
    if n == 0:
        return 1

    num_rows = len(rows)

    # Compute column lengths from row lengths
    max_col = rows[0] if rows else 0
    cols = [0] * max_col
    for r in rows:
        for j in range(r):
            cols[j] += 1

    # Compute hook lengths for each cell
    # hook(i, j) = (rows[i] - j) + (cols[j] - i) - 1
    #            = arm_length + leg_length + 1
    hook_product_log = 0.0
    for i in range(num_rows):
        for j in range(rows[i]):
            arm = rows[i] - j - 1
            leg = cols[j] - i - 1
            h = arm + leg + 1
            hook_product_log += math.log10(h)

    # F = n! / product of hooks
    # log10(F) = log10(n!) - sum(log10(hooks))
    log_n_fact = sum(math.log10(k) for k in range(1, n + 1))
    log_F = log_n_fact - hook_product_log

    return log_F

def hook_length_exact(rows):
    """Exact computation using Python arbitrary precision."""
    n = sum(rows)
    if n == 0:
        return 1

    num_rows = len(rows)
    max_col = rows[0] if rows else 0
    cols = [0] * max_col
    for r in rows:
        for j in range(r):
            cols[j] += 1

    # Compute n! / product of hooks
    numerator = math.factorial(n)
    denominator = 1
    for i in range(num_rows):
        for j in range(rows[i]):
            arm = rows[i] - j - 1
            leg = cols[j] - i - 1
            h = arm + leg + 1
            denominator *= h

    return numerator // denominator

def solve(N):
    """
    Solve F(N) for 3-smooth numbers.

    The poset of 3-smooth numbers under divisibility corresponds to
    lattice points (a, b) with 2^a * 3^b <= N, ordered componentwise.

    The staircase shape is a Young diagram (row lengths are non-increasing
    since increasing b means smaller max_a).

    Number of linear extensions = n! / product of hook lengths.
    """
    rows = get_staircase(N)

    # Verify rows are non-increasing (valid Young diagram)
    for i in range(1, len(rows)):
        assert rows[i] <= rows[i-1], f"Not a valid Young diagram: {rows}"

    n = sum(rows)
    print(f"  N = {N}")
    print(f"  Number of 3-smooth numbers: {n}")
    print(f"  Staircase rows: {rows}")

    if N <= 10000:
        # Use exact computation
        F = hook_length_exact(rows)
        print(f"  F({N}) = {F}")
        return F
    else:
        # Use logarithmic computation
        log_F = hook_length(rows)
        exponent = int(log_F)
        mantissa = 10 ** (log_F - exponent)
        print(f"  log10(F({N})) = {log_F:.6f}")
        print(f"  F({N}) ~ {mantissa:.10f}e{exponent}")
        return log_F

def solve_large(N):
    """Solve for very large N using Stirling and high-precision log."""
    rows = get_staircase(N)
    n = sum(rows)

    print(f"  N = {N:.2e}")
    print(f"  Number of 3-smooth numbers: {n}")
    print(f"  Number of rows: {len(rows)}")
    print(f"  Row lengths: {rows[:10]}... (first 10)")

    # Compute log10(F) = log10(n!) - sum(log10(hook_lengths))
    # Use Stirling for n!: log10(n!) = n*log10(n) - n*log10(e) + 0.5*log10(2*pi*n)

    # For precision, compute with Decimal
    D = Decimal

    log10 = lambda x: D(x).ln() / D(10).ln()

    # log10(n!)
    log_n_fact = D(0)
    for k in range(1, n + 1):
        log_n_fact += log10(k)

    # Hook lengths
    max_col = rows[0]
    cols = [0] * max_col
    for r in rows:
        for j in range(r):
            cols[j] += 1

    log_hooks = D(0)
    for i in range(len(rows)):
        for j in range(rows[i]):
            arm = rows[i] - j - 1
            leg = cols[j] - i - 1
            h = arm + leg + 1
            log_hooks += log10(h)

    log_F = log_n_fact - log_hooks

    exponent = int(log_F)
    mantissa_log = log_F - exponent
    mantissa = D(10) ** mantissa_log

    print(f"  log10(F) = {log_F}")
    print(f"  F({N:.2e}) ~ {mantissa:.10f}e{exponent}")

    return float(log_F), float(mantissa), exponent

def create_visualization():
    """Create visualization for the 3-smooth numbers problem."""
