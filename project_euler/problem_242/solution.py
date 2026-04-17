"""
Project Euler Problem 242: Odd Triplets

Count triplets (a,b,c) with 0 < a <= b <= c, a+b+c <= N, a XOR b XOR c = 0.
N = 10^12, answer mod 10^9.

Digit DP on the binary representation of N.
XOR-zero constraint: at each bit, (a_i, b_i, c_i) in {(0,0,0),(0,1,1),(1,0,1),(1,1,0)}.
State: (bit position, carry, tight, ordering of a<=b and b<=c).
"""

import functools

MOD = 10**9
N = 10**12

bits = []
tmp = N
while tmp > 0:
    bits.append(tmp & 1)
    tmp >>= 1
nbits = len(bits)

patterns = [(0, 0, 0), (0, 1, 1), (1, 0, 1), (1, 1, 0)]


@functools.lru_cache(maxsize=None)
def solve(pos, carry, tight, order):
    if pos < 0:
        return 1 if carry == 0 else 0

    result = 0
    n_bit = bits[pos]

    for ai, bi, ci in patterns:
        a_eq_b = order in (0, 1)
        b_eq_c = order in (0, 2)

        if a_eq_b and ai > bi:
            continue
        if b_eq_c and bi > ci:
            continue

        new_a_lt_b = (order in (2, 3)) or (ai < bi)
        new_b_lt_c = (order in (1, 3)) or (bi < ci)

        new_order = 0
        if not new_a_lt_b and new_b_lt_c:
            new_order = 1
        elif new_a_lt_b and not new_b_lt_c:
            new_order = 2
        elif new_a_lt_b and new_b_lt_c:
            new_order = 3

        s = ai + bi + ci + carry
        sum_bit = s % 2
        new_carry = s // 2

        if tight:
            if sum_bit > n_bit:
                continue
            new_tight = 1 if sum_bit == n_bit else 0
        else:
            new_tight = 0

        result += solve(pos - 1, new_carry, new_tight, new_order)

    return result % MOD


ans = solve(nbits - 1, 0, 1, 0)
ans = (ans - 1) % MOD
print(ans)
