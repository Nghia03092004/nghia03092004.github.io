def solve():
    NCUBOIDS = 50000
    SLEN = 6 * NCUBOIDS

    S = [0] * (SLEN + 1)
    for k in range(1, 56):
        S[k] = (100003 - 200003 * k + 300007 * k * k * k) % 1000000
    for k in range(56, SLEN + 1):
        S[k] = (S[k - 24] + S[k - 55]) % 1000000

    cuboids = []
    for n in range(1, NCUBOIDS + 1):
        x0 = S[6 * n - 5] % 10000
        y0 = S[6 * n - 4] % 10000
        z0 = S[6 * n - 3] % 10000
        dx = 1 + S[6 * n - 2] % 399
        dy = 1 + S[6 * n - 1] % 399
        dz = 1 + S[6 * n] % 399
        cuboids.append((x0, y0, z0, x0 + dx, y0 + dy, z0 + dz))

    zset = set()
    for c in cuboids:
        zset.add(c[2])
        zset.add(c[5])
    zcoords = sorted(zset)

    total_volume = 0

    for zi in range(len(zcoords) - 1):
        z0, z1 = zcoords[zi], zcoords[zi + 1]
        dz = z1 - z0
        active = [c for c in cuboids if c[2] <= z0 and z1 <= c[5]]
        if not active:
            continue

        xset = set()
        for c in active:
            xset.add(c[0])
            xset.add(c[3])
        xcoords = sorted(xset)

        for xi in range(len(xcoords) - 1):
            x0, x1 = xcoords[xi], xcoords[xi + 1]
            dx = x1 - x0
            yintervals = []
            for c in active:
                if c[0] <= x0 and x1 <= c[3]:
                    yintervals.append((c[1], c[4]))
            if not yintervals:
                continue

            yintervals.sort()
            ylen = 0
            cy0, cy1 = yintervals[0]
            for j in range(1, len(yintervals)):
                if yintervals[j][0] >= cy1:
                    ylen += cy1 - cy0
                    cy0, cy1 = yintervals[j]
                else:
                    cy1 = max(cy1, yintervals[j][1])
            ylen += cy1 - cy0

            total_volume += dz * dx * ylen

    print(total_volume)

if __name__ == "__main__":
    solve()
