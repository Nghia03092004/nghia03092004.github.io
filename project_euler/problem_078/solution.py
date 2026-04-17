"""
Problem 78: Coin Partitions

Find the least value of n for which p(n) is divisible by one million.
Answer: 55374
"""


def main():
    MOD = 1_000_000
    MAXN = 100_000

    # Precompute generalized pentagonal numbers with signs
    pentagonals = []
    for k in range(1, 1000):
        g1 = k * (3 * k - 1) // 2
        g2 = k * (3 * k + 1) // 2
        if g1 > MAXN:
            break
        sign = 1 if k % 2 == 1 else -1
        pentagonals.append((g1, sign))
        if g2 <= MAXN:
            pentagonals.append((g2, sign))

    p = [0] * (MAXN + 1)
    p[0] = 1

    for n in range(1, MAXN + 1):
        val = 0
        for g, s in pentagonals:
            if g > n:
                break
            val += s * p[n - g]
        p[n] = val % MOD
        if p[n] == 0:
            print(n)
            return

    print(-1)


if __name__ == "__main__":
    main()
