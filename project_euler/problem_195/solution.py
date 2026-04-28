"""
Project Euler Problem 195: Inscribed Circles of Triangles with One Angle of 60 Degrees

Count 60-degree triangles with incircle radius at most 1,053,779.
"""

import math


SQRT3 = math.sqrt(3.0)


def count_triangles(limit):
    limit_plain = 2.0 * limit / SQRT3
    limit_div3 = 6.0 * limit / SQRT3
    q_max = int(math.sqrt(limit_div3)) + 2
    total = 0

    for q in range(1, q_max + 1):
        p_max = int(limit_div3 / q)
        for p in range(q + 1, p_max + 1):
            if math.gcd(p, q) != 1:
                continue
            product = p * q
            if (p - q) % 3 == 0:
                total += int(limit_div3 / product)
            else:
                total += int(limit_plain / product)

    return total


def solve():
    return count_triangles(1053779)


if __name__ == "__main__":
    assert count_triangles(100) == 1234
    assert count_triangles(1000) == 22767
    assert count_triangles(10000) == 359912
    answer = solve()
    assert answer == 75085391
    print(answer)
