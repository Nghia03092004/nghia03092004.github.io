from itertools import combinations

def solve():
    """
    Count distinct arrangements of two dice (6 faces each, digits 0-9)
    that can display all two-digit perfect squares:
    01, 04, 09, 16, 25, 36, 49, 64, 81.
    Digits 6 and 9 are interchangeable.
    """
    # Required digit pairs for each square
    required_pairs = [
        (0, 1), (0, 4), (0, 9),
        (1, 6), (2, 5), (3, 6),
        (4, 9), (6, 4), (8, 1)
    ]

    # Generate all C(10,6) = 210 possible cubes
    cubes = list(combinations(range(10), 6))

    def has_digit(cube, d):
        """Check if cube has digit d (with 6/9 interchangeability)."""
        if d in cube:
            return True
        if d == 6 and 9 in cube:
            return True
        if d == 9 and 6 in cube:
            return True
        return False

    def valid_pair(c1, c2):
        """Check if cube pair can display all required squares."""
        for a, b in required_pairs:
            ok = (has_digit(c1, a) and has_digit(c2, b)) or \
                 (has_digit(c2, a) and has_digit(c1, b))
            if not ok:
                return False
        return True

    count = 0
    n = len(cubes)
    for i in range(n):
        for j in range(i, n):
            if valid_pair(cubes[i], cubes[j]):
                count += 1

    print(count)

if __name__ == "__main__":
    solve()
