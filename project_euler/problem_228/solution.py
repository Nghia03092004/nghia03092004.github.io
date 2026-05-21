"""
Problem 228: Minkowski Sums

The outward normals of S_n are the directions 2*pi*k/n, so the Minkowski sum
has one side for each reduced fraction p/q in [0, 1) whose denominator q
divides at least one n in [1864, 1909].
"""


def solve():
    first = 1864
    last = 1909

    phi = list(range(last + 1))
    for value in range(2, last + 1):
        if phi[value] == value:
            for multiple in range(value, last + 1, value):
                phi[multiple] = phi[multiple] // value * (value - 1)

    total = 1
    for denominator in range(2, last + 1):
        if last // denominator > (first - 1) // denominator:
            total += phi[denominator]

    print(total)


if __name__ == "__main__":
    solve()
