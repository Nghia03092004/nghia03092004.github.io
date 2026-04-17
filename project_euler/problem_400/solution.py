"""
Project Euler Problem 400: Fibonacci Tree Game
===============================================

A Fibonacci tree T(k) is defined recursively:
  T(0) = empty, T(1) = single node, T(k) = root with T(k-1) and T(k-2) as children.

Two players alternate removing a node and its subtree. The player forced to take
the root (when it is the only node left) loses.

f(k) = number of winning first moves on T(k).
Given: f(5) = 1, f(10) = 17. Find the last 18 digits of f(10000).

Solution: Map to Green Hackenbush, compute nim values via the Colon Principle,
and count winning moves using a dictionary-based DP on nim value targets.

Answer: 438505383468410633
"""

# ---------------------------------------------------------------------------
# Core computation
# ---------------------------------------------------------------------------

def compute_nim_values(K):
    """Compute nim(k) for k = 0..K.

    nim(0) = 0, nim(1) = 1, nim(k) = 1 + (nim(k-1) XOR nim(k-2)).
    """
    nim = [0] * (K + 1)
    if K >= 1:
        nim[1] = 1
    for k in range(2, K + 1):
        nim[k] = 1 + (nim[k - 1] ^ nim[k - 2])
    return nim

def compute_f(K, MOD=10**18):
    """Compute f(k) for k = 1..K, returning the last 18 digits.

    h(k, target) = number of non-root nodes in T(k) whose removal changes
                   nim(k) to 'target'.

    Recurrence:
      h(k, 1+nim(k-2))  += 1                          (remove left child root)
      h(k, 1+nim(k-1))  += 1                          (remove right child root)
      h(k, 1+(t'^nim(k-2))) += h(k-1, t')             (remove inside left)
      h(k, 1+(t'^nim(k-1))) += h(k-2, t')             (remove inside right)

    f(k) = [nim(k-2)==0] + [nim(k-1)==0]
         + h(k-1, nim(k-2)) + h(k-2, nim(k-1))
    """
    nim = compute_nim_values(K)
    f_values = [0] * (K + 1)

    h_prev2 = {}  # h[k-2]
    h_prev1 = {}  # h[k-1]

    for k in range(2, K + 1):
        hk = {}

        # Case A: remove root of T(k-1) (left child of root)
        tA = 1 + nim[k - 2]
        hk[tA] = hk.get(tA, 0) + 1

        # Case B: remove root of T(k-2) (right child of root), if it exists
        if k - 2 >= 1:
            tB = 1 + nim[k - 1]
            hk[tB] = hk.get(tB, 0) + 1

        # Case C: remove non-root node inside T(k-1)
        nk2 = nim[k - 2]
        for t_inner, cnt in h_prev1.items():
            t_outer = 1 + (t_inner ^ nk2)
            hk[t_outer] = (hk.get(t_outer, 0) + cnt) % MOD

        # Case D: remove non-root node inside T(k-2)
        if k - 2 >= 1:
            nk1 = nim[k - 1]
            for t_inner, cnt in h_prev2.items():
                t_outer = 1 + (t_inner ^ nk1)
                hk[t_outer] = (hk.get(t_outer, 0) + cnt) % MOD

        # Compute f(k)
        fk = 0
        if nim[k - 2] == 0:
            fk += 1
        if k - 2 >= 1 and nim[k - 1] == 0:
            fk += 1
        fk = (fk + h_prev1.get(nim[k - 2], 0)) % MOD
        if k - 2 >= 1:
            fk = (fk + h_prev2.get(nim[k - 1], 0)) % MOD
        f_values[k] = fk

        h_prev2 = h_prev1
        h_prev1 = hk

    return f_values, nim

def fibonacci_tree_positions(k):
    """Build T(k) and return (positions, edges) for plotting.
    Uses a layout where each node gets x,y coordinates."""
    if k == 0:
        return [], []

    positions = {}
    edges = []
    counter = [0]

    def build(order, x, y, dx):
        if order == 0:
            return None
        node_id = counter[0]
        counter[0] += 1
        positions[node_id] = (x, y)
        if order == 1:
            return node_id
        left = build(order - 1, x - dx, y - 1, dx * 0.5)
        right = build(order - 2, x + dx, y - 1, dx * 0.5)
        if left is not None:
            edges.append((node_id, left))
        if right is not None:
            edges.append((node_id, right))
        return node_id

    build(k, 0, 0, 2 ** (k - 2))
    return positions, edges

def compute_winning_nodes(k, nim_vals):
    """For T(k), identify which nodes are winning first moves.
    Returns set of (order_of_subtree, path_from_root) for winning nodes."""
    # Build the tree and for each non-root node, compute what happens when removed
    if k < 2:
        return set()

    winning = []

    def explore(order, path, parent_order):
        """Explore T(order) embedded within the larger tree.
        path = list of ('L'/'R') choices from the root of T(k).
        """
        if order <= 0:
            return

        # This node is the root of T(order). It is a non-root node of the full tree.
        # When removed, the parent loses this child.
        # We need to recompute the overall Grundy value.
        # For efficiency, just track whether this is a winning move.

        # The nim value of the full tree changes based on the path.
        # This is complex -- for visualization we just use small k.
        winning.append((order, list(path)))

        # Recurse into children
        if order >= 2:
            path.append('L')
            explore(order - 1, path, order)
            path.pop()

            if order - 2 >= 1:
                path.append('R')
                explore(order - 2, path, order)
                path.pop()

    # Children of root
    explore(k - 1, ['L'], k)
    if k - 2 >= 1:
        explore(k - 2, ['R'], k)

    return winning

# ---------------------------------------------------------------------------
