from itertools import combinations


def is_prime(n):
    """Deterministic Miller-Rabin for n < 3.2 * 10^13 (Jaeschke 1993)."""
    if n < 2:
        return False
    if n < 4:
        return True
    if n % 2 == 0 or n % 3 == 0:
        return False
    d, r = n - 1, 0
    while d % 2 == 0:
        d //= 2
        r += 1
    for a in (2, 3, 5, 7, 11, 13):
        if a >= n:
            continue
        x = pow(a, d, n)
        if x == 1 or x == n - 1:
            continue
        for _ in range(r - 1):
            x = pow(x, 2, n)
            if x == n - 1:
                break
        else:
            return False
    return True


def solve():
    n = 10
    total = 0
    for d in range(10):
        for k in range(n - 1, 0, -1):
            free = n - k
            sum_primes = 0
            for positions in combinations(range(n), free):
                pos_set = set(positions)

                def generate(idx, num):
                    nonlocal sum_primes
                    if idx == n:
                        if num >= 10 ** (n - 1) and is_prime(num):
                            sum_primes += num
                        return
                    if idx in pos_set:
                        for dig in range(10):
                            if dig != d:
                                generate(idx + 1, num * 10 + dig)
                    else:
                        generate(idx + 1, num * 10 + d)

                generate(0, 0)
            if sum_primes > 0:
                total += sum_primes
                break
    print(total)


solve()
