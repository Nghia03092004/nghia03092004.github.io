"""
Project Euler Problem 468: Smooth Divisors of Binomial Coefficients

S_B(n) = largest B-smooth divisor of n
F(n) = sum_{B=1}^{n} sum_{r=0}^{n} S_B(C(n,r))

Find F(11111111) mod 1000000993.
"""

import sys
from collections import defaultdict

MOD = 1_000_000_993

def sieve_primes(limit):
    """Return list of primes up to limit."""
    is_prime = [True] * (limit + 1)
    is_prime[0] = is_prime[1] = False
    for i in range(2, int(limit**0.5) + 1):
        if is_prime[i]:
            for j in range(i*i, limit + 1, i):
                is_prime[j] = False
    return [i for i in range(2, limit + 1) if is_prime[i]]

def base_digits(n, p):
    """Return digits of n in base p, least significant first."""
    if n == 0:
        return [0]
    digits = []
    while n > 0:
        digits.append(n % p)
        n //= p
    return digits

def pow_mod(base, exp, mod):
    """Fast modular exponentiation."""
    result = 1
    base %= mod
    while exp > 0:
        if exp & 1:
            result = result * base % mod
        exp >>= 1
        base = base * base % mod
    return result

def compute_sum_p_valuation(n, p, mod):
    """
    Compute sum_{r=0}^{n} p^{v_p(C(n,r))} mod `mod`.

    v_p(C(n,r)) = (s_p(r) + s_p(n-r) - s_p(n)) / (p-1)
    where s_p(k) = digit sum of k in base p.

    Equivalently, v_p(C(n,r)) = number of carries when adding r and (n-r) in base p.

    We use digit DP: process digits of n in base p from least significant.
    State: carry_in (0 or 1), tracking sum of p^(carries).

    At each digit position j with n_j = digit of n:
    - Choose r_j in [0, n_j + carry_in] considering carry
    - r_j is the j-th digit of r in base p
    - carry_out = 1 if r_j + (n_j - r_j + carry_in_from_complement) >= p

    Actually, let's think about it differently.
    When adding r and (n-r) in base p:
    - digit of r: d
    - digit of (n-r): depends on borrows

    A cleaner approach: use the Kummer's theorem interpretation directly.
    v_p(C(n,r)) = number of carries when adding r and n-r in base p.

    Let n have digits n_0, n_1, ..., n_k in base p (LSB first).
    For r with digits r_0, r_1, ..., r_k:
    - At position j, the sum r_j + (n_j - r_j) in the absence of carries would be n_j
    - With carry c_j from position j-1: we compute r_j + s_j + c_j where s_j = n_j - r_j adjusted

    Let me use a different DP formulation.

    Let c_j be the carry into position j (c_0 = 0).
    At position j: r_j can be 0..p-1 but we need 0 <= r_j <= ... valid.

    The digit of n-r at position j: (n_j - r_j - borrow_j) mod p
    borrow_{j+1} = 1 if n_j - r_j - borrow_j < 0

    Then carry when adding r and (n-r) at position j:
    r_j + ((n_j - r_j - borrow_j) mod p) + carry_j
    = r_j + (n_j - r_j - borrow_j + p * borrow_{j+1}) + carry_j
    = n_j - borrow_j + p * borrow_{j+1} + carry_j

    If this >= p, carry_{j+1_add} = 1, else 0.
    But this equals n_j - borrow_j + p * borrow_{j+1} + carry_j.

    It turns out carry_{j+1_add} = borrow_{j+1} when everything is consistent.

    Total carries = sum of carry_{j+1_add} for each position = v_p(C(n,r)).

    Simpler approach using Kummer's theorem:
    v_p(C(n,r)) = number of carries when adding r and n-r in base p.
    This equals the number of positions j where r_j + (n-r)_j + c_j >= p.

    DP state: (position j, carry c in {0,1}), value = sum of p^(carries so far).

    Transition at position j with carry c:
    For each valid r_j (0 to p-1), determine (n-r)_j and new carry.

    We need r <= n, so we track borrow for the subtraction n - r as well.
    This adds another state dimension: (position, carry_add, borrow_sub).

    Both carry_add and borrow_sub are in {0, 1}, so 4 states per position.

    At position j:
    - n_j is the j-th digit of n in base p
    - r_j ranges from 0 to p-1
    - s_j = (n_j - r_j - borrow_sub) -- the "raw" difference
    - If s_j < 0: actual digit = s_j + p, new borrow = 1
    - Else: actual digit = s_j, new borrow = 0
    - sum_j = r_j + actual_s_j + carry_add
    - If sum_j >= p: new carry = 1, this position contributes a carry
    - Else: new carry = 0

    When a carry occurs, multiply accumulated value by p.

    DP[j][carry][borrow] = sum over valid r choices of p^(carries up to position j)
    """
    digits = base_digits(n, p)
    k = len(digits)  # number of digit positions

    # dp[carry][borrow] = accumulated sum mod `mod`
    dp = defaultdict(int)
    dp[(0, 0)] = 1  # initial state: no carry, no borrow

    p_mod = p % mod

    for j in range(k):
        n_j = digits[j]
        new_dp = defaultdict(int)

        for (carry, borrow), val in dp.items():
            if val == 0:
                continue
            for r_j in range(p):
                raw = n_j - r_j - borrow
                if raw < 0:
                    s_j = raw + p
                    new_borrow = 1
                else:
                    s_j = raw
                    new_borrow = 0

                total = r_j + s_j + carry
                if total >= p:
                    new_carry = 1
                    # This position has a carry, multiply by p
                    new_dp[(new_carry, new_borrow)] = (
                        new_dp[(new_carry, new_borrow)] + val * p_mod
                    ) % mod
                else:
                    new_carry = 0
                    new_dp[(new_carry, new_borrow)] = (
                        new_dp[(new_carry, new_borrow)] + val
                    ) % mod

        dp = new_dp

    # Final state must have carry=0 and borrow=0 (since r <= n means no final borrow,
    # and the addition r + (n-r) = n has no final carry)
    result = dp.get((0, 0), 0)
    return result % mod

