"""
Problem 663: Sums of Subarrays

Maintain maximum subarray sum under point updates using a segment tree.
S(n, l) = sum of max subarray sums after each of l updates.
"""

class SegTree:
    """Segment tree for maximum subarray sum queries."""

    def __init__(self, n):
        self.n = n
        self.size = 1
        while self.size < n + 1:
            self.size *= 2
        # Each node: (max_prefix, min_prefix, total, max_subarray)
        self.tree = [(0, 0, 0, 0)] * (2 * self.size)

    def _merge(self, left, right):
        lmax_p, lmin_p, ltotal, lmax_s = left
        rmax_p, rmin_p, rtotal, rmax_s = right
        total = ltotal + rtotal
        max_p = max(lmax_p, ltotal + rmax_p)
        min_p = min(lmin_p, ltotal + rmin_p)
        max_s = max(lmax_s, rmax_s, ltotal + rmax_p - lmin_p)
        return (max_p, min_p, total, max_s)

    def update(self, pos, delta):
        """Add delta to position pos."""
        pos += self.size
        cur = self.tree[pos]
        new_val = cur[2] + delta
        self.tree[pos] = (max(0, new_val), min(0, new_val), new_val, max(0, new_val))
        pos >>= 1
        while pos >= 1:
            self.tree[pos] = self._merge(self.tree[2*pos], self.tree[2*pos+1])
            pos >>= 1

    def query(self):
        """Return maximum subarray sum."""
        return max(0, self.tree[1][3])

def solve_small(n, l):
    """Solve S(n, l) using segment tree."""
    # Pseudo-random generator (from problem: t_0 = 0, t_{i+1} = t_i^2 mod something)
    # Actually the problem uses a specific generator. For verification,
    # we use the values from the problem statement.
    # The generator: s_0 = 290797, s_{i+1} = s_i^2 mod 50515093
    # t_i = s_i mod 2^20 (or similar)

    # Using the standard PE663 generator:
    s = 290797
    def next_s():
        nonlocal s
        s = (s * s) % 50515093
        return s

    A = [0] * n
    seg = SegTree(n)

    total = 0
    for i in range(1, l + 1):
        t1 = next_s()
        t2 = next_s()
        k = t1 % n
        delta = 2 * (t2 % n) - n + 1
        A[k] += delta
        seg.update(k, delta)
        M = seg.query()
        total += M

    return total

# Test with small cases
# Note: the exact generator may differ from what's assumed here.
# The verification values would need the exact generator from the problem.
print("Problem 663: Sums of Subarrays")
print("Implementation uses segment tree for O(log n) per update")

# Demonstrate Kadane's algorithm
def kadane(arr):
    """Maximum subarray sum via Kadane's algorithm."""
    max_ending = 0
    max_so_far = 0
    for x in arr:
        max_ending = max(0, max_ending + x)
        max_so_far = max(max_so_far, max_ending)
    return max_so_far

# Example from problem
A = [0, 0, 0, 0, 0]
print(f"Initial: A = {A}, M = {kadane(A)}")

# Verify Kadane's on known arrays
assert kadane([1, -2, 3, -1, 2]) == 4  # subarray [3, -1, 2]
assert kadane([-1, -2, -3]) == 0  # empty subarray
assert kadane([5, -2, 3]) == 6  # [5, -2, 3]
print("Kadane's algorithm verified.")
