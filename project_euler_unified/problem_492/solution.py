"""
Problem 492: Totient Chains
Iterated Euler totient: phi^(k)(n) until reaching 1.
L(n) = chain length (steps to reach 1).
Compute sum of L(n) for n = 2..N.
"""

def totient_sieve(limit: int) -> list:
    """Compute phi(n) for n = 0..limit."""
    phi = list(range(limit + 1))
    for p in range(2, limit + 1):
        if phi[p] == p:  # p is prime
            for m in range(p, limit + 1, p):
                phi[m] = phi[m] // p * (p - 1)
    return phi

def chain_lengths(limit: int):
    """Compute L(n) = totient chain length for n = 0..limit.
    Returns (phi, L) arrays."""
    phi = totient_sieve(limit)
    L = [0] * (limit + 1)
    for n in range(2, limit + 1):
        L[n] = 1 + L[phi[n]]
    return phi, L

def totient_chain(n: int, phi: list) -> list:
    """Return the full totient chain starting at n."""
    chain = [n]
    while n > 1:
        n = phi[n]
        chain.append(n)
    return chain

def solve(limit: int):
    """Sum of L(n) for n = 2..limit."""
    _, L = chain_lengths(limit)
    return sum(L[2:])

# Compute and display
limit = 1000
phi, L = chain_lengths(limit)

print("Totient chains for small n:")
for n in range(2, 21):
    chain = totient_chain(n, phi)
    print(f"  n={n:2d}: chain = {' -> '.join(map(str, chain))}, L({n}) = {L[n]}")

answer_100 = sum(L[2:101])
answer_1000 = sum(L[2:1001])
print(f"\nSum of L(n) for n=2..100:  {answer_100}")
print(f"Sum of L(n) for n=2..1000: {answer_1000}")