def solve_small(n):
    """Brute force for small n to verify."""
    from math import comb

    def largest_smooth_divisor(num, B):
        """Largest B-smooth divisor of num."""
        if num == 0:
            return 0
        result = 1
        for p in range(2, B + 1):
            if not all(p % d != 0 for d in range(2, p)):
                continue  # skip non-primes
            # Check if p is prime
            is_p = True
            for d in range(2, int(p**0.5) + 1):
                if p % d == 0:
                    is_p = False
                    break
            if not is_p:
                continue
            while num % p == 0:
                result *= p
                num //= p
        return result

    def is_prime(x):
        if x < 2:
            return False
        for d in range(2, int(x**0.5) + 1):
            if x % d == 0:
                return False
        return True

    primes = [p for p in range(2, n + 1) if is_prime(p)]

    total = 0
    for B in range(1, n + 1):
        for r in range(n + 1):
            c = comb(n, r)
            sb = 1
            temp = c
            for p in primes:
                if p > B:
                    break
                while temp % p == 0:
                    sb *= p
                    temp //= p
                temp = c
            # Recompute properly
            sb = 1
            for p in primes:
                if p > B:
                    break
                cc = c
                while cc % p == 0:
                    sb *= p
                    cc //= p
            total += sb

    return total

def solve_small_v2(n):
    """Correct brute force."""
    from math import comb

    def is_prime(x):
        if x < 2: return False
        for d in range(2, int(x**0.5) + 1):
            if x % d == 0: return False
        return True

    primes = [p for p in range(2, n + 1) if is_prime(p)]

    total = 0
    for B in range(1, n + 1):
        smooth_primes = [p for p in primes if p <= B]
        for r in range(n + 1):
            c = comb(n, r)
            sb = 1
            for p in smooth_primes:
                while c % p == 0:
                    sb *= p
                    c //= p
                c = comb(n, r)  # reset
            # Actually need to extract all factors of each prime
            sb = 1
            for p in smooth_primes:
                cc = comb(n, r)
                e = 0
                while cc % p == 0:
                    cc //= p
                    e += 1
                sb *= p ** e
            total += sb
    return total

