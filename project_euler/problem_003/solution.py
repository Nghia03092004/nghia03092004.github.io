"""Problem 3: Largest Prime Factor"""

def largest_prime_factor(n: int) -> int:
    d = 2
    while d * d <= n:
        while n % d == 0:
            n //= d
        d += 1
    return n if n > 1 else d - 1

answer = largest_prime_factor(600851475143)
assert answer == 6857
print(answer)
