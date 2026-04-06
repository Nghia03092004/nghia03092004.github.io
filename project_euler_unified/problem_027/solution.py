def solve():
    LIMIT = 1000000
    is_prime = [True] * (LIMIT + 1)
    is_prime[0] = is_prime[1] = False
    for i in range(2, int(LIMIT**0.5) + 1):
        if is_prime[i]:
            for j in range(i * i, LIMIT + 1, i):
                is_prime[j] = False

    def check_prime(n):
        if n < 2:
            return False
        if n < len(is_prime):
            return is_prime[n]
        for i in range(2, int(n**0.5) + 1):
            if n % i == 0:
                return False
        return True

    primes_b = [p for p in range(2, 1001) if is_prime[p]]
    best_a, best_b, best_count = 0, 0, 0

    for b in primes_b:
        for a in range(-999, 1000):
            n = 0
            while check_prime(n * n + a * n + b):
                n += 1
            if n > best_count:
                best_count = n
                best_a, best_b = a, b

    print(best_a * best_b)

if __name__ == "__main__":
    solve()