def solve_optimized(n, mod):
    """
    Optimized solution using per-prime digit DP.

    F(n) = sum_{B=1}^{n} sum_{r=0}^{n} S_B(C(n,r))
         = sum_{B=1}^{n} sum_{r=0}^{n} prod_{p<=B, p prime} p^{v_p(C(n,r))}

    Rearranging: let primes be p_1 < p_2 < ... < p_k (all primes <= n).
    Define G_i = sum_{r=0}^{n} prod_{j=1}^{i} p_j^{v_{p_j}(C(n,r))}

    Then F(n) = sum_{i=0}^{k} gap_i * G_i
    where gap_i = (p_{i+1} - p_i) for 0 <= i < k, with p_0 = 1 and gap for
    the last segment is (n - p_k + 1) if p_k <= n, plus gap for B values
    between p_k and n.

    Wait, let's be more careful. For B from 1 to n:
    - For B in [1, p_1-1]: S_B(C(n,r)) = 1 (no primes <= B)
    - For B in [p_1, p_2-1]: S_B uses only p_1
    - For B in [p_i, p_{i+1}-1]: S_B uses p_1,...,p_i
    - For B in [p_k, n]: S_B uses all primes

    So: F(n) = (p_1 - 1) * (n+1)  [since S_B = 1 for all r]
             + sum_{i=1}^{k} (next - p_i) * G_i

    where next = p_{i+1} for i < k, and next = n+1 for i = k.

    Problem: G_i is the product over multiple primes, and the r-sum doesn't
    factor into independent per-prime sums because different primes' valuations
    of C(n,r) are NOT independent.

    However, for C(n,r), using Kummer's theorem, v_p(C(n,r)) depends on the
    base-p representation of r. Since different primes have different bases,
    the joint distribution over r of (v_{p1}, v_{p2}, ...) is complex.

    For the full solution, we need a different approach or accept that
    the direct computation is infeasible in Python for n=11111111.
    """
    primes = sieve_primes(n)
    k = len(primes)

    # For each prime p, compute: for each r in 0..n, p^{v_p(C(n,r))}
    # This is what compute_sum_p_valuation does: sum_r p^{v_p(C(n,r))}

    # But we need the PRODUCT over primes, summed over r.
    # Since different primes' valuations are independent when viewed via CRT
    # on the digits of r... actually they ARE independent!

    # Key insight: v_p(C(n,r)) depends on the base-p digits of r.
    # v_q(C(n,r)) depends on the base-q digits of r.
    # For different primes p and q, knowing the base-p digits of r tells
    # nothing about the base-q digits (by CRT, for r in a suitable range).

    # BUT r ranges from 0 to n, not 0 to p*q*..., so they're not fully
    # independent. However, for large n, the dependence is weak.

    # Actually, the correct approach uses the multiplicativity more carefully.
    # For each B, S_B(C(n,r)) = prod_{p<=B} p^{v_p(C(n,r))}.
    # Sum over r: sum_r prod_{p<=B} p^{v_p(C(n,r))}.

    # This is NOT simply the product of individual sums because the product
    # is inside the sum. The valuations for different primes are correlated
    # through r.

    # The efficient approach for this problem uses the following:
    # For each prime p, define f_p(r) = p^{v_p(C(n,r))}.
    # Then S_B(C(n,r)) = prod_{p<=B} f_p(r).

    # By Dirichlet convolution / multiplicative function theory on C(n,r),
    # there's a way to compute this efficiently.

    # For now, implement a working solution for small n and provide the
    # framework for the full solution.

    print(f"Number of primes up to {n}: {k}")
    print("Computing per-prime sums...")

    # Compute individual prime sums (these are needed even if not the full answer)
    prime_sums = {}
    for p in primes[:20]:  # Just first few for demonstration
        s = compute_sum_p_valuation(n, p, mod)
        prime_sums[p] = s
        print(f"  sum_r {p}^v_{p}(C({n},r)) = {s} (mod {mod})")

    return prime_sums

def create_visualization(n_small=11):
    """Create visualization for small case."""
    from math import comb

    def is_prime(x):
        if x < 2: return False
        for d in range(2, int(x**0.5) + 1):
            if x % d == 0: return False
        return True

    primes = [p for p in range(2, n_small + 1) if is_prime(p)]
