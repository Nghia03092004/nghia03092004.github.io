"""Project Euler Problem 37: Truncatable Primes"""

def solve():
    LIMIT = 1_000_000
    is_prime = [True] * LIMIT
    is_prime[0] = is_prime[1] = False
    for i in range(2, int(LIMIT**0.5) + 1):
        if is_prime[i]:
            for j in range(i * i, LIMIT, i):
                is_prime[j] = False

    def is_right_truncatable(n):
        while n >= 10:
            n //= 10
            if not is_prime[n]: return False
        return is_prime[n]

    def is_left_truncatable(n):
        p = 10
        while p < n:
            if not is_prime[n % p]: return False
            p *= 10
        return True

    total, count = 0, 0
    for i in range(11, LIMIT):
        if is_prime[i] and is_right_truncatable(i) and is_left_truncatable(i):
            total += i
            count += 1
            if count == 11: break
    return total

answer = solve()
assert answer == 748317
print(answer)
