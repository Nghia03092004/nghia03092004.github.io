"""
Problem 669: The King's Banquet

Find the knight in the m-th chair from the king's left,
where n = F_86 - 1 knights sit at a round table with
Fibonacci-sum adjacency constraint.
"""

def fibonacci_list(k):
    """Compute Fibonacci numbers F_1, ..., F_k."""
    fib = [0, 1, 1]
    for i in range(3, k + 1):
        fib.append(fib[-1] + fib[-2])
    return fib

# Compute Fibonacci numbers
fib = fibonacci_list(90)
print(f"F_86 = {fib[86]}")
n = fib[86] - 1
print(f"n = {n}")
print(f"Verify: n = F_86 - 1 = {fib[86] - 1}")

target_pos = 10**16
print(f"Target position: {target_pos}")

# Recursive path finder
def find_knight(k, m, fib):
    """Find the m-th knight (1-indexed) in the Hamiltonian path
    on {1, ..., F_k - 1} of the Fibonacci-sum graph.

    Returns the knight number.
    """
    if k <= 2:
        return m  # base case: {1} or small set

    n_lower = fib[k-2] - 1  # size of lower interval
    n_upper = fib[k-1]       # size of upper interval (approx)

    # The path interleaves lower and upper sub-paths
    # This is a simplified model - exact interleaving depends on problem specifics
    if m <= n_lower:
        return find_knight(k - 2, m, fib)
    else:
        return fib[k-2] - 1 + find_knight(k - 1, m - n_lower, fib)

# Small test
for k in range(3, 8):
    n_k = fib[k] - 1
    print(f"k={k}, n={n_k}, path:", end=" ")
    for m in range(1, n_k + 1):
        print(find_knight(k, m, fib), end=" ")
    print()

# The full solution requires the exact interleaving rule
# which depends on the specific Fibonacci-sum graph structure
print(f"\nFull solution requires exact Fibonacci path construction for k=86.")
