"""
Problem 82: Path Sum: Three Ways

Find the minimal path sum from the left column to the right column,
moving right, up, or down in an 80x80 matrix.

Answer: 260324
"""

import os

def solve(matrix):
    """DP column-by-column with up/down propagation."""
    n = len(matrix)
    dp = [matrix[i][0] for i in range(n)]

    for j in range(1, n):
        # Cost coming from the left
        ndp = [dp[i] + matrix[i][j] for i in range(n)]

        # Downward pass
        for i in range(1, n):
            ndp[i] = min(ndp[i], ndp[i-1] + matrix[i][j])

        # Upward pass
        for i in range(n-2, -1, -1):
            ndp[i] = min(ndp[i], ndp[i+1] + matrix[i][j])

        dp = ndp

    return min(dp)

def read_matrix(filename):
    matrix = []
    with open(filename) as f:
        for line in f:
            row = list(map(int, line.strip().split(',')))
            matrix.append(row)
    return matrix

def main():
    script_dir = os.path.dirname(os.path.abspath(__file__))
    filepath = os.path.join(script_dir, "p082_matrix.txt")

    if os.path.exists(filepath):
        matrix = read_matrix(filepath)
    else:
        matrix = [
            [131, 673, 234, 103,  18],
            [201,  96, 342, 965, 150],
            [630, 803, 746, 422, 111],
            [537, 699, 497, 121, 956],
            [805, 732, 524,  37, 331],
        ]
        # Expected: 994

    answer = solve(matrix)
    print(answer)
    # Full 80x80 answer: 260324
