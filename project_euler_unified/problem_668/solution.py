"""
Problem 668: Square Root Smooth Numbers

A positive integer n is square root smooth if all prime factors < sqrt(n).
Count such n <= N = 10^10 (including 1).

n is NOT square root smooth iff it has a prime factor p >= sqrt(n).
We count the complement.

For each n > 1, let p = P+(n) be largest prime factor.
n is not smooth iff p^2 >= n.

Write n = p * m where m = n/p. Then m < p (from p^2 >= n = pm => p > m,
actually p >= m, and if p = m then n = p^2, P+(n) = p = sqrt(n), not strictly less).

Case A: m = 1, i.e., n = p is prime. Count = pi(N).
Case B: m >= 2, m < p, all prime factors of m < p (auto since m < p).
  For each prime p: m from 2 to min(p-1, N//p).
  Number of valid m = min(p-1, N//p) - 1.

Answer = 1 + N - pi(N) - sum over primes p of (min(p-1, N//p) - 1)
where the sum is over primes 2 <= p <= N//2.

But wait: for p = 2, min(p-1, N//p) - 1 = min(1, 5*10^9) - 1 = 0.
That's correct since m must be >= 2 and < 2, impossible.

For p = 3: min(2, N//3) - 1 = 1. So m = 2, n = 6. Check: P+(6) = 3 >= sqrt(6) ~ 2.45. Yes.

Let me verify with N = 100, answer should be 100 - (100 - 29) = 29.
"""

def solve_small(N):
    """Brute force for verification."""
    count = 0
    for n in range(1, N + 1):
        if n == 1:
            count += 1
            continue
        # Find largest prime factor
        temp = n
        lpf = 0
        d = 2
        while d * d <= temp:
            while temp % d == 0:
                lpf = max(lpf, d)
                temp //= d
            d += 1
        if temp > 1:
            lpf = max(lpf, temp)
        if lpf * lpf < n:  # strictly less: lpf < sqrt(n)
            count += 1
    return count

# Verify: f(100) should be 29
print("f(100) =", solve_small(100))

def solve(N):
    import math
    sqrtN = int(math.isqrt(N))

    # Sieve primes up to sqrtN
    sN = sqrtN
    is_prime = [False, False] + [True] * (sN - 1)
    for i in range(2, int(math.isqrt(sN)) + 1):
        if is_prime[i]:
            for j in range(i*i, sN + 1, i):
                is_prime[j] = False

    # Lucy_Hedgehog prime counting
    small = list(range(-1, sN + 1))  # small[v] = v - 1 for v = 0..sN
    small[0] = 0
    large = [0] * (sN + 2)
    for k in range(1, sN + 1):
        large[k] = N // k - 1

    for p in range(2, sN + 1):
        if not is_prime[p]:
            continue
        pp = p * p
        pcnt = small[p - 1]

        # Update large
        for k in range(1, sN + 1):
            if k * pp > N:
                break
            val = N // (k * p)
            if val <= sN:
                sub = small[val]
            else:
                sub = large[k * p]
            large[k] -= sub - pcnt

        # Update small (from sN down to pp)
        for v in range(sN, pp - 1, -1):
            small[v] -= small[v // p] - pcnt

    def getPi(x):
        if x <= 0:
            return 0
        if x <= sN:
            return small[x]
        return large[N // x]

    piN = large[1]

    # Count non-smooth composites
    # For each prime p with 2 <= p <= N//2:
    #   contribution = min(p-1, N//p) - 1 (if >= 1, else 0)

    # Split: p <= sqrtN and p > sqrtN
    total_non_smooth_composite = 0

    # Part 1: primes p <= sqrtN
    # min(p-1, N//p) = p - 1 since N//p >= sqrtN >= p for p <= sqrtN
    # contribution = p - 2
    for p in range(2, sN + 1):
        if is_prime[p]:
            total_non_smooth_composite += p - 2

    # Part 2: primes sqrtN < p <= N//2
    # min(p-1, N//p) = N//p since N//p < sqrtN < p
    # contribution = N//p - 1
    # Group by q = N//p: primes in (N//(q+1), N//q], contribution = q - 1 each
    maxq = N // (sqrtN + 1)
    for q in range(2, maxq + 1):
        phi = N // q
        plo = N // (q + 1)
        if phi <= sqrtN:
            continue
        if plo < sqrtN:
            plo = sqrtN
        cnt = getPi(phi) - getPi(plo)
        if cnt > 0:
            total_non_smooth_composite += (q - 1) * cnt

    answer = 1 + N - piN - total_non_smooth_composite
    return answer

# Verify small case
print("f(100) via formula =", solve(100))

# Solve the actual problem
print(solve(10**10))
