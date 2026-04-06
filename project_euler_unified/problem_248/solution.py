import math

def solve():
    """
    Problem 248: Find the 150000th n with phi(n) = 13!
    """
    target = math.factorial(13)  # 6227020800

    # Factorization of 13! = 2^10 * 3^5 * 5^2 * 7 * 11 * 13
    factors = [(2, 10), (3, 5), (5, 2), (7, 1), (11, 1), (13, 1)]

    # Generate all divisors of 13!
    divisors = [1]
    for p, e in factors:
        new_divs = []
        for d in divisors:
            pk = 1
            for k in range(1, e + 1):
                pk *= p
                new_divs.append(d * pk)
        divisors.extend(new_divs)
    divisors.sort()

    def is_prime(n):
        if n < 2: return False
        if n < 4: return True
        if n % 2 == 0 or n % 3 == 0: return False
        i = 5
        while i * i <= n:
            if n % i == 0 or n % (i+2) == 0: return False
            i += 6
        return True

    # Candidate primes
    primes = sorted(set(d + 1 for d in divisors if is_prime(d + 1)))

    solutions = []

    def search(idx, rem, n):
        if rem == 1:
            solutions.append(n)
            return
        for i in range(idx, len(primes)):
            p = primes[i]
            if p - 1 > rem:
                break
            if rem % (p - 1) != 0:
                continue
            r = rem // (p - 1)
            pk = 1       # p^(k-1)
            n_factor = p # p^k
            while True:
                if r % pk != 0:
                    break
                if n_factor > 10**18 // max(n, 1):
                    break
                search(i + 1, r // pk, n * n_factor)
                pk *= p
                n_factor *= p

    search(0, target, 1)
    solutions.sort()
    print(solutions[150000 - 1])

solve()
