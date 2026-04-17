"""
Problem 46: Goldbach's Other Conjecture

Find the smallest odd composite that cannot be written as p + 2k^2
where p is prime and k >= 1.
"""

def sieve(limit):
    is_prime = [True] * limit
    is_prime[0] = is_prime[1] = False
    for i in range(2, int(limit**0.5) + 1):
        if is_prime[i]:
            for j in range(i * i, limit, i):
                is_prime[j] = False
    return is_prime

def solve():
    LIMIT = 10000
    is_prime = sieve(LIMIT)

    for n in range(9, LIMIT, 2):
        if is_prime[n]:
            continue
        found = False
        k = 1
        while 2 * k * k < n:
            if is_prime[n - 2 * k * k]:
                found = True
                break
            k += 1
        if not found:
            return n

answer = solve()
assert answer == 5777
print(answer)
