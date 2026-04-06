def solve():
    N = 1001
    # Closed-form: S = (4N^3 + 3N^2 + 8N - 9) / 6
    answer = (4 * N**3 + 3 * N**2 + 8 * N - 9) // 6
    print(answer)

if __name__ == "__main__":
    solve()
