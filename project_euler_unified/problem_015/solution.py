"""Project Euler Problem 15: Lattice Paths"""


def solve():

    from math import comb

    return comb(40, 20)


answer = solve()
assert answer == 137846528820
print(answer)
