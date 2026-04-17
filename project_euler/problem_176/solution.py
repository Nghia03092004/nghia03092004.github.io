"""
Problem 176: Right-angled Triangles That Share a Cathetus

Find the smallest integer that is a leg of exactly 47547 right triangles.

For odd leg a: f(a) = (prod(2*ei+1) - 1) / 2
For even leg a = 2^e0 * m: f(a) = ((2*e0-1)*prod(2*ei+1) - 1) / 2

We need the product = 95095 = 5 * 7 * 11 * 13 * 19.
Enumerate all multiplicative partitions into factors >= 2 (odd factors since 95095 is odd).
"""

TARGET = 95095

def get_divisors(n):
    divs = []
    i = 1
    while i * i <= n:
        if n % i == 0:
            divs.append(i)
            if i != n // i:
                divs.append(n // i)
        i += 1
    return sorted(divs)

divisors = [d for d in get_divisors(TARGET) if d >= 2]

partitions = []

def gen_partitions(n, min_f, current):
    if n == 1:
        partitions.append(current[:])
        return
    for d in divisors:
        if d < min_f:
            continue
        if d > n:
            break
        if n % d != 0:
            continue
        current.append(d)
        gen_partitions(n // d, d, current)
        current.pop()

gen_partitions(TARGET, 2, [])

primes = [2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47]

best = float('inf')

for partition in partitions:
    # Case 1: odd a - all factors map to exponents for odd primes
    exps = sorted([(f - 1) // 2 for f in partition], reverse=True)
    a_val = 1
    for i, e in enumerate(exps):
        a_val *= primes[1 + i] ** e  # odd primes starting from 3
    if a_val < best:
        best = a_val

    # Case 2: even a - one factor becomes (2*e0-1), rest map to odd prime exponents
    seen = set()
    for idx in range(len(partition)):
        if partition[idx] in seen:
            continue
        seen.add(partition[idx])
        f2 = partition[idx]
        e0 = (f2 + 1) // 2  # 2*e0-1 = f2 => e0 = (f2+1)/2

        remaining = sorted([(partition[j] - 1) // 2 for j in range(len(partition))
                            if j != idx], reverse=True)
        # Remove zero exponents
        remaining = [e for e in remaining if e > 0]

        a_val = 2 ** e0
        for i, e in enumerate(remaining):
            a_val *= primes[1 + i] ** e
        if a_val < best:
            best = a_val

print(best)
