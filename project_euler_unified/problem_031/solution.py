"""Project Euler Problem 31: Coin Sums"""

def coin_sums(target, coins):
    dp = [0] * (target + 1)
    dp[0] = 1
    for c in coins:
        for j in range(c, target + 1):
            dp[j] += dp[j - c]
    return dp[target]

def main():
    print(coin_sums(200, [1, 2, 5, 10, 20, 50, 100, 200]))  # 73682

if __name__ == "__main__":
    main()
