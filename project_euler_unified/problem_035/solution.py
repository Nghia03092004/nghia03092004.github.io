"""Project Euler Problem 35: Circular Primes"""

def sieve(limit):
    is_prime = [True] * limit
    is_prime[0] = is_prime[1] = False
    for i in range(2, int(limit**0.5) + 1):
        if is_prime[i]:
            for j in range(i * i, limit, i):
                is_prime[j] = False
    return is_prime

def main():
    N = 1_000_000
    is_prime = sieve(N)
    count = 0
    for p in range(2, N):
        if not is_prime[p]:
            continue
        s = str(p)
        k = len(s)
        if all(is_prime[int(s[i:] + s[:i])] for i in range(k)):
            count += 1
    print(count)  # 55

if __name__ == "__main__":
    main()
