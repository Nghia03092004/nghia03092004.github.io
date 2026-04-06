def solve():
    N = 40_000_000

    # Sieve for Euler's totient
    phi = list(range(N))
    for i in range(2, N):
        if phi[i] == i:  # i is prime
            for j in range(i, N, i):
                phi[j] = phi[j] // i * (i - 1)

    # Compute chain lengths
    chain = [0] * N
    chain[1] = 1
    for i in range(2, N):
        chain[i] = 1 + chain[phi[i]]

    # Sum primes with chain length 25
    answer = 0
    for i in range(2, N):
        if phi[i] == i - 1 and chain[i] == 25:
            answer += i

    print(answer)

if __name__ == "__main__":
    solve()
