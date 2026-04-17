from itertools import permutations


def is_prime(n):
    """Deterministic primality test by trial division."""
    if n < 2:
        return False
    if n < 4:
        return True
    if n % 2 == 0 or n % 3 == 0:
        return False
    i = 5
    while i * i <= n:
        if n % i == 0 or n % (i + 2) == 0:
            return False
        i += 6
    return True


def solve():
    """Find the largest n-digit pandigital prime.

    By the divisibility-by-3 criterion, n-pandigital numbers are composite
    for n in {2,3,5,6,8,9}. Only n=7 and n=4 can yield primes.
    We search 7-digit pandigital numbers in descending order first.
    """
    for n in (7, 4):
        for perm in permutations(range(n, 0, -1)):
            num = int(''.join(map(str, perm)))
            if is_prime(num):
                return num
    return None


answer = solve()
assert answer == 7652413
print(answer)
