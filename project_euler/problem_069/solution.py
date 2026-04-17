"""
Project Euler Problem 69: Totient Maximum

Find the value of n <= 1,000,000 for which n/phi(n) is a maximum.

By Theorem 3 (Primorial Optimality), n is the largest primorial <= 1,000,000.
n/phi(n) = prod p/(p-1) over distinct prime factors, maximized by including
the most consecutive smallest primes whose product does not exceed the limit.
"""


def main():
    LIMIT = 1_000_000
    primes = [2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31]

    n = 1
    for p in primes:
        if n * p > LIMIT:
            break
        n *= p

    print(n)


if __name__ == "__main__":
    main()
