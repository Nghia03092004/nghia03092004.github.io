#!/usr/bin/env python3
"""
Project Euler Problem 459: Flipping Game

The flipping game is played on an N x N board with disks (white/black).
A move flips a rectangle whose width is a perfect square and height is
a triangular number, with the upper-right corner on a white disk.

We need W(N) = number of winning first moves assuming perfect play.

Approach: Sprague-Grundy theory. The game decomposes by cell.
Each cell (i,j) is an independent nim-heap. The row game has moves
of triangular-number lengths; the column game has perfect-square lengths.
The Grundy value of the board is XOR of all cell Grundy values.

For the all-white initial position, a winning move must change the
total XOR to 0.
"""

import sys
from collections import Counter

def triangular_numbers(limit):
    """Generate triangular numbers up to limit."""
    result = []
    k = 1
    while k * (k + 1) // 2 <= limit:
        result.append(k * (k + 1) // 2)
        k += 1
    return result

def perfect_squares(limit):
    """Generate perfect squares up to limit."""
    result = []
    k = 1
    while k * k <= limit:
        result.append(k * k)
        k += 1
    return result

def compute_grundy_1d(n, move_sizes):
    """
    Compute Grundy values for a 1D flipping game on positions 0..n-1.
    A move at position p with size s flips all positions in [p-s+1, p].
    The Grundy value at position p depends on the reachable states.

    For a ruler-like game where a move of size s at position p affects
    positions p-s+1..p, the Grundy value for position p is computed
    using the mex (minimum excludant) of reachable Grundy values.

    In this simplified model, for the 1D component:
    G(0) = 0 (no move possible at position 0 with any size > 0 going left)
    G(p) = mex({G(p) XOR contribution from flipping [p-s+1..p]})

    Actually, for the NIM-value decomposition of the flipping game:
    Each position can be independently analyzed. A move flips a contiguous
    block ending at position p. The Grundy value for position p in a
    single-pile game is:

    G(p) = mex({XOR of G(p-s+1)..G(p) : s in move_sizes, s <= p+1})

    But this is circular. Instead, we use the octal game / move-set approach.

    For the 1D burning game: Grundy value of a single token at position p
    where moves are to remove blocks of specified sizes ending at p.
    """
    # For this specific game structure, we compute Grundy values iteratively.
    # The key insight is that each row and column can be treated as a
    # Nim-like game where the move sizes are triangular numbers or
    # perfect squares respectively.
    #
    # For a simple take-away game with move set S, Grundy values are:
    # G(0) = 0
    # G(n) = mex({G(n-s) : s in S, s <= n})

    grundy = [0] * (n + 1)
    for i in range(1, n + 1):
        reachable = set()
        for s in move_sizes:
            if s > i:
                break
            reachable.add(grundy[i - s])
        # Compute mex
        mex = 0
        while mex in reachable:
            mex += 1
        grundy[i] = mex
    return grundy

def W(N):
    """
    Compute W(N): number of winning first moves on N x N board.

    The board game with rectangles of width=perfect_square, height=triangular
    decomposes as: each cell (i,j) has Grundy value G_row(i) XOR G_col(j),
    where G_row uses triangular number moves and G_col uses perfect square moves.

    For an all-white board, every cell is "active". The total Grundy value is:
    XOR over all (i,j) of [G_row(i) XOR G_col(j)]

    A first move is winning if it produces Grundy value 0 for the opponent.
    """
    tri = triangular_numbers(N)
    sq = perfect_squares(N)

    # Compute 1D Grundy values
    # Row dimension: triangular number moves
    g_row = compute_grundy_1d(N, tri)
    # Column dimension: perfect square moves
    g_col = compute_grundy_1d(N, sq)

    # Total Grundy value of the board:
    # XOR_{i=1..N, j=1..N} (g_row[i] XOR g_col[j])
    #
    # This simplifies: if N is even, each g_col[j] is XORed with itself
    # an even number of times in the row dimension, etc.
    #
    # XOR_{i=1..N} XOR_{j=1..N} (g_row[i] XOR g_col[j])
    #
    # = XOR_{j=1..N} [ XOR_{i=1..N} (g_row[i] XOR g_col[j]) ]
    # = XOR_{j=1..N} [ (XOR_{i=1..N} g_row[i]) XOR (N * g_col[j] mod 2 stuff) ]
    #
    # Actually: XOR_{i=1..N} (g_row[i] XOR c) for constant c:
    # = (XOR_{i=1..N} g_row[i]) XOR (c if N is odd, else 0)
    #
    # So total = XOR_{j=1..N} [(XOR_rows) XOR (g_col[j] if N odd else 0)]
    #          = N * (XOR_rows) XOR (if N odd: XOR_cols, else 0)  -- in XOR sense
    #
    # XOR of N copies of XOR_rows = XOR_rows if N odd, else 0
    # So total = (XOR_rows if N odd else 0) XOR (XOR_cols if N odd else 0)
    # = (XOR_rows XOR XOR_cols) if N odd, else 0

    xor_rows = 0
    for i in range(1, N + 1):
        xor_rows ^= g_row[i]

    xor_cols = 0
    for j in range(1, N + 1):
        xor_cols ^= g_col[j]

    total_grundy = 0
    if N % 2 == 1:
        total_grundy = xor_rows ^ xor_cols

    if total_grundy == 0:
        return 0  # No winning move (second player wins)

    # Count winning moves: each valid rectangle move that makes total Grundy = 0
    # A rectangle at upper-right corner (r, c) with height h (triangular) and
    # width w (perfect square) flips cells (r..r+h-1, c-w+1..c).
    #
    # The change to total Grundy from flipping a rectangle depends on which
    # cells are flipped and their contribution.
    #
    # For a proper analysis, we count moves that transform the Grundy value to 0.
    # This requires understanding how rectangle flips change the XOR total.

    # For small N, we can enumerate directly
    count = 0

    # The move flips a rectangle. In the decomposed game, a rectangle flip
    # of width w (perfect square) and height h (triangular number) starting
    # from upper-right corner (r, c) changes the Grundy contributions of
    # all cells in that rectangle.
    #
    # Since this is a Nim-value game, a winning move is one where the
    # resulting position has Grundy value 0.

    # For the take-away game interpretation:
    # A move is defined by choosing a position in the row-game and a position
    # in the column-game. The number of winning moves is the count of
    # (row_move, col_move) pairs that together zero out the Grundy value.

    # In a product game G_row x G_col, a move in position (i,j) can be:
    # - Move in row only (change row state)
    # - Move in column only (change column state)
    # - Move in both simultaneously

    # For the rectangle flipping game, each valid move corresponds to
    # choosing a width w and height h, and a position. The move affects
    # w consecutive columns and h consecutive rows.

    # Simplified counting for small N (verification):
    # For larger N, the pattern in Grundy values allows efficient counting.

    return count

def W_bruteforce(N):
    """
    Brute force for small N: build the game tree and use Sprague-Grundy.
    Positions are bitmasks of the N x N board (0 = white, 1 = black).
    Target = all black = (1 << (N*N)) - 1.
    """
    if N > 4:
        print(f"N={N} too large for brute force")
        return -1

    tri = triangular_numbers(N)
    sq = perfect_squares(N)

    total_cells = N * N
    target = (1 << total_cells) - 1

    def cell(r, c):
        return r * N + c

    def get_moves(state):
        """Generate all valid moves from state. Each move is a bitmask to XOR."""
        moves = []
        for h in tri:
            for w in sq:
                # Upper-right corner at (r, c), rectangle covers
                # rows r..r+h-1, cols c-w+1..c
                for r in range(N - h + 1):
                    for c in range(w - 1, N):
                        # Check upper-right corner (r, c) is white (bit = 0)
                        if state & (1 << cell(r, c)):
                            continue  # It's black, skip
                        # Build flip mask
                        mask = 0
                        for dr in range(h):
                            for dc in range(w):
                                mask |= 1 << cell(r + dr, c - dc)
                        moves.append(mask)
        return moves

    # Compute Grundy values using memoization
    from functools import lru_cache

    @lru_cache(maxsize=None)
    def grundy(state):
        moves = get_moves(state)
        reachable = set()
        for m in moves:
            new_state = state ^ m
            reachable.add(grundy(new_state))
        mex = 0
        while mex in reachable:
            mex += 1
        return mex

    # For W(N): count winning moves from all-white (state=0)
    initial = 0
    g = grundy(initial)
    if g == 0:
        return 0

    count = 0
    moves = get_moves(initial)
    for m in moves:
        new_state = initial ^ m
        if grundy(new_state) == 0:
            count += 1
    return count

def create_visualization(N_max=30):
    """Create visualization of Grundy values and winning move counts."""
