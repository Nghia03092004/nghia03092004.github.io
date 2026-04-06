# Problem 260: Stone Game

## Problem Statement

A game is played with three piles of stones. On each turn, a player may:

1. Remove any positive number of stones from **one** pile, or
2. Remove an equal positive number of stones from any **two** piles.

The player who takes the last stone wins. A position $(a, b, c)$ with $0 \le a \le b \le c$ is a **P-position** (losing for the player to move) if the previous player wins under optimal play.

Find $\displaystyle\sum_{\substack{(a,b,c) \in \mathcal{P} \\ 0 \le a \le b \le c \le 1000}} (a + b + c)$, where $\mathcal{P}$ is the set of P-positions.

## Mathematical Foundation

**Definition (Sprague--Grundy Classification).** In an impartial combinatorial game, a position is a *P-position* if every move leads to an N-position, and an *N-position* if some move leads to a P-position. The terminal position $(0,0,0)$ is a P-position.

**Theorem 1 (Move Set).** *From position $(a, b, c)$ with $a \le b \le c$, the available moves produce (after re-sorting) all positions of the form:*

1. *$(a', b, c)$ for $0 \le a' < a$; $(a, b', c)$ for $0 \le b' < b$; $(a, b, c')$ for $0 \le c' < c$ (single-pile removal).*
2. *$(a-k, b-k, c)$ for $1 \le k \le a$; $(a-k, b, c-k)$ for $1 \le k \le a$; $(a, b-k, c-k)$ for $1 \le k \le b$ (equal removal from two piles).*

*All resulting triples are re-sorted to maintain $a' \le b' \le c'$.*

**Proof.** These are all positions reachable by removing $k \ge 1$ stones from one pile or $k \ge 1$ stones from each of two chosen piles. The re-sorting ensures canonical form. $\square$

**Theorem 2 (Monotone Computation).** *The P/N classification can be computed in order of non-decreasing $a + b + c$, since every move strictly decreases the total stone count.*

**Proof.** Single-pile removal decreases the total by $k \ge 1$. Two-pile removal decreases the total by $2k \ge 2$. Hence all successors of a position with total $T$ have total $< T$, and the classification of all positions with total $< T$ is known when processing total $T$. $\square$

**Lemma 1 (P-position Characterization).** *A position $(a, b, c)$ is a P-position if and only if:*
- *No single-pile move leads to a P-position: for all $a' < a$, the sorted triple $(a', b, c)$ is an N-position; and similarly for reducing $b$ or $c$.*
- *No two-pile move leads to a P-position: for all $k \ge 1$, the sorted triples $(a-k, b-k, c)$, $(a-k, b, c-k)$, $(a, b-k, c-k)$ are all N-positions.*

**Proof.** This is exactly the definition: a P-position has no move to a P-position. $\square$

**Theorem 3 (Efficient Detection via P-position Lookup).** *For each candidate position $(a, b, c)$, checking the P-position condition requires examining $O(c)$ successors (at most $a + b + c$ moves). Using a 3D boolean array, each lookup is $O(1)$.*

**Proof.** The number of single-pile moves is $(a - 0) + (b - 0) + (c - 0) = a + b + c$. The number of two-pile moves is $a + a + b = 2a + b$. Total moves: $O(a + b + c) = O(c)$ since $c \ge b \ge a$. Each successor lookup in the precomputed boolean array is $O(1)$. $\square$

## Algorithm

```
function SumPPositions(N):
    // N = 1000
    // is_P[a][b][c] = true if (a,b,c) is a P-position (a <= b <= c)
    Initialize is_P[0..N][0..N][0..N] = false

    total = 0

    for a = 0 to N:
        for b = a to N:
            for c = b to N:
                is_p = true

                // Check single-pile removals
                for a' = 0 to a-1:  // reduce pile a
                    (x,y,z) = sort(a', b, c)
                    if is_P[x][y][z]: is_p = false; break
                if not is_p: mark N-position; continue

                for b' = 0 to b-1:  // reduce pile b
                    (x,y,z) = sort(a, b', c)
                    if is_P[x][y][z]: is_p = false; break
                if not is_p: mark N-position; continue

                for c' = 0 to c-1:  // reduce pile c
                    (x,y,z) = sort(a, b, c')
                    if is_P[x][y][z]: is_p = false; break
                if not is_p: mark N-position; continue

                // Check two-pile removals
                for k = 1 to a:
                    (x,y,z) = sort(a-k, b-k, c)
                    if is_P[x][y][z]: is_p = false; break
                if not is_p: mark N-position; continue

                for k = 1 to a:
                    (x,y,z) = sort(a-k, b, c-k)
                    if is_P[x][y][z]: is_p = false; break
                if not is_p: mark N-position; continue

                for k = 1 to b:
                    (x,y,z) = sort(a, b-k, c-k)
                    if is_P[x][y][z]: is_p = false; break
                if not is_p: mark N-position; continue

                // All moves lead to N-positions => P-position
                is_P[a][b][c] = true
                total += a + b + c

    return total
```

## Complexity Analysis

- **Time:** $O(N^3)$ positions, each requiring $O(N)$ move checks with early termination. Worst case: $O(N^4)$. In practice, early termination (most positions are N-positions, detected quickly) reduces the constant significantly. For $N = 1000$: approximately $1.67 \times 10^8$ positions with average $O(1)$ to $O(N)$ checks each.
- **Space:** $O(N^3)$ for the boolean array. With $N = 1000$: $\binom{1003}{3} \approx 1.67 \times 10^8$ bits $\approx 20$ MB.

## Answer

$$\boxed{167542057}$$
