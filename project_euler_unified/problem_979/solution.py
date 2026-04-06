"""
Problem 979: Josephus Problem Variant

Compute sum_{n=1}^{10000} J(n, 3), where J(n, k) is the 1-indexed position
of the last survivor in the Josephus problem with n people and step size k=3.

The Josephus recurrence (0-indexed):
    J(1, k) = 0
    J(n, k) = (J(n-1, k) + k) mod n

Then convert to 1-indexed: J_1(n, k) = J(n, k) + 1.

Key observations:
    - J(n, 3) shows a quasi-periodic structure with period ~n
    - The survivor position tends to cluster in certain ranges
    - For large n, J(n,3)/n converges to specific patterns

Answer: computed via iterative recurrence

Methods:
    - josephus_iterative(n, k): Single J(n,k) via recurrence
    - solve_sum(N, k): Compute sum of J(n,k) for n=1..N
    - verify_small_cases(): Check against brute force simulation
    - josephus_simulate(n, k): Brute force simulation for verification
"""


def josephus_iterative(n, k):
    """Compute 1-indexed Josephus survivor position for n people, step k."""
    j = 0
    for i in range(2, n + 1):
        j = (j + k) % i
    return j + 1


def josephus_simulate(n, k):
    """Simulate the Josephus problem directly for verification."""
    circle = list(range(1, n + 1))
    idx = 0
    while len(circle) > 1:
        idx = (idx + k - 1) % len(circle)
        circle.pop(idx)
        if idx == len(circle):
            idx = 0
    return circle[0]


def solve_sum(N, k):
    """Compute sum of J(n, k) for n=1..N and return (total, positions)."""
    total = 0
    js = []
    for n in range(1, N + 1):
        j = 0
        for i in range(2, n + 1):
            j = (j + k) % i
        j += 1  # 1-indexed
        total += j
        js.append(j)
    return total, js

# Verification

# Verify against brute force for small cases
for n in range(1, 30):
    assert josephus_iterative(n, 3) == josephus_simulate(n, 3), \
        f"Mismatch at n={n}: {josephus_iterative(n, 3)} vs {josephus_simulate(n, 3)}"

# Known values: J(1,3)=1, J(2,3)=2, J(3,3)=2, J(4,3)=1
assert josephus_iterative(1, 3) == 1
assert josephus_iterative(2, 3) == 2
assert josephus_iterative(3, 3) == 2
assert josephus_iterative(4, 3) == 1

answer, js = solve_sum(10000, 3)
print(answer)
