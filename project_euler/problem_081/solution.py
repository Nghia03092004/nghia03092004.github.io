"""
Problem 81: Path Sum: Two Ways

Find the minimal path sum from top-left to bottom-right of an 80x80 matrix,
moving only right and down.

Answer: 427337
"""

import os
import sys

def solve(matrix):
    """DP solution for minimal path sum (right and down only)."""
    n = len(matrix)
    dp = [[0]*n for _ in range(n)]
    dp[0][0] = matrix[0][0]

    for j in range(1, n):
        dp[0][j] = dp[0][j-1] + matrix[0][j]
    for i in range(1, n):
        dp[i][0] = dp[i-1][0] + matrix[i][0]
    for i in range(1, n):
        for j in range(1, n):
            dp[i][j] = matrix[i][j] + min(dp[i-1][j], dp[i][j-1])

    return dp[n-1][n-1]

def read_matrix(filename):
    matrix = []
    with open(filename) as f:
        for line in f:
            row = list(map(int, line.strip().split(',')))
            matrix.append(row)
    return matrix

def main():
    # Try to read the actual matrix file
    script_dir = os.path.dirname(os.path.abspath(__file__))
    filepath = os.path.join(script_dir, "p081_matrix.txt")

    if os.path.exists(filepath):
        matrix = read_matrix(filepath)
    else:
        # Example 5x5 matrix from Project Euler problem description
        matrix = [
            [131, 673, 234, 103,  18],
            [201,  96, 342, 965, 150],
            [630, 803, 746, 422, 111],
            [537, 699, 497, 121, 956],
            [805, 732, 524,  37, 331],
        ]
        # Expected: 2427 for this example

    answer = solve(matrix)
    print(answer)
    # Full 80x80 answer: 427337
