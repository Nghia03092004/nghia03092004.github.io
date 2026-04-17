"""
Problem 255: Rounded Square Roots

Average number of iterations of the rounded square root algorithm
for n from 10^13 to 10^14 - 1.

All numbers have 14 digits (even), so x0 = 7 * 10^6 = 7000000.

Iteration: x_{k+1} = floor((x_k + ceil(n/x_k)) / 2)
Stop when x_{k+1} == x_k.

Interval-based approach: track (x_current, n_lo, n_hi) and split by
ceil(n/x) values at each step.

Answer: 4.4474011180
"""

from collections import defaultdict

def ceildiv(a, b):
    return (a + b - 1) // b

def solve():
    N_LO = 10**13
    N_HI = 10**14 - 1
    TOTAL = N_HI - N_LO + 1
    x0 = 7000000

    # State: list of (x, n_lo, n_hi)
    current = [(x0, N_LO, N_HI)]
    total_iterations = 0.0
    iteration = 0

    while current:
        iteration += 1
        next_map = defaultdict(list)  # x_next -> [(nlo, nhi)]
        converged_count = 0

        for x, nlo, nhi in current:
            q_lo = ceildiv(nlo, x)
            q_hi = ceildiv(nhi, x)

            xn_lo = (x + q_lo) // 2
            xn_hi = (x + q_hi) // 2

            for xn in range(xn_lo, xn_hi + 1):
                q1 = max(2 * xn - x, q_lo)
                q2 = min(2 * xn + 1 - x, q_hi)
                if q1 > q2:
                    continue

                n1 = max((q1 - 1) * x + 1, nlo)
                n2 = min(q2 * x, nhi)
                if n1 > n2:
                    continue

                if xn == x:
                    converged_count += n2 - n1 + 1
                else:
                    next_map[xn].append((n1, n2))

        total_iterations += converged_count * iteration

        # Merge intervals
        current = []
        for xn, intervals in sorted(next_map.items()):
            intervals.sort()
            lo, hi = intervals[0]
            for i in range(1, len(intervals)):
                if intervals[i][0] <= hi + 1:
                    hi = max(hi, intervals[i][1])
                else:
                    current.append((xn, lo, hi))
                    lo, hi = intervals[i]
            current.append((xn, lo, hi))

    avg = total_iterations / TOTAL
    print(f"{avg:.10f}")

if __name__ == "__main__":
    solve()
