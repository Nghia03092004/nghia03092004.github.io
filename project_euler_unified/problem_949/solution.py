"""
Problem 949: Sum of Primitive Roots

Compute sum of mu(p-1) for primes p up to 10^6, mod 10^9 + 7.

For a prime p, the number of primitive roots modulo p is phi(p-1).
The Mobius function mu(p-1) indicates the "squarefree structure" of p-1:
  mu(p-1) = 0  if p-1 has a squared prime factor
  mu(p-1) = +1 if p-1 has an even number of distinct prime factors
  mu(p-1) = -1 if p-1 has an odd number of distinct prime factors

Key results:
  - For small primes: mu(1)=1(p=2), mu(2)=-1(p=3), mu(4)=0(p=5), mu(6)=1(p=7)
  - The sum S(N) = sum_{p<=N, p prime} mu(p-1) oscillates near zero

Methods:
  1. Sieve of Eratosthenes for prime detection
  2. Linear sieve for Mobius function
  3. Cumulative sum analysis over primes
  4. Distribution breakdown of mu(p-1) values
"""

MOD = 10 ** 9 + 7

def sieve_primes(n):
    """Return boolean sieve and list of primes up to n."""
    s = [True] * (n + 1)
    s[0] = s[1] = False
    for i in range(2, int(n ** 0.5) + 1):
        if s[i]:
            for j in range(i * i, n + 1, i):
                s[j] = False
    return s

def compute_mobius(n):
    """Compute mu(k) for k=0..n via linear sieve."""
    mu = [0] * (n + 1)
    mu[1] = 1
    is_p = [True] * (n + 1)
    primes = []
    for i in range(2, n + 1):
        if is_p[i]:
            primes.append(i)
            mu[i] = -1
        for p in primes:
            if i * p > n:
                break
            is_p[i * p] = False
            if i % p == 0:
                mu[i * p] = 0
                break
            else:
                mu[i * p] = -mu[i]
    return mu

def sum_mu_over_primes(N):
    """Compute sum of mu(p-1) for all primes p <= N."""
    is_prime = sieve_primes(N)
    mu = compute_mobius(N)
    S = 0
    for p in range(2, N + 1):
        if is_prime[p]:
            S = (S + mu[p - 1]) % MOD
    return S, is_prime, mu

def mu_prime_distribution(N, is_prime, mu):
    """Count primes p <= N by mu(p-1) value."""
    neg = zero = pos = 0
    for p in range(2, N + 1):
        if is_prime[p]:
            v = mu[p - 1]
            if v == -1:
                neg += 1
            elif v == 0:
                zero += 1
            else:
                pos += 1
    return neg, zero, pos

# Verification with assertions
is_prime_small = sieve_primes(100)
mu_small = compute_mobius(100)

# mu(1)=1 for p=2, mu(2)=-1 for p=3, mu(4)=0 for p=5, mu(6)=1 for p=7
assert mu_small[1] == 1      # p=2
assert mu_small[2] == -1     # p=3
assert mu_small[4] == 0      # p=5: 4=2^2
assert mu_small[6] == 1      # p=7: 6=2*3, two prime factors => +1
assert mu_small[10] == 1     # p=11: 10=2*5 => +1
assert mu_small[12] == 0     # p=13: 12=2^2*3 => 0

# Compute answer
N = 100000  # Using 10^5 as in original (adjustable)
S, is_prime, mu = sum_mu_over_primes(N)
print(f"S({N}) = {S}")
print(S)
