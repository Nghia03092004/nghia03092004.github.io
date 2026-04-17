"""
Problem 128: Hexagonal Tile Differences

Find the 2000th tile for which exactly 3 of the differences
with its 6 neighbors are prime.

Key insight: Only the first and last tile of each ring can have PD=3.
- First tile S(r) = 3r(r-1)+2: needs 6r-1, 6r+1, 12r+5 all prime
- Last tile  E(r) = 3r(r+1)+1: needs 6r-1, 6r+5, 12r-7 all prime (r>=2)
"""

from sympy import isprime

def solve(target=2000):
    count = 1  # tile 1 is the first with PD=3

    r = 1
    while True:
        # First tile of ring r
        if isprime(6*r - 1) and isprime(6*r + 1) and isprime(12*r + 5):
            count += 1
            if count == target:
                return 3*r*(r-1) + 2

        # Last tile of ring r (r >= 2 for 12r-7 > 1)
        if r >= 2 and isprime(6*r - 1) and isprime(6*r + 5) and isprime(12*r - 7):
            count += 1
            if count == target:
                return 3*r*(r+1) + 1

        r += 1

if __name__ == '__main__':
    answer = solve()
    print(answer)
