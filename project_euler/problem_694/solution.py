"""
Project Euler Problem 694: Cube-full Divisors

S(n) = sum_{i=1}^{n} s(i) where s(i) counts cube-full divisors of i.
S(n) = sum over all cube-full c <= n of floor(n/c).

Enumerate cube-full numbers recursively using primes up to n^(1/3).
"""

def solve():
    N = 10**18
    limit = 10**6 + 1

    # Sieve of Eratosthenes
    is_prime = [True] * (limit + 1)
    is_prime[0] = is_prime[1] = False
    primes = []
    for i in range(2, limit + 1):
        if is_prime[i]:
            primes.append(i)
            for j in range(i * i, limit + 1, i):
                is_prime[j] = False

    answer = [0]

    def enumerate(cur, pidx):
        answer[0] += N // cur
        for i in range(pidx, len(primes)):
            p = primes[i]
            p3 = p * p * p
            if cur * p3 > N:
                break
            val = cur * p3
            while val <= N:
                enumerate(val, i + 1)
                if val > N // p:
                    break
                val *= p

    enumerate(1, 0)
    print(answer[0])

if __name__ == "__main__":
    solve()
