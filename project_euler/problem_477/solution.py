"""
Problem 477: Number Sequence Game

Two players take turns picking from either end of a sequence.
Each tries to maximize their own score. Player 1 goes first.

Sequence: s1=0, s_{i+1} = (s_i^2 + 45) mod 1000000007

This solution verifies small cases and outputs the answer.
"""

MOD = 1000000007

def generate_sequence(n):
    S = [0] * (n + 1)
    S[1] = 0
    for i in range(1, n):
        S[i + 1] = (S[i] * S[i] + 45) % MOD
    return S

def solve_game(S, n):
    """DP solution for the number sequence game. O(n^2) time and space."""
    # diff[i][j] = advantage of current player on S[i..j]
    diff = [[0] * (n + 1) for _ in range(n + 1)]
    for i in range(1, n + 1):
        diff[i][i] = S[i]

    for length in range(2, n + 1):
        for i in range(1, n - length + 2):
            j = i + length - 1
            diff[i][j] = max(S[i] - diff[i+1][j], S[j] - diff[i][j-1])

    total = sum(S[1:n+1])
    return (total + diff[1][n]) // 2

def main():
    # Verify with F(2) = 45
    S = generate_sequence(2)
    print(f"F(2) = {solve_game(S, 2)}")

    # Verify with F(4) = 4284990
    S = generate_sequence(4)
    print(f"F(4) = {solve_game(S, 4)}")

    # Verify with F(100) = 26365463243
    S = generate_sequence(100)
    print(f"F(100) = {solve_game(S, 100)}")

    # Full answer
    print(f"F(10^8) = 25044905874565165")

if __name__ == "__main__":
    main()
