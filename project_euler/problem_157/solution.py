from math import gcd

def count_divisors(n):
    """Count the number of positive divisors of n."""
    if n <= 0:
        return 0
    count = 0
    d = 1
    while d * d <= n:
        if n % d == 0:
            count += 1
            if d != n // d:
                count += 1
        d += 1
    return count

def get_divisors(n2):
    """Get all divisors of 10^n2 = 2^n2 * 5^n2."""
    divs = []
    for a in range(n2 + 1):
        for b in range(n2 + 1):
            divs.append(2**a * 5**b)
    return sorted(divs)

total = 0
for n in range(1, 10):
    N = 10**n
    N2 = N * N
    divs = get_divisors(2 * n)
    subtotal = 0
    for u in divs:
        if u > N:
            continue  # ensures a <= b
        v = N2 // u
        g = gcd(N + u, N + v)
        subtotal += count_divisors(g)
    total += subtotal

print(total)
