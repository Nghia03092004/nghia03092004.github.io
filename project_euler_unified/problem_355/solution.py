"""
Problem 355: Maximal Coprime Subset

S(n) = 1 + sum over primes p <= n of p^floor(log_p(n))

The maximum-sum pairwise coprime subset of {1,...,n} is {1} union
{p^floor(log_p(n)) for each prime p <= n}.

We compute sum_{n=1}^{N} S(n) where N = 10^5.

For each prime p and power k, p^k contributes to S(n) for
n in [p^k, min(p^{k+1}-1, N)].
"""

def solve():
    N = 100000  # 10^5

    # Sieve of Eratosthenes
    is_prime = [True] * (N + 1)
    is_prime[0] = is_prime[1] = False
    for i in range(2, int(N**0.5) + 1):
        if is_prime[i]:
            for j in range(i*i, N + 1, i):
                is_prime[j] = False

    total = N  # contribution of 1 for each n

    for p in range(2, N + 1):
        if not is_prime[p]:
            continue

        pk = p  # p^k, k=1
        while pk <= N:
            next_pk = pk * p  # p^{k+1}
            if next_pk - 1 <= N and next_pk > pk:
                upper = next_pk - 1
            else:
                upper = N

            count = upper - pk + 1
            total += pk * count

            if pk > N // p:
                break
            pk *= p

    print(total)

if __name__ == "__main__":
    solve()
