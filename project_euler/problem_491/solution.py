from math import factorial
from itertools import product

def solve():
    """
    Count double pandigital numbers divisible by 11.

    A 20-digit number using each of 0-9 exactly twice is divisible by 11
    iff the odd-position digit sum O satisfies O = 1 (mod 11).
    We enumerate assignment vectors c in {0,1,2}^10 with sum 10,
    filter on sum(d*c_d) = 1 (mod 11), and count permutations
    minus leading-zero arrangements.
    """
    fact = [factorial(i) for i in range(11)]
    answer = 0

    for combo in product(range(3), repeat=10):
        if sum(combo) != 10:
            continue
        if sum(d * combo[d] for d in range(10)) % 11 != 1:
            continue

        odd_perms = fact[10]
        even_perms = fact[10]
        for d in range(10):
            odd_perms //= fact[combo[d]]
            even_perms //= fact[2 - combo[d]]

        ways = odd_perms * even_perms

        if combo[0] >= 1:
            lz = fact[9] // fact[combo[0] - 1]
            for d in range(1, 10):
                lz //= fact[combo[d]]
            ways -= lz * even_perms

        answer += ways

    print(answer)

solve()
