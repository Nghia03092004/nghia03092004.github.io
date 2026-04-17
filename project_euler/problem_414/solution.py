"""
Project Euler Problem 414: Kaprekar Constant

Define C_b to be the Kaprekar constant in base b for 5 digits.
Define sb(i) = 0 if i = C_b or all digits identical, else number of iterations to reach C_b.
S(b) = sum of sb(i) for 0 < i < b^5.

Given: S(15) = 5274369, S(111) = 400668930299.
Find: last 18 digits of sum of S(6k+3) for 2 <= k <= 300.

Approach:
- For each base b, the Kaprekar routine on 5-digit numbers maps sorted digit tuples.
- D - A = (d1-d5)*(b^4-1) + (d2-d4)*(b^3-b) where d1>=d2>=d3>=d4>=d5.
- Build Kaprekar graph on sorted multisets, BFS from C_b, weight by multiplicity.
"""

import sys
from collections import defaultdict, deque
from math import factorial, comb

def to_digits(n, b, length=5):
    """Convert number n to base-b digits, padded to given length."""
    digits = []
    for _ in range(length):
        digits.append(n % b)
        n //= b
    return digits  # least significant first

def from_digits(digits, b):
    """Convert base-b digits (least significant first) to number."""
    result = 0
    power = 1
    for d in digits:
        result += d * power
        power *= b
    return result

def kaprekar_step(n, b):
    """Perform one Kaprekar step on n in base b with 5 digits."""
    digits = to_digits(n, b, 5)
    desc = sorted(digits, reverse=True)
    asc = sorted(digits)
    D = from_digits(desc[::-1], b)  # desc is most-significant-first, convert properly
    A = from_digits(asc[::-1], b)

    # Actually, let's be more careful.
    # desc = sorted digits descending: d[0] >= d[1] >= ... >= d[4]
    # The number formed: d[0]*b^4 + d[1]*b^3 + d[2]*b^2 + d[3]*b + d[4]
    D = 0
    for d in desc:
        D = D * b + d
    A = 0
    for d in asc:
        A = A * b + d

    return D - A

def find_kaprekar_constant(b):
    """
    Find the Kaprekar constant for 5-digit numbers in base b.
    Returns the fixed point, or the cycle if no fixed point exists.
    """
    # Start from a non-trivial number
    n = b + 1  # has digits [1, 1, 0, 0, 0] which are not all equal
    seen = {}
    step = 0
    while n not in seen:
        seen[n] = step
        n = kaprekar_step(n, b)
        step += 1
        if step > b ** 5:
            break
    return n  # If it's a fixed point, kaprekar_step(n, b) == n

def sorted_tuple(n, b):
    """Get the sorted digit tuple of n in base b (descending)."""
    digits = to_digits(n, b, 5)
    return tuple(sorted(digits, reverse=True))

def multiplicity(tup):
    """Number of distinct permutations of the digit tuple."""
    n = len(tup)
    result = factorial(n)
    # Count frequency of each digit
    freq = {}
    for d in tup:
        freq[d] = freq.get(d, 0) + 1
    for f in freq.values():
        result //= factorial(f)
    return result

def compute_S_direct(b):
    """
    Compute S(b) directly by iterating all numbers 1..b^5-1.
    Only feasible for small b.
    """
    C_b = find_kaprekar_constant(b)

    # Verify it's a fixed point
    if kaprekar_step(C_b, b) != C_b:
        # It's a cycle, not a fixed point
        # For this problem, we assume fixed points exist for the given bases
        print(f"  Warning: base {b} has a cycle, not a fixed point at {C_b}")
        # Find cycle
        cycle = [C_b]
        n = kaprekar_step(C_b, b)
        while n != C_b:
            cycle.append(n)
            n = kaprekar_step(n, b)
        print(f"  Cycle length: {len(cycle)}, cycle: {cycle}")
        return None

    limit = b ** 5
    total = 0

    # For each number, find iterations to reach C_b
    for i in range(1, limit):
        digits = to_digits(i, b, 5)
        # Check if all digits are identical
        if len(set(digits)) == 1:
            continue
        if i == C_b:
            continue

        n = i
        steps = 0
        while n != C_b:
            n = kaprekar_step(n, b)
            steps += 1
            if steps > limit:
                # Does not converge (cycle)
                steps = 0
                break
        total += steps

    return total

