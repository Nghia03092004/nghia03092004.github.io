"""
Problem 612: Friend Numbers
Count numbers whose digit permutations include at least one prime.
"""
from itertools import permutations

def is_prime(n):
    if n < 2: return False
    if n < 4: return True
    if n % 2 == 0 or n % 3 == 0: return False
    i = 5
    while i * i <= n:
        if n % i == 0 or n % (i + 2) == 0: return False
        i += 6
    return True

def has_prime_permutation(n):
    """Check if any permutation of digits of n is prime."""
    digits = str(n)
    for perm in set(permutations(digits)):
        if perm[0] != '0':
            num = int(''.join(perm))
            if is_prime(num):
                return True
    return False

def solve(N=1000):
    count = 0
    for n in range(2, N + 1):
        if has_prime_permutation(n):
            count += 1
    return count

# Test
assert has_prime_permutation(13)  # 13 and 31 are both prime
assert has_prime_permutation(23)  # 23 is prime

answer = solve(1000)
print(f"Friend numbers up to 1000: {answer}")
