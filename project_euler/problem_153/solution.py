import math

def solve():
    N = 10**8

    # Part 1: sum_{d=1}^{N} d * floor(N/d) via hyperbola method
    part1 = 0
    d = 1
    while d <= N:
        q = N // d
        d_max = N // q
        part1 += q * (d + d_max) * (d_max - d + 1) // 2
        d = d_max + 1

    # Part 2: Gaussian integer contributions
    part2 = 0
    for b in range(1, int(math.isqrt(N)) + 1):
        if b * b + 1 > N:
            break
        for a in range(1, int(math.isqrt(N - b * b)) + 1):
            if math.gcd(a, b) != 1:
                continue
            norm = a * a + b * b
            if norm > N:
                break
            k = 1
            while k * norm <= N:
                q = N // (k * norm)
                k_max = N // (q * norm)
                s = (k + k_max) * (k_max - k + 1) // 2
                part2 += 2 * a * q * s
                k = k_max + 1

    print(part1 + part2)

solve()
