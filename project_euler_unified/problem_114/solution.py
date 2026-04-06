def solve():
    N = 50
    f = {-1: 1, 0: 1}
    prefix = {-1: 1, 0: 2}

    for n in range(1, N + 1):
        p_idx = n - 4
        if n >= 3:
            p = prefix[p_idx] if p_idx >= -1 else prefix[-1]
        else:
            p = 0
        f[n] = f[n - 1] + p
        prefix[n] = prefix[n - 1] + f[n]

    print(f[N])


solve()
