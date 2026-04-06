"""
Project Euler Problem 518: Prime Triples and Geometric Sequences

Find S(10^8) = sum of a+b+c over all prime triples (a,b,c)
with a<b<c<10^8 and a+1,b+1,c+1 forming a geometric sequence.
"""
import math

def solve():
    LIMIT = 10**8

    # Sieve of Eratosthenes
    is_prime = bytearray([1]) * (LIMIT + 1)
    is_prime[0] = is_prime[1] = 0
    for i in range(2, int(LIMIT**0.5) + 1):
        if is_prime[i]:
            is_prime[i*i::i] = bytearray(len(is_prime[i*i::i]))

    ans = 0
    # Enumerate x, k, y
    x = 2
    while x * x <= LIMIT:
        k = 1
        while k * x * x <= LIMIT:
            c1 = k * x * x
            if c1 - 1 >= 2 and is_prime[c1 - 1]:
                for y in range(1, x):
                    if x % 2 == 0 and y % 2 == 0:
                        continue
                    a1 = k * y * y
                    b1 = k * x * y
                    if a1 < 2 or b1 < 2 or a1 - 1 < 2:
                        continue
                    if not is_prime[a1 - 1]:
                        continue
                    if not is_prime[b1 - 1]:
                        continue
                    if math.gcd(x, y) != 1:
                        continue
                    ans += (a1 - 1) + (b1 - 1) + (c1 - 1)
            k += 1
        x += 1

    print(ans)

if __name__ == "__main__":
    solve()
