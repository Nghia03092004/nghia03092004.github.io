"""
Problem 109: Darts

Count distinct ways to check out (reach exactly 0) with score < 100.
Last dart must be a double. Up to 3 darts total.
Non-final darts are unordered (combinations, not permutations).
"""

def solve():
    # Build all 62 distinct darts
    # Each dart has a score and an index for canonical ordering
    all_darts = []  # (score, label) - label for identification

    # Singles: S1..S20, S25
    for i in range(1, 21):
        all_darts.append((i, f'S{i}'))
    all_darts.append((25, 'S25'))

    # Doubles: D1..D20, D25
    doubles = []
    for i in range(1, 21):
        all_darts.append((2 * i, f'D{i}'))
        doubles.append(2 * i)
    all_darts.append((50, 'D25'))
    doubles.append(50)

    # Trebles: T1..T20
    for i in range(1, 21):
        all_darts.append((3 * i, f'T{i}'))

    scores = [d[0] for d in all_darts]
    n = len(scores)  # 62

    count = 0

    # 1-dart: just a double
    for d in doubles:
        if d < 100:
            count += 1

    # 2-dart: one non-final + one final double
    for i in range(n):
        for d in doubles:
            if scores[i] + d < 100:
                count += 1

    # 3-dart: two non-final (unordered, i <= j) + one final double
    for i in range(n):
        for j in range(i, n):
            for d in doubles:
                if scores[i] + scores[j] + d < 100:
                    count += 1

    return count

answer = solve()
assert answer == 38182
print(answer)
