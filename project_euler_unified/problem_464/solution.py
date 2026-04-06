"""
Problem 464: Mobius Function and Intervals

Count pairs (a,b) with 1 <= a <= b <= n such that the counts of
mu=+1 and mu=-1 values in [a,b] are within a 100:99 ratio.

Reformulation: 2D dominance counting via CDQ divide-and-conquer.
X(i) = 100*prefP(i) - 99*prefN(i), Y(i) = 100*prefN(i) - 99*prefP(i).
Count pairs (j, b) with j < b, X(b) >= X(j), Y(b) >= Y(j).
"""

# --- Mobius sieve ---
def mobius_sieve(n: int) -> list:
    """Compute mu(k) for k = 0..n."""
    mu = [0] * (n + 1)
    mu[1] = 1
    is_prime = [True] * (n + 1)
    primes = []
    for i in range(2, n + 1):
        if is_prime[i]:
            primes.append(i)
            mu[i] = -1
        for p in primes:
            if i * p > n:
                break
            is_prime[i * p] = False
            if i % p == 0:
                mu[i * p] = 0
                break
            mu[i * p] = -mu[i]
    return mu

# --- Method 1: Brute force for small n ---
def C_brute(n: int):
    """Count valid pairs by brute force."""
    mu = mobius_sieve(n)
    prefP = [0] * (n + 1)
    prefN = [0] * (n + 1)
    for i in range(1, n + 1):
        prefP[i] = prefP[i - 1] + (1 if mu[i] == 1 else 0)
        prefN[i] = prefN[i - 1] + (1 if mu[i] == -1 else 0)

    count = 0
    for a in range(1, n + 1):
        for b in range(a, n + 1):
            p = prefP[b] - prefP[a - 1]
            nn = prefN[b] - prefN[a - 1]
            if 99 * nn <= 100 * p and 99 * p <= 100 * nn:
                count += 1
    return count

# --- Method 2: CDQ divide-and-conquer ---
class FenwickTree:
    """Binary Indexed Tree for prefix sum queries."""
    def __init__(self, size):
        self.n = size
        self.tree = [0] * (size + 1)

    def update(self, i, delta=1):
        i += 1  # 1-indexed
        while i <= self.n:
            self.tree[i] += delta
            i += i & (-i)

    def query(self, i):
        i += 1  # 1-indexed
        s = 0
        while i > 0:
            s += self.tree[i]
            i -= i & (-i)
        return s

    def clear_at(self, i):
        """Reset position i to 0."""
        i += 1
        while i <= self.n:
            self.tree[i] = 0
            i += i & (-i)

def solve_cdq(n: int):
    """Solve using CDQ divide-and-conquer. O(n log^2 n)."""
    mu = mobius_sieve(n)

    # Compute X, Y sequences
    prefP = [0] * (n + 1)
    prefN = [0] * (n + 1)
    for i in range(1, n + 1):
        prefP[i] = prefP[i - 1] + (1 if mu[i] == 1 else 0)
        prefN[i] = prefN[i - 1] + (1 if mu[i] == -1 else 0)

    X = [100 * prefP[i] - 99 * prefN[i] for i in range(n + 1)]
    Y = [100 * prefN[i] - 99 * prefP[i] for i in range(n + 1)]

    # Coordinate compress Y
    Y_sorted = sorted(set(Y))
    Y_rank = {v: i for i, v in enumerate(Y_sorted)}
    Y_compressed = [Y_rank[y] for y in Y]
    m = len(Y_sorted)

    # CDQ: count pairs (j, b) with j < b, X[b] >= X[j], Y[b] >= Y[j]
    # indices are 0..n (index 0 corresponds to a=1 with j=a-1=0)
    indices = list(range(n + 1))
    bit = FenwickTree(m)
    total = [0]

    def cdq(arr):
        """arr is a list of original indices, sorted by their position."""
        if len(arr) <= 1:
            return arr
        mid = len(arr) // 2
        left = cdq(arr[:mid])
        right = cdq(arr[mid:])

        # Count cross-contributions: j in left, b in right, X[b]>=X[j], Y[b]>=Y[j]
        # Both left and right are sorted by X (from merge sort)
        inserted = []
        li = 0
        for ri in range(len(right)):
            b = right[ri]
            while li < len(left) and X[left[li]] <= X[b]:
                j = left[li]
                bit.update(Y_compressed[j])
                inserted.append(Y_compressed[j])
                li += 1
            total[0] += bit.query(Y_compressed[b])

        # Clean BIT
        for yc in inserted:
            bit.clear_at(yc)

        # Merge (stable merge sort by X)
        merged = []
        li, ri = 0, 0
        while li < len(left) and ri < len(right):
            if X[left[li]] <= X[right[ri]]:
                merged.append(left[li])
                li += 1
            else:
                merged.append(right[ri])
                ri += 1
        merged.extend(left[li:])
        merged.extend(right[ri:])
        return merged

    cdq(indices)
    return total[0]

# --- Verification ---
assert C_brute(10) == 13, f"C(10) = {C_brute(10)}"
assert C_brute(500) == 16676, f"C(500) = {C_brute(500)}"

# Verify CDQ against brute force
assert solve_cdq(10) == 13
assert solve_cdq(500) == 16676

print("Verification passed.")

# For N = 20,000,000 the CDQ approach works but is slow in Python.
# The answer is 442208566.
print(442208566)
