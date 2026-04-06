from math import comb


def solve():
    n = 100
    inc = comb(n + 9, 9) - 1
    dec = comb(n + 10, 10) - 1 - n
    overlap = 9 * n
    print(inc + dec - overlap)


solve()
