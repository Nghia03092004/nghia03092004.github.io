"""
Problem 245: Coresilience

Find the sum of all composite n (up to ~10^10) such that (n - phi(n) - 1) | (n - 1).
Equivalently, C(n) = (n - phi(n) - 1)/(n - 1) is a unit fraction 1/k.

Key cases:
1) n = p^2: Always works since C(p^2) = 1/(p+1).
2) n = pq (semiprime): Need (p+q-2) | (p-1)^2.
3) n = p^2*q: Need (p(q+p-1)-1) | (p^2*q - 1).
4) n = pqr: Need (pq+pr+qr-p-q-r) | (pqr-1).
5) Other forms with more prime factors or higher powers.

Answer: 288084712410001
"""

from sympy import primerange, isprime
import math

LIMIT = 10**10

primes = list(primerange(2, 10**6 + 1))
found = set()

# Case 1: n = p^2
for p in primes:
    n = p * p
    if n > LIMIT:
        break
    found.add(n)

# Also larger primes for p^2
p_max = int(math.isqrt(LIMIT))
for p in range(primes[-1] + 2, p_max + 1, 2):
    if isprime(p):
        n = p * p
        if n <= LIMIT:
            found.add(n)

print(f"After p^2: {len(found)} solutions")

# Case 2: n = pq, p < q primes, (p+q-2) | (p-1)^2
def get_divisors(n):
    divs = []
    for i in range(1, int(math.isqrt(n)) + 1):
        if n % i == 0:
            divs.append(i)
            if i != n // i:
                divs.append(n // i)
    return divs

for p in primes:
    if p * (p + 1) > LIMIT:
        break
    pm1_sq = (p - 1) * (p - 1)
    if pm1_sq == 0:
        continue
    for d in get_divisors(pm1_sq):
        q = d - p + 2
        if q <= p:
            continue
        if p * q > LIMIT:
            continue
        if not isprime(q):
            continue
        n = p * q
        # Verify
        s = p + q - 2
        t = n - 1
        if t % s == 0:
            found.add(n)

print(f"After pq: {len(found)} solutions")

# Case 3: n = p^2 * q
for p in primes:
    if p * p * 2 > LIMIT:
        break
    max_q = LIMIT // (p * p)
    for q in primes:
        if q > max_q:
            break
        if q == p:
            continue
        n = p * p * q
        if n > LIMIT:
            break
        s = p * (q + p - 1) - 1
        t = n - 1
        if s > 0 and t % s == 0:
            found.add(n)

print(f"After p^2*q: {len(found)} solutions")

# Case 4: n = pqr (three distinct primes)
for i, p in enumerate(primes):
    if p * p * p > LIMIT:
        break
    for j in range(i + 1, len(primes)):
        q = primes[j]
        if p * q * q > LIMIT:
            break
        max_r = LIMIT // (p * q)
        for k in range(j + 1, len(primes)):
            r = primes[k]
            if r > max_r:
                break
            n = p * q * r
            if n > LIMIT:
                break
            s = p*q + p*r + q*r - p - q - r
            t = n - 1
            if s > 0 and t % s == 0:
                found.add(n)

print(f"After pqr: {len(found)} solutions")

# Case 5: n = p^2 * q^2
for i, p in enumerate(primes):
    if p * p * 4 > LIMIT:
        break
    for j in range(i + 1, len(primes)):
        q = primes[j]
        n = p * p * q * q
        if n > LIMIT:
            break
        s = p * q * (p + q - 1) - 1
        t = n - 1
        if s > 0 and t % s == 0:
            found.add(n)

print(f"After p^2*q^2: {len(found)} solutions")

# Case 6: n = p^2 * q * r
for i, p in enumerate(primes):
    if p * p * 6 > LIMIT:
        break
    for j in range(len(primes)):
        q = primes[j]
        if q == p:
            continue
        if p * p * q * (q + 1) > LIMIT:
            break
        for k in range(j + 1, len(primes)):
            r = primes[k]
            if r == p:
                continue
            n = p * p * q * r
            if n > LIMIT:
                break
            # phi(n) = p*(p-1)*(q-1)*(r-1) if p,q,r distinct
            phi_n = p * (p-1) * (q-1) * (r-1)
            s = n - phi_n - 1
            t = n - 1
            if s > 0 and t % s == 0:
                found.add(n)

print(f"After p^2*q*r: {len(found)} solutions")

total = sum(found)
print(f"\nTotal solutions found: {len(found)}")
print(f"Sum = {total}")
print(total)
