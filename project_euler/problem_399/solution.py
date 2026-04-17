"""
Project Euler Problem 399: Squarefree Fibonacci Numbers

Find the 100,000,000th squarefree Fibonacci number.
Answer format: last 16 digits, comma, scientific notation (1 decimal).

Key idea: Under Wall's conjecture, F(n) is NOT squarefree iff
there exists a prime p such that p * alpha(p) | n,
where alpha(p) is the rank of apparition (smallest k with p | F(k)).

Algorithm:
1. Sieve to find primes
2. For each prime p, compute alpha(p) via Fibonacci mod p
3. Mark all multiples of p*alpha(p) in a boolean array
4. Count squarefree indices until we reach the 10^8-th one
5. Compute F(k) mod 10^16 and log10(F(k))
"""

import math

def sieve_primes(limit):
    """Sieve of Eratosthenes returning list of primes up to limit."""
    is_prime = bytearray(b'\x01') * (limit + 1)
    is_prime[0] = is_prime[1] = 0
    for i in range(2, int(limit**0.5) + 1):
        if is_prime[i]:
            is_prime[i*i::i] = bytearray(len(is_prime[i*i::i]))
    return [i for i in range(2, limit + 1) if is_prime[i]]

def rank_of_apparition(p):
    """Find smallest k > 0 such that F(k) = 0 (mod p)."""
    a, b = 0, 1
    for k in range(1, p * p + 10):  # alpha(p) divides 2(p+1) at most
        a, b = b, (a + b) % p
        if a == 0:
            return k
    return -1  # Should not happen for valid primes

def fib_mod(n, mod):
    """Compute (F(n), F(n+1)) mod m using the doubling method."""
    if n == 0:
        return 0, 1
    a, b = fib_mod(n >> 1, mod)
    # F(2k) = F(k) * (2*F(k+1) - F(k))
    # F(2k+1) = F(k)^2 + F(k+1)^2
    c = a * ((2 * b - a) % mod) % mod
    d = (a * a + b * b) % mod
    if n & 1:
        return d, (c + d) % mod
    else:
        return c, d

def log10_fib(n):
    """Compute log10(F(n)) using Binet's formula approximation.
    For large n: F(n) ~ phi^n / sqrt(5)
    log10(F(n)) ~ n * log10(phi) - 0.5 * log10(5)
    """
    phi = (1 + math.sqrt(5)) / 2
    return n * math.log10(phi) + math.log10(1 / math.sqrt(5))

def solve(target=100_000_000):
    """Find the target-th squarefree Fibonacci number."""
    # Estimate sieve size: ~76.47% of Fibonacci numbers are squarefree
    # Need about target / 0.7647 indices
    N = int(target / 0.76) + 1000
    # Add safety margin
    N = max(N, target + target // 3)

    print(f"Sieve size N = {N}")
    print(f"Target: {target}-th squarefree Fibonacci number")

    # Step 1: Generate primes
    # We need primes p where p * alpha(p) <= N
    # alpha(p) >= 1, so p <= N. But alpha(p) >= (p-1)/4 roughly,
    # so p * (p-1)/4 <= N => p <= ~2*sqrt(N)
    # To be safe, generate primes up to sqrt(2*N) + extra
    prime_limit = int(math.sqrt(2 * N)) + 1000
    # But small primes matter most. Also alpha(p) can be small for some p.
    # Actually alpha(p) divides p-1 or 2(p+1), so alpha(p) <= 2(p+1)
    # So p * alpha(p) <= p * 2(p+1) ~ 2p^2
    # For p*alpha(p) <= N: p <= sqrt(N/2) ~ sqrt(N)
    # But alpha(p) can be much smaller than p for large primes.
    # However, for p > sqrt(N), even alpha(p)=1 gives p*1=p > sqrt(N),
    # and we'd only mark ~N/p < sqrt(N) entries. Still need to consider.
    # Let's be safe and go up to N (but that's too many primes).
    # Actually: if p > N, then p*alpha(p) > N, no multiples in range.
    # If p <= N, p*alpha(p) could be <= N only if alpha(p) is small enough.
    # For most primes p, alpha(p) ~ p, so p*alpha(p) ~ p^2.
    # Only need primes up to ~sqrt(N).
    # But some primes have small alpha(p), e.g., alpha(p)=1 never happens (F(1)=1).
    # alpha(p) >= 2 for p >= 2. So p*alpha(p) >= 2p, need p <= N/2.
    # To be truly safe, use N/2 as limit, but compute alpha and skip if p*alpha > N.
    prime_limit = N // 2 + 1
    print(f"Generating primes up to {prime_limit}...")
    primes = sieve_primes(prime_limit)
    print(f"Found {len(primes)} primes")

    # Step 2: Sieve non-squarefree indices
    # not_squarefree[i] = True means F(i) is NOT squarefree
    not_squarefree = bytearray(N + 1)

    print("Computing ranks of apparition and sieving...")
    primes_used = 0
    for p in primes:
        alpha = rank_of_apparition(p)
        q = p * alpha  # period for p^2 dividing F(n)
        if q > N:
            continue
        primes_used += 1
        # Mark all multiples of q
        for j in range(q, N + 1, q):
            not_squarefree[j] = 1

    print(f"Primes contributing to sieve: {primes_used}")

    # Step 3: Count squarefree indices to find the target-th one
    count = 0
    target_index = -1
    for i in range(1, N + 1):
        if not not_squarefree[i]:
            count += 1
            if count == target:
                target_index = i
                break

    if target_index == -1:
        print(f"ERROR: Sieve too small! Only found {count} squarefree Fibonacci numbers in range 1..{N}")
        return None

    print(f"The {target}-th squarefree Fibonacci number is F({target_index})")

    # Step 4: Compute F(target_index) mod 10^16
    MOD = 10**16
    fib_val, _ = fib_mod(target_index, MOD)
    last_16 = f"{fib_val:016d}"
    print(f"Last 16 digits: {last_16}")

    # Step 5: Compute scientific notation
    log_val = log10_fib(target_index)
    mantissa = 10 ** (log_val - int(log_val))
    exponent = int(log_val)
    sci_notation = f"{mantissa:.1f}e{exponent}"
    print(f"Scientific notation: {sci_notation}")

    answer = f"{last_16},{sci_notation}"
    print(f"\nAnswer: {answer}")

    return target_index, count, not_squarefree

def solve_small(target=200):
    """Solve for the 200th squarefree Fibonacci number (verification)."""
    N = 500
    primes = sieve_primes(N)
    not_squarefree = bytearray(N + 1)

    for p in primes:
        alpha = rank_of_apparition(p)
        q = p * alpha
        if q > N:
            continue
        for j in range(q, N + 1, q):
            not_squarefree[j] = 1

    count = 0
    for i in range(1, N + 1):
        if not not_squarefree[i]:
            count += 1
            if count == target:
                print(f"The {target}-th squarefree Fibonacci number is F({i})")
                # Compute it exactly for small case
                a, b = 0, 1
                for _ in range(i):
                    a, b = b, a + b
                fib_val = a
                print(f"F({i}) = {fib_val}")
                last_16 = str(fib_val)[-16:]
                print(f"Last 16 digits: {last_16}")
                log_val = log10_fib(i)
                mantissa = 10 ** (log_val - int(log_val))
                exponent = int(log_val)
                print(f"Scientific notation: {mantissa:.1f}e{exponent}")
                return i, fib_val
    return None

def create_visualization(not_squarefree=None):
    """Create visualization of squarefree Fibonacci number properties."""
