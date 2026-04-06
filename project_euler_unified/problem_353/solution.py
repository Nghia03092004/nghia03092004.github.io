"""
Problem 353: Risky Moon

For each prime r <= 14, find lattice points on sphere of radius r,
then use Dijkstra to find the minimum-risk path from north pole to south pole.

Risk of a segment with arc angle theta = (theta/2)^2.
Total answer is the sum over all primes r <= 14.
"""

import heapq
import math

def solve():
    primes = [2, 3, 5, 7, 11, 13]
    total_risk = 0.0

    for r in primes:
        r2 = r * r

        # Enumerate all lattice points on sphere of radius r
        points = set()
        for a in range(-r, r + 1):
            for b in range(-r, r + 1):
                rem = r2 - a * a - b * b
                if rem < 0:
                    continue
                c = int(round(math.sqrt(rem)))
                if c * c == rem:
                    points.add((a, b, c))
                    if c > 0:
                        points.add((a, b, -c))

        points = list(points)
        n = len(points)
        idx = {p: i for i, p in enumerate(points)}

        north = idx.get((0, 0, r))
        south = idx.get((0, 0, -r))

        if north is None or south is None:
            continue

        # Dijkstra
        dist = [float('inf')] * n
        dist[north] = 0.0
        pq = [(0.0, north)]

        while pq:
            d, u = heapq.heappop(pq)
            if d > dist[u] + 1e-15:
                continue
            if u == south:
                break

            a1, b1, c1 = points[u]
            for v in range(n):
                if v == u:
                    continue
                a2, b2, c2 = points[v]
                cos_theta = (a1 * a2 + b1 * b2 + c1 * c2) / r2
                cos_theta = max(-1.0, min(1.0, cos_theta))
                theta = math.acos(cos_theta)
                risk = (theta / 2.0) ** 2
                if dist[u] + risk < dist[v] - 1e-15:
                    dist[v] = dist[u] + risk
                    heapq.heappush(pq, (dist[v], v))

        total_risk += dist[south]

    print(f"{total_risk:.10f}")

if __name__ == "__main__":
    solve()
