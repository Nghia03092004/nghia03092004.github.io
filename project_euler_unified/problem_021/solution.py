def sum_proper_divisors(n):
    if n <= 1:
        return 0
    s = 1
    i = 2
    while i * i <= n:
        if n % i == 0:
            s += i
            if i != n // i:
                s += n // i
        i += 1
    return s

def solve():
    LIMIT = 10000
    total = 0
    for a in range(2, LIMIT):
        b = sum_proper_divisors(a)
        if b != a and b > 0 and sum_proper_divisors(b) == a:
            total += a
    print(total)
