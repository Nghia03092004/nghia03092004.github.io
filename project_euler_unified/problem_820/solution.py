def solve():
    N = 10**7

    total = 0
    for k in range(2, N + 1):
        r = pow(10, N - 1, k)
        digit = (10 * r) // k
        total += digit

    print(total)

if __name__ == "__main__":
    solve()
