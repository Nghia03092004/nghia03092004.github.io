import math

def sieve(limit):
    is_prime = bytearray([1]) * (limit + 1)
    is_prime[0] = is_prime[1] = 0
    for i in range(2, int(limit**0.5) + 1):
        if is_prime[i]:
            is_prime[i*i::i] = bytearray(len(is_prime[i*i::i]))
    return is_prime

def reverse_num(n):
    return int(str(n)[::-1])

def is_palindrome(n):
    s = str(n)
    return s == s[::-1]

def solve():
    LIMIT = 10**8
    is_prime = sieve(LIMIT)

    results = []

    for p in range(2, LIMIT + 1):
        if not is_prime[p]:
            continue
        sq = p * p
        if is_palindrome(sq):
            continue
        rev = reverse_num(sq)
        sr = int(math.isqrt(rev))
        if sr * sr != rev:
            continue
        if sr <= 1 or sr > LIMIT:
            continue
        if not is_prime[sr]:
            continue
        results.append(sq)

    results.sort()

    if len(results) >= 50:
        answer = sum(results[:50])
        print(answer)
    else:
        print(f"Found only {len(results)} reversible prime squares")
        print(f"Partial sum: {sum(results)}")

if __name__ == "__main__":
    solve()
