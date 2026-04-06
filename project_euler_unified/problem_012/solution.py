"""Project Euler Problem 12: Highly Divisible Triangular Number"""

def count_divisors(n):
    if n == 0:
        return 0
    count = 1
    d = 2
    while d * d <= n:
        e = 0
        while n % d == 0:
            n //= d
            e += 1
        count *= e + 1
        d += 1
    if n > 1:
        count *= 2
    return count

n = 1
while True:
    if n % 2 == 0:
        d = count_divisors(n // 2) * count_divisors(n + 1)
    else:
        d = count_divisors(n) * count_divisors((n + 1) // 2)
    if d > 500:
        print(n * (n + 1) // 2)
        break
    n += 1
