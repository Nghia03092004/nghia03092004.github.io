"""
Problem 618: Numbers with a Given Prime Factor Sum
f(n) = sum of numbers whose prime factors (with multiplicity) sum to n.
S(k) = sum of f(p_i) for i=1..k where p_i is the i-th prime.
"""

def sieve(n):
    is_prime = [True] * (n + 1)
    is_prime[0] = is_prime[1] = False
    for i in range(2, int(n**0.5) + 1):
        if is_prime[i]:
            for j in range(i*i, n + 1, i):
                is_prime[j] = False
    return [i for i in range(2, n + 1) if is_prime[i]]

def compute_f(N, MOD=10**9+7):
    """f(n) = sum of all m such that sum of prime factors of m (with mult) = n.
    DP approach: dp[n] = sum of all numbers whose prime factor sum is n.
    """
    primes = sieve(N)
    dp = [0] * (N + 1)
    dp[0] = 1  # empty product = 1
    
    for p in primes:
        for n in range(N, p - 1, -1):
            # Add p as a factor: contributes p to the sum, multiplies the number by p
            # Actually we need to be more careful:
            # dp[n] accumulates the sum of all products
            # When we add prime p with sum contribution p:
            # new_dp[n] += dp[n-p] * p (each existing number gets multiplied by p)
            pass
    
    # Simpler approach for small N:
    # dp[n] = sum of all integers whose prime factor sum = n
    dp = [0] * (N + 1)
    dp[0] = 1  # multiplicative identity
    
    for p in primes:
        if p > N:
            break
        # Unbounded: each prime can be used multiple times
        for n in range(p, N + 1):
            dp[n] = (dp[n] + dp[n - p] * p) % MOD
    
    return dp

MOD = 10**9 + 7
N = 100
dp = compute_f(N, MOD)

# f(2) should be 2 (only 2 itself)
# f(4) should be 2+4=6? No: numbers with factor sum 4: 4=2^2 (sum=2+2=4). So f(4)=4.
# f(3) = 3 (only 3). f(5) = 5+6=11? 5 (factor sum 5), 6=2*3 (sum 2+3=5). So f(5)=5+6=11.
print(f"f(2) = {dp[2]}")
print(f"f(3) = {dp[3]}")
print(f"f(4) = {dp[4]}")
print(f"f(5) = {dp[5]}")

primes = sieve(N)
S = 0
for p in primes:
    if p <= N:
        S = (S + dp[p]) % MOD
print(f"S(sum of f(p) for primes p <= {N}) = {S}")
