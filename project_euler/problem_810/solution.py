"""
Problem 810: XOR-Primes
Define XOR-multiplication $\otimes$ via polynomial multiplication over $GF(2)$. A number $n > 1$ is XOR-prime if it cannot be written as $a \otimes b$ with $a,b > 1$. Find the sum of all XOR-primes up to $5 \times 10^6$.
"""

def solve(N=100000):
    """Find sum of XOR-primes up to N using sieve."""
    is_prime = [True] * (N + 1)
    is_prime[0] = is_prime[1] = False

    def xor_mul(a, b):
        result = 0
        while b:
            if b & 1:
                result ^= a
            a <<= 1
            b >>= 1
        return result

    for i in range(2, N + 1):
        if not is_prime[i]:
            continue
        # Mark multiples
        for j in range(i, N + 1):
            prod = xor_mul(i, j)
            if prod > N:
                break
            if prod > i:
                is_prime[prod] = False

    return sum(i for i in range(2, N + 1) if is_prime[i])

answer = solve()
print(answer)
