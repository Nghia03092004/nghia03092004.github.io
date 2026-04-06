"""
Problem 204: Generalised Hamming Numbers
Count 100-smooth numbers up to 10^9.
"""

def primes_up_to(n):
    sieve = [True] * (n + 1)
    sieve[0] = sieve[1] = False
    for i in range(2, int(n**0.5) + 1):
        if sieve[i]:
            for j in range(i*i, n + 1, i):
                sieve[j] = False
    return [i for i in range(2, n + 1) if sieve[i]]

def count_smooth(primes, limit):
    """Count B-smooth numbers up to limit by recursive DFS."""
    def dfs(idx, cur):
        if idx == len(primes):
            return 1
        count = 0
        p = primes[idx]
        power = 1
        while cur * power <= limit:
            count += dfs(idx + 1, cur * power)
            power *= p
        return count
    return dfs(0, 1)

def count_smooth_brute(B, limit):
    """Brute force: check each number for B-smoothness."""
    count = 0
    for n in range(1, limit + 1):
        temp = n
        for p in primes_up_to(B):
            while temp % p == 0:
                temp //= p
        if temp == 1:
            count += 1
    return count

# Cross-check
p100 = primes_up_to(100)
assert len(p100) == 25
assert count_smooth_brute(5, 100) == count_smooth(primes_up_to(5), 100)

answer = count_smooth(p100, 10**9)
assert answer == 2944730, f"Expected 2944730, got {answer}"
print(answer)
