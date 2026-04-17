"""
Problem 838: Not Coprime

Count pairs (a, b) with 1 <= a <= b <= N such that gcd(a, b) > 1.

Key identity: Phi(N) = sum_{d=1}^{N} mu(d) * floor(N/d)^2
counts ordered coprime pairs, then C(N) = N(N+1)/2 - (Phi(N)+1)/2.
"""

from math import gcd

# --- Method 1: Direct Mobius sieve ---
def mobius_sieve(n: int) -> list:
    """Compute mu(k) for k = 0..n via linear sieve."""
    mu = [0] * (n + 1)
    mu[1] = 1
    is_prime = [True] * (n + 1)
    primes = []
    for i in range(2, n + 1):
        if is_prime[i]:
            primes.append(i)
            mu[i] = -1
        for p in primes:
            if i * p > n:
                break
            is_prime[i * p] = False
            if i % p == 0:
                mu[i * p] = 0
                break
            else:
                mu[i * p] = -mu[i]
    return mu

def count_non_coprime_sieve(N: int):
    """Count non-coprime pairs using full Mobius sieve + block summation."""
    mu = mobius_sieve(N)
    # Prefix sums of mu (Mertens function)
    M = [0] * (N + 2)
    for i in range(1, N + 1):
        M[i] = M[i - 1] + mu[i]

    # Block summation: sum mu(d) * floor(N/d)^2
    phi_N = 0
    d = 1
    while d <= N:
        q = N // d
        # Find largest d2 with floor(N/d2) = q
        d2 = N // q
        phi_N += q * q * (M[d2] - M[d - 1])
        d = d2 + 1

    total_pairs = N * (N + 1) // 2
    coprime_pairs = (phi_N + 1) // 2
    return total_pairs - coprime_pairs

# --- Method 2: Brute force (for verification on small N) ---
def count_non_coprime_brute(N: int):
    """Brute force: enumerate all pairs and check gcd."""
    count = 0
    for a in range(1, N + 1):
        for b in range(a, N + 1):
            if gcd(a, b) > 1:
                count += 1
    return count

# --- Method 3: Euler totient summation ---
def count_non_coprime_totient(N: int):
    """Use Euler's totient: sum_{a=1}^{N} phi(a) = coprime pairs count (ordered, a<b) + N.
    Actually, sum_{a=1}^{N} phi(a) = (Phi(N) + 1) / 2 where Phi counts ordered coprime pairs.
    More precisely: number of (a,b) with 1<=b<=a<=N and gcd(a,b)=1 is sum phi(a).
    """
    # Compute phi via sieve
    phi = list(range(N + 1))
    for p in range(2, N + 1):
        if phi[p] == p:  # p is prime
            for m in range(p, N + 1, p):
                phi[m] -= phi[m] // p
    coprime_le = sum(phi[1:N + 1])  # sum phi(a) for a=1..N = coprime (b,a) with b<=a
    total_pairs = N * (N + 1) // 2
    return total_pairs - coprime_le

# --- Compute and verify ---
# Verify on small N
for n in range(1, 50):
    bf = count_non_coprime_brute(n)
    sv = count_non_coprime_sieve(n)
    tot = count_non_coprime_totient(n)
    assert bf == sv == tot, f"Mismatch at N={n}: brute={bf}, sieve={sv}, totient={tot}"

# Compute for target
N = 10_000_000
answer = count_non_coprime_sieve(N)
print(answer)
