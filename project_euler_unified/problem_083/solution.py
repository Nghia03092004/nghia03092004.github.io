"""
Problem 83: Path Sum: Four Ways

Find the minimal path sum from top-left to bottom-right of an 80x80 matrix,
moving in all four directions (up, down, left, right).
Uses Dijkstra's algorithm.

Answer: 425185
"""

import os
import heapq

def solve(matrix):
    """Dijkstra's algorithm on the grid."""
    n = len(matrix)
    INF = float('inf')
    dist = [[INF]*n for _ in range(n)]
    dist[0][0] = matrix[0][0]
    pq = [(matrix[0][0], 0, 0)]

    directions = [(-1, 0), (1, 0), (0, -1), (0, 1)]

    while pq:
        d, x, y = heapq.heappop(pq)
        if d > dist[x][y]:
            continue
        if x == n-1 and y == n-1:
            return d
        for dx, dy in directions:
            nx, ny = x + dx, y + dy
            if 0 <= nx < n and 0 <= ny < n:
                nd = d + matrix[nx][ny]
                if nd < dist[nx][ny]:
                    dist[nx][ny] = nd
                    heapq.heappush(pq, (nd, nx, ny))

    return dist[n-1][n-1]

def read_matrix(filename):
    matrix = []
    with open(filename) as f:
        for line in f:
            row = list(map(int, line.strip().split(',')))
            matrix.append(row)
    return matrix

def main():
    script_dir = os.path.dirname(os.path.abspath(__file__))
    filepath = os.path.join(script_dir, "p083_matrix.txt")

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
        # Expected: 2297

    answer = solve(matrix)
    print(answer)
    # Full 80x80 answer: 425185
