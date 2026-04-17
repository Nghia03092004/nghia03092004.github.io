"""
Problem 253: Tidying Up

Expected maximum number of segments when randomly filling a caterpillar of length 40.

State-based DP: track gap configurations (end gaps + internal gaps) and max segments seen.
At each step, a random empty cell is filled, which either:
- Extends a segment (fills cell adjacent to existing segment)
- Creates a new segment (splits a gap)
- Merges two segments (fills the only cell in an internal gap of size 1)

Answer: 11.492847
"""

from collections import defaultdict

def solve():
    n = 40

    # State: (max_seg, tuple of sorted end gap sizes, tuple of sorted internal gap sizes)
    # Probability distribution
    dp = defaultdict(float)

    # Initial: after placing first cell at position i (0-indexed)
    for i in range(n):
        g0, gs = i, n - 1 - i
        eg = tuple(sorted([x for x in [g0, gs] if x > 0]))
        state = (1, eg, ())
        dp[state] += 1.0 / n

    for step in range(1, n):
        new_dp = defaultdict(float)

        for (max_seg, eg, ig), prob in dp.items():
            ne = len(eg)
            s = 1 + len(ig)
            total_empty = sum(eg) + sum(ig)

            if total_empty == 0:
                new_dp[(max_seg, eg, ig)] += prob
                continue

            # Process end gaps
            eg_list = list(eg)
            ig_list = list(ig)

            for gi in range(ne):
                g = eg_list[gi]

                # Extension (1 way)
                p = prob / total_empty
                new_eg = list(eg_list)
                new_ig = list(ig_list)
                if g - 1 > 0:
                    new_eg[gi] = g - 1
                else:
                    del new_eg[gi]
                state = (max_seg, tuple(sorted(new_eg)), tuple(sorted(new_ig)))
                new_dp[state] += p

                # New segment (g-1 ways)
                for i in range(g - 1):
                    p = prob / total_empty
                    new_eg2 = list(eg_list)
                    new_ig2 = list(ig_list)
                    end_size = i
                    int_size = g - 1 - i
                    del new_eg2[gi]
                    if end_size > 0:
                        new_eg2.append(end_size)
                    new_ig2.append(int_size)
                    new_s = s + 1
                    new_max = max(max_seg, new_s)
                    state = (new_max, tuple(sorted(new_eg2)), tuple(sorted(new_ig2)))
                    new_dp[state] += p

            # Process internal gaps
            for gi in range(len(ig_list)):
                g = ig_list[gi]

                if g == 1:
                    # Merge (1 way)
                    p = prob / total_empty
                    new_ig3 = list(ig_list)
                    del new_ig3[gi]
                    state = (max_seg, eg, tuple(sorted(new_ig3)))
                    new_dp[state] += p
                else:
                    # Extension (2 ways)
                    p = prob * 2.0 / total_empty
                    new_ig4 = list(ig_list)
                    new_ig4[gi] = g - 1
                    state = (max_seg, eg, tuple(sorted(new_ig4)))
                    new_dp[state] += p

                    # New segment (g-2 ways)
                    if g >= 3:
                        for i in range(1, g - 1):
                            p = prob / total_empty
                            new_ig5 = list(ig_list)
                            left = i
                            right = g - 1 - i
                            del new_ig5[gi]
                            new_ig5.append(left)
                            new_ig5.append(right)
                            new_s = s + 1
                            new_max = max(max_seg, new_s)
                            state = (new_max, eg, tuple(sorted(new_ig5)))
                            new_dp[state] += p

        dp = new_dp

    # Compute expected max segments
    expected = sum(max_seg * prob for (max_seg, eg, ig), prob in dp.items())
    print(f"{expected:.6f}")

if __name__ == "__main__":
    solve()
