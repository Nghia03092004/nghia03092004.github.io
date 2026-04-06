"""Project Euler Problem 33: Digit Cancelling Fractions"""
from math import gcd

def main():
    num_prod, den_prod = 1, 1
    for a in range(1, 10):
        for b in range(1, 10):
            denom = 9 * a + b
            numer = 10 * a * b
            if numer % denom != 0:
                continue
            e = numer // denom
            if e < 1 or e > 9:
                continue
            if 10 * a + b >= 10 * b + e:
                continue
            num_prod *= a
            den_prod *= e
    print(den_prod // gcd(num_prod, den_prod))  # 100

if __name__ == "__main__":
    main()
