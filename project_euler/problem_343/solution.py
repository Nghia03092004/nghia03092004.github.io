"""
Problem 343: Fractional Sequences

For primes 5 <= p < 10^6, compute f(p^2) and sum them.

The problem defines a process on an integer n:
  - Find the greatest prime factor g of n
  - Replace n with n/g + g - 1
  - Repeat until n is prime
  - f(original_n) = final prime

For n = p^2 (p prime):
  - gpf(p^2) = p
  - p^2 -> p^2/p + p - 1 = 2p - 1
  - If 2p-1 is prime, f(p^2) = 2p-1
  - Otherwise continue the process on 2p-1

Answer: 269533451410884183
"""

def solve():
    LIMIT = 1000000

    # Sieve of Eratosthenes
    is_prime = [True] * (2 * LIMIT + 2)
    is_prime[0] = is_prime[1] = False
    for i in range(2, int((2 * LIMIT + 1) ** 0.5) + 1):
        if is_prime[i]:
            for j in range(i * i, 2 * LIMIT + 2, i):
                is_prime[j] = False

    # Smallest prime factor sieve for quick factorization
    spf = list(range(2 * LIMIT + 2))
    for i in range(2, int((2 * LIMIT + 1) ** 0.5) + 1):
        if spf[i] == i:  # i is prime
            for j in range(i * i, 2 * LIMIT + 2, i):
                if spf[j] == j:
                    spf[j] = i

    def greatest_prime_factor(n):
        g = 1
        while n > 1:
            if n < len(spf):
                p = spf[n]
                g = max(g, p)
                while n % p == 0:
                    n //= p
            else:
                d = 2
                while d * d <= n:
                    if n % d == 0:
                        g = max(g, d)
                        while n % d == 0:
                            n //= d
                    d += 1
                if n > 1:
                    g = max(g, n)
                break
        return g

    def is_pr(n):
        if n < len(is_prime):
            return is_prime[n]
        if n < 2:
            return False
        if n % 2 == 0:
            return n == 2
        d = 3
        while d * d <= n:
            if n % d == 0:
                return False
            d += 2
        return True

    def f(n):
        while not is_pr(n):
            g = greatest_prime_factor(n)
            n = n // g + g - 1
        return n

    total = 0
    for p in range(5, LIMIT):
        if not is_prime[p]:
            continue
        # f(p^2): gpf(p^2) = p, first step gives 2p-1
        val = f(2 * p - 1)
        total += val

    print(total)

if __name__ == "__main__":
    solve()
