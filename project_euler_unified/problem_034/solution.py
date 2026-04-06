"""Project Euler Problem 34: Digit Factorials"""

def main():
    fact = [1] * 10
    for i in range(1, 10):
        fact[i] = fact[i - 1] * i

    total = 0
    for n in range(3, 2540161):
        s, tmp = 0, n
        while tmp > 0:
            s += fact[tmp % 10]
            tmp //= 10
        if s == n:
            total += n
    print(total)  # 40730

if __name__ == "__main__":
    main()
