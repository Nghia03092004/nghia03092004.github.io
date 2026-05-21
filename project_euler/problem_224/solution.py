"""
Problem 224: Almost Right-angled Triangles II

We need positive solutions of

    a^2 + b^2 = c^2 - 1

with a <= b and a + b + c <= 75,000,000.

The Berggren matrices preserve x^2 + y^2 - z^2 = -1. Starting from the root
(2, 2, 3), they generate every positive primitive solution. Because a and b
play symmetric roles, sorted triples only need two children when a = b and all
three otherwise.
"""

MATRICES = (
    ((1, -2, 2), (2, -1, 2), (2, -2, 3)),
    ((1, 2, 2), (2, 1, 2), (2, 2, 3)),
    ((-1, 2, 2), (-2, 1, 2), (-2, 2, 3)),
)


def transform(matrix, triple):
    a, b, c = triple
    return (
        matrix[0][0] * a + matrix[0][1] * b + matrix[0][2] * c,
        matrix[1][0] * a + matrix[1][1] * b + matrix[1][2] * c,
        matrix[2][0] * a + matrix[2][1] * b + matrix[2][2] * c,
    )


def solve():
    limit = 75_000_000
    count = 0
    stack = [(2, 2, 3)]

    while stack:
        a, b, c = stack.pop()
        if a > b:
            a, b = b, a

        if a + b + c > limit:
            continue

        count += 1
        children = MATRICES[:2] if a == b else MATRICES

        for matrix in children:
            x, y, z = transform(matrix, (a, b, c))
            if x <= 0 or y <= 0 or z <= 0:
                continue
            if x > y:
                x, y = y, x
            stack.append((x, y, z))

    print(count)


if __name__ == "__main__":
    solve()
