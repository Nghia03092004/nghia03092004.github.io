#!/usr/bin/env python3
"""
Project Euler Problem 460: An Ant on the Move

An ant travels from A(0,1) to B(d,1) on the Euclidean plane.
At each step it moves to a lattice point (x1,y1) with x1>=0, y1>=1.
Velocity: v = y0 if y0==y1, else (y1-y0)/(ln(y1)-ln(y0)).
Time = distance / v.

Find F(10000) = minimum total time, rounded to 9 decimal places.

Answer: 18.420738199
"""

import math
import sys

def log_mean(y0, y1):
    """Logarithmic mean of y0 and y1."""
    if y0 == y1:
        return float(y0)
    return (y1 - y0) / (math.log(y1) - math.log(y0))

def travel_time(x0, y0, x1, y1):
    """Time to travel from (x0,y0) to (x1,y1)."""
    dist = math.sqrt((x1 - x0)**2 + (y1 - y0)**2)
    v = log_mean(y0, y1)
    return dist / v

def F_dp(d, y_max=None):
    """
    Compute F(d) using dynamic programming.

    T[x][y] = minimum time to reach (d, 1) from (x, y).
    We work backwards from (d, 1).

    For efficiency, we limit the maximum y coordinate.
    """
    if y_max is None:
        # Heuristic: optimal height ~ sqrt(d) based on continuous approximation
        y_max = int(3 * math.sqrt(d)) + 5

    INF = float('inf')

    # T[x][y] = min time from (x, y) to (d, 1)
    # Use 1D array per x, iterate x from d down to 0
    # y ranges from 1 to y_max

    # Forward DP: f[x][y] = min time from (0,1) to (x,y)
    f = [[INF] * (y_max + 1) for _ in range(d + 1)]
    f[0][1] = 0.0

    for x0 in range(d + 1):
        for y0 in range(1, y_max + 1):
            if f[x0][y0] == INF:
                continue
            current_time = f[x0][y0]

            # Try all reachable points (x1, y1)
            # Limit step size for efficiency
            max_dx = min(d - x0, max(2 * y0, 20))

            for x1 in range(x0 + 1, min(x0 + max_dx + 1, d + 1)):
                for y1 in range(1, y_max + 1):
                    t = travel_time(x0, y0, x1, y1)
                    new_time = current_time + t
                    if new_time < f[x1][y1]:
                        f[x1][y1] = new_time

    return f[d][1]

def F_optimized(d, y_max=None):
    """
    Optimized DP for F(d).

    Key optimization: for each (x0, y0), only consider a limited set
    of next positions. The optimal next step tends to be close in x
    (step size ~ y0) and the y change is bounded.
    """
    if y_max is None:
        y_max = int(2.5 * math.sqrt(d)) + 10

    INF = float('inf')

    # Use dictionary for sparse representation
    # f[x] = dict mapping y -> min_time
    f = [{} for _ in range(d + 1)]
    f[0][1] = 0.0

    for x0 in range(d):
        if not f[x0]:
            continue
        for y0, current_time in f[x0].items():
            # Optimal horizontal step at height y0 is roughly y0
            # (travel distance ~ step, speed ~ y0, time ~ step/y0 ~ 1)
            # Try steps from 1 to a reasonable max
            step_limit = min(d - x0, max(int(3 * y0), 10))

            for dx in range(1, step_limit + 1):
                x1 = x0 + dx
                if x1 > d:
                    break

                # Try different heights
                # Going higher is only beneficial if we stay there for a while
                for y1 in range(max(1, y0 - 5), min(y_max, y0 + 5) + 1):
                    t = travel_time(x0, y0, x1, y1)
                    new_time = current_time + t
                    if y1 not in f[x1] or new_time < f[x1][y1]:
                        f[x1][y1] = new_time

    return f[d].get(1, INF)

def F_small(d):
    """Exact DP for small d (all transitions considered)."""
    y_max = min(d + 2, 30)
    INF = float('inf')

    f = {}
    f[(0, 1)] = 0.0

    for x0 in range(d + 1):
        for y0 in range(1, y_max + 1):
            if (x0, y0) not in f:
                continue
            ct = f[(x0, y0)]

            for x1 in range(x0 + 1, d + 1):
                for y1 in range(1, y_max + 1):
                    t = travel_time(x0, y0, x1, y1)
                    nt = ct + t
                    key = (x1, y1)
                    if key not in f or nt < f[key]:
                        f[key] = nt

    return f.get((d, 1), INF)

def create_visualization():
    """Create visualization of the ant problem."""
