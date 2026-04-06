import math
from sortedcontainers import SortedList

def solve():
    K = 2000000
    MOD = 50515093

    # Generate points: s_0 = 290797, s_{n+1} = s_n^2 mod M
    # P_n = (s_{2n}, s_{2n+1})
    pts = []
    s = 290797
    for i in range(K):
        x = s
        s = (s * s) % MOD
        y = s
        s = (s * s) % MOD
        pts.append((x, y))

    # Sort by x-coordinate
    pts.sort()

    # Sweep line algorithm
    # Use SortedList keyed by (y, x)
    active = SortedList(key=lambda p: (p[1], p[0]))
    best = float('inf')
    left = 0

    for i in range(K):
        d = best

        # Remove points too far left
        while left < i and pts[i][0] - pts[left][0] > d:
            active.remove(pts[left])
            left += 1

        # Find points in y-range [pts[i][1] - d, pts[i][1] + d]
        lo_y = pts[i][1] - d
        hi_y = pts[i][1] + d

        # Use irange or bisect to find candidates
        idx_lo = active.bisect_key_left((lo_y, -1))
        idx_hi = active.bisect_key_right((hi_y, float('inf')))

        for j in range(idx_lo, min(idx_hi, len(active))):
            p = active[j]
            if p[1] > hi_y:
                break
            dx = pts[i][0] - p[0]
            dy = pts[i][1] - p[1]
            dist = math.sqrt(dx*dx + dy*dy)
            if dist < best:
                best = dist

        active.add(pts[i])

    print(f"{best:.9f}")

if __name__ == "__main__":
    solve()
