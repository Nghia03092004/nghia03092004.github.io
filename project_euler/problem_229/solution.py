"""
Problem 229: Four Representations Using Squares

For each slice [lo, hi), mark all values of

    a^2 + b^2,
    a^2 + 2b^2,
    a^2 + 3b^2,
    a^2 + 7b^2

that fall into the slice. The arrays b_k[a] remember where the previous slice
stopped, so every parabola continues from the correct b without restarting.
"""

from math import isqrt


SLICE_SIZE = 1_000_000
ONE = 1 << 0
TWO = 1 << 1
THREE = 1 << 2
SEVEN = 1 << 3
ALL = ONE | TWO | THREE | SEVEN


def solve():
    limit = 2_000_000_000
    exclusive = limit + 1
    max_a = isqrt(limit)

    b1 = [1] * (max_a + 1)
    b2 = [1] * (max_a + 1)
    b3 = [1] * (max_a + 1)
    b7 = [1] * (max_a + 1)

    count = 0
    block_start = 0

    while block_start < exclusive:
        block_end = min(block_start + SLICE_SIZE, exclusive)
        used = bytearray(block_end - block_start)

        a = 1
        while a * a + b1[a] * b1[a] < block_end:
            aa = a * a

            b = b1[a]
            while aa + b * b < block_end:
                used[aa + b * b - block_start] |= ONE
                b += 1
            b1[a] = b

            b = b2[a]
            while aa + 2 * b * b < block_end:
                used[aa + 2 * b * b - block_start] |= TWO
                b += 1
            b2[a] = b

            b = b3[a]
            while aa + 3 * b * b < block_end:
                used[aa + 3 * b * b - block_start] |= THREE
                b += 1
            b3[a] = b

            b = b7[a]
            while aa + 7 * b * b < block_end:
                used[aa + 7 * b * b - block_start] |= SEVEN
                b += 1
            b7[a] = b

            a += 1

        count += sum(1 for value in used if value == ALL)
        block_start = block_end

    print(count)


if __name__ == "__main__":
    solve()
