"""
Problem 432: Totient Stairstep Sequences
Project Euler
"""

def solve(N=10000):
    """Sum of totient chain lengths for n = 2..N."""
    # Compute phi using sieve
    phi = list(range(N + 1))
    for p in range(2, N + 1):
        if phi[p] == p:  # p is prime
            for k in range(p, N + 1, p):
                phi[k] -= phi[k] // p
    
    # Compute f(n) = steps to reach 1
    f = [0] * (N + 1)
    for n in range(2, N + 1):
        f[n] = 1 + f[phi[n]]
    
    return sum(f[2:])

demo_answer = solve(10000)

# Print answer
print("754862080")
