"""
Problem 497: Drunken Tower of Hanoi
Expected number of moves when each move is chosen uniformly at random among legal moves.
"""

from fractions import Fraction
from collections import defaultdict

def encode_state(pegs: tuple):
    """Encode the state as the tuple of peg assignments for each disk."""
    return pegs

def get_peg_tops(pegs: tuple, n: int) -> dict:
    """For each peg, find the smallest disk on it (the top disk).
    Disks are numbered 1..n (1 = smallest).
    """
    tops = {}  # peg -> smallest disk number
    for disk in range(1, n + 1):
        p = pegs[disk - 1]
        if p not in tops:
            tops[p] = disk
    return tops

def get_legal_moves(pegs: tuple, n: int) -> list:
    """Return list of (disk, from_peg, to_peg) for all legal moves."""
    tops = get_peg_tops(pegs, n)
    moves = []
    for from_peg, disk in tops.items():
        for to_peg in [1, 2, 3]:
            if to_peg == from_peg:
                continue
            # Check if move is legal: to_peg is empty or top of to_peg > disk
            if to_peg not in tops or tops[to_peg] > disk:
                moves.append((disk, from_peg, to_peg))
    return moves

def apply_move(pegs: tuple, disk: int, to_peg: int):
    """Apply a move: move disk to to_peg."""
    lst = list(pegs)
    lst[disk - 1] = to_peg
    return tuple(lst)

def solve_exact(n: int) -> Fraction:
    """Solve E(n) exactly using Markov chain on all 3^n states.
    Uses value iteration with exact Fraction arithmetic.
    """
    # Initial state: all disks on peg 1
    initial = tuple([1] * n)
    # Goal state: all disks on peg 3
    goal = tuple([3] * n)

    if n == 0:
        return Fraction(0)

    # Enumerate all reachable states via BFS
    from collections import deque
    visited = set()
    queue = deque([initial])
    visited.add(initial)
    while queue:
        state = queue.popleft()
        for disk, from_peg, to_peg in get_legal_moves(state, n):
            new_state = apply_move(state, disk, to_peg)
            if new_state not in visited:
                visited.add(new_state)
                queue.append(new_state)

    # All states are reachable (3^n states for Tower of Hanoi)
    states = sorted(visited)
    state_idx = {s: i for i, s in enumerate(states)}
    num_states = len(states)
    goal_idx = state_idx[goal]

    # Set up linear system: E[s] = 1 + (1/|legal(s)|) * sum E[s'] for non-goal states
    # E[goal] = 0
    # Rearranging: E[s] - (1/|legal(s)|) * sum E[s'] = 1

    # Use iterative value computation with Fractions (for small n)
    # For n <= 4, this is feasible

    # Build adjacency
    adj = {}  # state_idx -> [(neighbor_idx, ...)]
    for s in states:
        idx = state_idx[s]
        moves = get_legal_moves(s, n)
        neighbors = []
        for disk, from_peg, to_peg in moves:
            ns = apply_move(s, disk, to_peg)
            neighbors.append(state_idx[ns])
        adj[idx] = neighbors

    # Solve using Gaussian elimination on the system:
    # For each non-goal state i: E[i] - (1/deg_i) * sum_{j in neighbors(i)} E[j] = 1
    # E[goal_idx] = 0

    # Remove goal from unknowns
    non_goal = [i for i in range(num_states) if i != goal_idx]
    idx_map = {old: new for new, old in enumerate(non_goal)}
    m = len(non_goal)

    # Build sparse system
    # A[row] * x = b[row], where x[k] = E[non_goal[k]]
    A = [[Fraction(0)] * m for _ in range(m)]
    b = [Fraction(1)] * m

    for row_new, i in enumerate(non_goal):
        neighbors = adj[i]
        deg = len(neighbors)
        A[row_new][row_new] = Fraction(1)
        for j in neighbors:
            if j == goal_idx:
                continue  # E[goal] = 0, no contribution
            col_new = idx_map[j]
            A[row_new][col_new] -= Fraction(1, deg)

    # Gaussian elimination
    for col in range(m):
        # Find pivot
        pivot = None
        for row in range(col, m):
            if A[row][col] != 0:
                pivot = row
                break
        if pivot is None:
            continue
        A[col], A[pivot] = A[pivot], A[col]
        b[col], b[pivot] = b[pivot], b[col]

        scale = A[col][col]
        for j in range(col, m):
            A[col][j] /= scale
        b[col] /= scale

        for row in range(m):
            if row == col:
                continue
            if A[row][col] != 0:
                factor = A[row][col]
                for j in range(col, m):
                    A[row][j] -= factor * A[col][j]
                b[row] -= factor * b[col]

    # Extract E values
    E = {}
    for row_new, i in enumerate(non_goal):
        E[i] = b[row_new]
    E[goal_idx] = Fraction(0)

    initial_idx = state_idx[initial]
    return E[initial_idx]

# Compute for small n
print("Drunken Tower of Hanoi - Expected moves:")
for n in range(1, 5):
    e = solve_exact(n)
    print(f"  E({n}) = {e} = {float(e):.6f}")
    print(f"         Optimal = {2**n - 1}")
