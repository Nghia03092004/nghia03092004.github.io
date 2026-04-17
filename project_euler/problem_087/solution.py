def solve():
    """
    Count numbers below 50,000,000 expressible as p^2 + q^3 + r^4
    where p, q, r are primes.
    """
    LIMIT = 50_000_000

    # Sieve of Eratosthenes
    sieve_limit = 7072
    is_prime = [True] * (sieve_limit + 1)
    is_prime[0] = is_prime[1] = False
    for i in range(2, int(sieve_limit**0.5) + 1):
        if is_prime[i]:
            for j in range(i * i, sieve_limit + 1, i):
                is_prime[j] = False

    primes = [i for i in range(2, sieve_limit + 1) if is_prime[i]]

    seen = set()

    for r in primes:
        r4 = r ** 4
        if r4 >= LIMIT:
            break
        for q in primes:
            q3 = q ** 3
            if r4 + q3 >= LIMIT:
                break
            for p in primes:
                p2 = p ** 2
                s = r4 + q3 + p2
                if s >= LIMIT:
                    break
                seen.add(s)

    print(len(seen))

if __name__ == "__main__":
    solve()
