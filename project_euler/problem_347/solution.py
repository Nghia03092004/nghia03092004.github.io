def solve():
    N = 10**7

    # Sieve of Eratosthenes
    is_prime = [False, False] + [True] * (N - 1)
    for i in range(2, int(N**0.5) + 1):
        if is_prime[i]:
            for j in range(i * i, N + 1, i):
                is_prime[j] = False

    primes = [i for i in range(2, N + 1) if is_prime[i]]

    total = 0
    for i, p in enumerate(primes):
        if p * p > N:
            break
        for j in range(i + 1, len(primes)):
            q = primes[j]
            if p * q > N:
                break

            best = 0
            pa = p
            while pa * q <= N:
                val = pa * q
                while val * q <= N:
                    val *= q
                best = max(best, val)
                pa *= p

            total += best

    print(total)

if __name__ == "__main__":
    solve()
