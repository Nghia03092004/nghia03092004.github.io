"""
Problem 351: Hexagonal Orchards

H(n) = 3*n*(n+1) - 6 * sum(phi(k) for k in 1..n)

We sieve Euler's totient function up to n = 10^8.
"""

def solve():
    N = 100000000  # 10^8

    # Sieve for Euler's totient function
    phi = list(range(N + 1))

    for i in range(2, N + 1):
        if phi[i] == i:  # i is prime
            for j in range(i, N + 1, i):
                phi[j] -= phi[j] // i

    # Compute sum of phi[k] for k = 1..N
    phi_sum = sum(phi[1:])

    # H(n) = 3*n*(n+1) - 6 * phi_sum
    total = 3 * N * (N + 1)
    hidden = total - 6 * phi_sum

    print(hidden)

if __name__ == "__main__":
    solve()
