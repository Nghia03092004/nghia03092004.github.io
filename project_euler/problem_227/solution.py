"""
Problem 227: The Chase

State d is the shorter circular distance between the two dice, so d ranges
from 0 to 50. The expected time satisfies

    E(d) = 1 + sum P(d -> d') * E(d')

for d >= 1. We solve the resulting 50 x 50 linear system.
"""


def gaussian_elimination(matrix):
    n = len(matrix)

    for col in range(n):
        pivot = max(range(col, n), key=lambda row: abs(matrix[row][col]))
        matrix[col], matrix[pivot] = matrix[pivot], matrix[col]

        pivot_value = matrix[col][col]
        for row in range(col + 1, n):
            factor = matrix[row][col] / pivot_value
            if factor == 0.0:
                continue
            for index in range(col, n + 1):
                matrix[row][index] -= factor * matrix[col][index]

    solution = [0.0] * n
    for row in range(n - 1, -1, -1):
        value = matrix[row][n]
        for col in range(row + 1, n):
            value -= matrix[row][col] * solution[col]
        solution[row] = value / matrix[row][row]

    return solution


def solve():
    board = 100
    states = board // 2
    delta_probabilities = {
        -2: 1.0 / 36.0,
        -1: 8.0 / 36.0,
        0: 18.0 / 36.0,
        1: 8.0 / 36.0,
        2: 1.0 / 36.0,
    }

    transition = [[0.0] * states for _ in range(states)]
    for distance in range(1, states + 1):
        for delta, probability in delta_probabilities.items():
            raw = (distance + delta) % board
            new_distance = min(raw, board - raw)
            if 1 <= new_distance <= states:
                transition[distance - 1][new_distance - 1] += probability

    augmented = []
    for row in range(states):
        equation = [0.0] * (states + 1)
        for col in range(states):
            equation[col] = (1.0 if row == col else 0.0) - transition[row][col]
        equation[states] = 1.0
        augmented.append(equation)

    expectations = gaussian_elimination(augmented)
    print(f"{expectations[states - 1]:.6f}")


if __name__ == "__main__":
    solve()
