def solve():
    LIMIT = 28123

    spd = [0] * (LIMIT + 1)
    for i in range(1, LIMIT + 1):
        for j in range(2 * i, LIMIT + 1, i):
            spd[j] += i

    abundant = [i for i in range(1, LIMIT + 1) if spd[i] > i]

    is_sum = [False] * (LIMIT + 1)
    for i, a in enumerate(abundant):
        for j in range(i, len(abundant)):
            s = a + abundant[j]
            if s > LIMIT:
                break
            is_sum[s] = True

    print(sum(i for i in range(1, LIMIT + 1) if not is_sum[i]))
