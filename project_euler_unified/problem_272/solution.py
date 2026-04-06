"""
Problem 272: Modular Cubes, Part 2

Find the sum of all n <= 10^11 with C(n) = 242, where C(n) is the number of
x with 1 < x < n and x^3 = 1 (mod n).

C(n) = c(n) - 1 where c(n) = 3^k, k = number of "active" prime conditions.
Need c(n) = 243 = 3^5.

Active conditions:
- Each distinct prime p = 1 mod 3 dividing n adds 1
- If 9 | n, adds 1

Two cases for k = 5:
Case A: exactly 5 primes = 1 mod 3, 9 does not divide n
Case B: exactly 4 primes = 1 mod 3, 9 divides n
"""

import sys
sys.setrecursionlimit(100000)

N = 10**11

def sieve_primes(lim):
    is_p = bytearray(b'\x01') * (lim + 1)
    is_p[0] = is_p[1] = 0
    for i in range(2, int(lim**0.5) + 1):
        if is_p[i]:
            is_p[i*i::i] = bytearray(len(is_p[i*i::i]))
    return [i for i in range(2, lim + 1) if is_p[i]]

primes = sieve_primes(50000000)
typeA = [p for p in primes if p >= 7 and p % 3 == 1]

total_sum = 0

def sum_multiples(L, d):
    """Sum of multiples of d in [1, L]."""
    if d > L or L <= 0:
        return 0
    f = L // d
    return d * f * (f + 1) // 2

def ie_dfs(forbidden, idx, L, prod, sign, active_prod, case_a):
    """Inclusion-exclusion on forbidden primes."""
    global total_sum
    if case_a:
        contrib = sum_multiples(L, prod)
        if prod * 9 <= L:
            contrib -= sum_multiples(L, prod * 9)
    else:
        contrib = sum_multiples(L, prod)
    total_sum += sign * active_prod * contrib

    for i in range(idx, len(forbidden)):
        p = forbidden[i]
        if prod * p > L:
            break
        ie_dfs(forbidden, i + 1, L, prod * p, -sign, active_prod, case_a)

def active_dfs(idx, cnt, target, prod, case_a, active_set):
    """Enumerate active prime sets."""
    if cnt == target:
        L = N // prod
        if L < 1:
            return
        # Build forbidden list
        ai = 0
        forbidden = []
        for p in typeA:
            if p > L:
                break
            if ai < len(active_set) and active_set[ai] == p:
                ai += 1
                continue
            forbidden.append(p)
        ie_dfs(forbidden, 0, L, 1, 1, prod, case_a)
        return

    for i in range(idx, len(typeA)):
        p = typeA[i]
        if prod * p > N:
            break
        active_set.append(p)
        active_dfs(i + 1, cnt + 1, target, prod * p, case_a, active_set)
        active_set.pop()

def solve():
    global total_sum
    # Case A: 5 type-A primes, no 9|n
    active_dfs(0, 0, 5, 1, True, [])
    # Case B: 4 type-A primes, 9|n
    active_dfs(0, 0, 4, 9, False, [])
    print(total_sum)

if __name__ == "__main__":
    solve()