def compute_S_multiset(b):
    """
    Compute S(b) using multiset (sorted tuple) representation.
    Much faster than direct for moderate b.

    Instead of iterating all b^5 numbers, iterate sorted 5-tuples
    and weight by their multiplicity.
    """
    C_b = find_kaprekar_constant(b)

    # Check if C_b is a true fixed point
    if kaprekar_step(C_b, b) != C_b:
        # Handle cycles: find the cycle
        cycle_set = set()
        n = C_b
        while True:
            cycle_set.add(n)
            n = kaprekar_step(n, b)
            if n in cycle_set:
                break
        # sb(i) = 0 for all i in cycle, and iterations to reach cycle for others
        # For simplicity, treat cycle members as "constant" with sb=0
        # This complicates BFS - skip for now
        print(f"  Base {b}: cycle detected, length {len(cycle_set)}")
        # Still compute using direct method if small enough
        if b <= 30:
            return compute_S_direct(b)
        return None

    C_b_tuple = sorted_tuple(C_b, b)

    # Build Kaprekar graph on sorted tuples
    # IMPORTANT: The Kaprekar step D-A from a sorted tuple gives a specific number.
    # If that number has sorted tuple == C_b_tuple but is NOT C_b itself,
    # those tuples need one extra step (the permutation -> C_b step).
    #
    # We handle this by using a two-node approach for C_b_tuple:
    #   - "C_b_exact": the actual number C_b (depth 0, contributes 0)
    #   - "C_b_perm": other permutations of C_b's digits (depth 1, each contributes 1)
    # Tuples whose D-A == C_b go to C_b_exact (effective depth = tuple_depth).
    # Tuples whose D-A != C_b but sorted == C_b_tuple go to C_b_perm (effective depth = tuple_depth + 1).

    # Enumerate all sorted 5-tuples
    graph = {}        # tuple -> its Kaprekar image tuple
    mult = {}         # tuple -> multiplicity
    maps_to_Cb = {}   # tuple -> True if D-A == C_b exactly, False if D-A has C_b's sorted tuple but != C_b

    all_identical = set()

    for d0 in range(b):
        for d1 in range(d0 + 1):
            for d2 in range(d1 + 1):
                for d3 in range(d2 + 1):
                    for d4 in range(d3 + 1):
                        tup = (d0, d1, d2, d3, d4)

                        # Check all identical
                        if d0 == d4:
                            all_identical.add(tup)
                            continue

                        # Compute Kaprekar step
                        desc = list(tup)
                        asc = list(reversed(tup))
                        D = 0
                        for d in desc:
                            D = D * b + d
                        A = 0
                        for d in asc:
                            A = A * b + d
                        result = D - A
                        result_tuple = sorted_tuple(result, b)

                        graph[tup] = result_tuple
                        mult[tup] = multiplicity(tup)
                        maps_to_Cb[tup] = (result == C_b)

    # Build reverse graph, but split C_b_tuple into two virtual nodes:
    #   C_b_tuple + "_exact" (depth 0): the actual C_b
    #   C_b_tuple + "_perm" (depth 1): other permutations of C_b's digits
    #
    # Tuples whose D-A == C_b point to "_exact" node.
    # Tuples whose D-A has C_b_tuple but != C_b point to "_perm" node.
    # "_perm" itself points to "_exact" (one more Kaprekar step).
    #
    # We implement this by adjusting the graph: tuples that map to C_b_tuple
    # but NOT to C_b exactly are redirected to a virtual node "C_b_perm".
    # "C_b_perm" maps to C_b_tuple. This adds 1 to the depth of the entire subtree.

    # Use a sentinel for the virtual "perm" node
    C_b_perm = ("__CB_PERM__",)

    reverse_graph = defaultdict(list)
    for src, dst in graph.items():
        if dst == C_b_tuple and not maps_to_Cb[src]:
            # This tuple's D-A has C_b's sorted digits but is NOT C_b.
            # Redirect to the virtual perm node.
            reverse_graph[C_b_perm].append(src)
        else:
            reverse_graph[dst].append(src)

    # C_b_perm -> C_b_tuple in the forward graph means
    # C_b_tuple -> C_b_perm in the reverse graph
    # (C_b_perm is a "child" of C_b_tuple in the forward direction)
    # Actually: in reverse BFS from C_b_tuple, we want C_b_perm at depth 1.
    reverse_graph[C_b_tuple].append(C_b_perm)

    # BFS
    dist = {C_b_tuple: 0}
    queue = deque([C_b_tuple])
    total = 0

    # Add contribution from other permutations of C_b's digits (sb = 1 for each)
    C_b_mult = multiplicity(C_b_tuple)
    total += (C_b_mult - 1) * 1  # (mult-1) permutations each with sb=1

    while queue:
        node = queue.popleft()
        d = dist[node]
        for parent in reverse_graph[node]:
            if parent not in dist and parent not in all_identical:
                dist[parent] = d + 1
                if parent != C_b_perm:
                    total += (d + 1) * mult[parent]
                # else: C_b_perm is virtual, no multiplicity contribution
                queue.append(parent)

    return total

def compute_S(b):
    """Compute S(b) using the most efficient method available."""
    return compute_S_multiset(b)

def solve():
    """Compute sum of S(6k+3) for 2 <= k <= 300, last 18 digits."""
    MOD = 10 ** 18
    total = 0

    for k in range(2, 301):
        b = 6 * k + 3
        s = compute_S(b)
        if s is not None:
            total += s
            if k % 20 == 0 or k <= 5:
                print(f"  k={k}, b={b}, S(b)={s}, running_total mod 10^18 = {total % MOD}")
        else:
            print(f"  k={k}, b={b}: FAILED (cycle detected)")

    print(f"\nAnswer (last 18 digits): {total % MOD}")
    return total % MOD

def create_visualization():
    """Create visualization of the Kaprekar routine and save as PNG."""
    print("\nGenerating visualization...")
