def solve():
    N = 10**12

    # Compute floor(sqrt(N)) precisely
    sqrtN = int(N**0.5)
    while sqrtN * sqrtN > N:
        sqrtN -= 1
    while (sqrtN + 1) * (sqrtN + 1) <= N:
        sqrtN += 1

    # Lucy Hedgehog prime counting method
    # small[i] = pi(i) for i <= sqrtN
    # large[i] = pi(N // i) for 1 <= i <= sqrtN
    small = list(range(-1, sqrtN + 1))
    small[0] = 0
    large = [0] * (sqrtN + 1)
    for i in range(1, sqrtN + 1):
        large[i] = N // i - 1

    sieve = [True] * (sqrtN + 1)
    primes = []

    for p in range(2, sqrtN + 1):
        if not sieve[p]:
            continue
        primes.append(p)
        if p * p > N:
            continue
        pcnt = small[p - 1]
        for i in range(1, sqrtN + 1):
            if i * p * p > N:
                break
            d = i * p
            if d <= sqrtN:
                large[i] -= large[d] - pcnt
            else:
                large[i] -= small[N // d] - pcnt
        for i in range(sqrtN, p * p - 1, -1):
            small[i] -= small[i // p] - pcnt

    def pi(x):
        if x <= 0:
            return 0
        if x <= sqrtN:
            return small[x]
        idx = N // x
        if idx <= sqrtN:
            return large[idx]
        return 0

    ans = 0

    # Case 1: p^7 <= N
    for p in primes:
        if p**7 > N:
            break
        ans += 1

    # Case 2: p^3 * q <= N, p != q
    for p in primes:
        p3 = p * p * p
        if p3 >= N:
            break
        qlim = N // p3
        cnt = pi(qlim)
        if p <= qlim:
            cnt -= 1
        ans += cnt

    # Case 3: p < q < r, p*q*r <= N
    for i, p in enumerate(primes):
        if p * p * p >= N:
            break
        for j in range(i + 1, len(primes)):
            q = primes[j]
            pq = p * q
            if pq * (q + 1) > N:
                break
            rlim = N // pq
            cnt = pi(rlim) - pi(q)
            if cnt > 0:
                ans += cnt

    print(ans)

solve()
