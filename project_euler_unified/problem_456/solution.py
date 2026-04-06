import math

def solve():
    N = 2000000

    # Generate points and compute angles
    angles = []
    for i in range(1, N + 1):
        xi = (1248 * i % 32323) - 16161
        yi = (8421 * i % 30103) - 15051
        angles.append(math.atan2(yi, xi))

    angles.sort()

    # Duplicate for circular handling
    ang = angles + [a + 2 * math.pi for a in angles]

    # Two-pointer: count f(i) = points in open semicircle
    bad = 0
    j = 0
    for i in range(N):
        if j <= i:
            j = i + 1
        while j < 2 * N and ang[j] - ang[i] < math.pi - 1e-9:
            j += 1
        fi = j - i - 1
        bad += fi * (fi - 1) // 2

    total = N * (N - 1) * (N - 2) // 6
    answer = total - bad

    print(answer)

solve()
