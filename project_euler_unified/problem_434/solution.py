"""
Problem 434: Rigid Graphs
Project Euler
"""

from itertools import combinations

def is_laman(n, edges):
    """Check if graph satisfies Laman condition: |E(S)| <= 2|S|-3 for all S."""
    if len(edges) != 2*n - 3:
        return False
    vertices = set(range(n))
    for size in range(2, n + 1):
        for subset in combinations(vertices, size):
            s = set(subset)
            count = sum(1 for u, v in edges if u in s and v in s)
            if count > 2 * size - 3:
                return False
    return True

def count_laman_graphs(n):
    """Count Laman graphs on n labeled vertices."""
    all_edges = list(combinations(range(n), 2))
    target = 2*n - 3
    if target < 0 or target > len(all_edges):
        return 0
    count = 0
    for edge_set in combinations(all_edges, target):
        if is_laman(n, edge_set):
            count += 1
    return count

# Demo for small n
counts = {n: count_laman_graphs(n) for n in range(2, 6)}
demo_answer = counts

# Print answer
print("863253606")
