def solve():
    pow5 = [i**5 for i in range(10)]
    total = 0
    for n in range(2, 6 * 9**5 + 1):
        s, t = 0, n
        while t:
            s += pow5[t % 10]
            t //= 10
        if s == n:
            total += n
    print(total)

if __name__ == "__main__":
    solve()
