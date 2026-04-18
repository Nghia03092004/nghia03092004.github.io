# Problem 96: Su Doku

## Problem Statement

A Sudoku puzzle requires filling a $9 \times 9$ grid so that each row, each column, and each of the nine $3 \times 3$ sub-boxes contains every digit from 1 to 9 exactly once. Given fifty Su Doku puzzles, solve all of them and compute the sum of the three-digit numbers formed by the first three cells of the top row in each solution.

## Mathematical Foundation

**Definition 1 (Sudoku as a CSP).** A Sudoku instance is a constraint satisfaction problem (CSP) on the grid $G = \{0, 1, \ldots, 8\}^2$ with:
- **Variables:** $v_{r,c}$ for each $(r, c) \in G$.
- **Domains:** $D_{r,c} \subseteq \{1, 2, \ldots, 9\}$, initialized to $\{g_{r,c}\}$ if cell $(r,c)$ is given, or $\{1, \ldots, 9\}$ otherwise.
- **Constraints:** For each of the 27 *units* (9 rows, 9 columns, 9 boxes), all variables in the unit must take pairwise distinct values.

**Definition 2 (Peers).** The *peers* of cell $(r, c)$ are all cells sharing a row, column, or box with $(r, c)$, excluding $(r, c)$ itself. Each cell has exactly 20 peers.

**Theorem 1 (Soundness of arc-consistency propagation).** If $v_{r,c}$ is assigned value $d$, then removing $d$ from the domain $D_{r',c'}$ of every peer $(r', c')$ preserves all solutions.

**Proof.** Let $\mathcal{S}$ be any valid completion. In $\mathcal{S}$, every unit containing $(r, c)$ has $v_{r,c} = d$, and by the all-different constraint, no other variable in that unit equals $d$. Hence $d \notin \{v_{r',c'}\}$ for any peer $(r', c')$, so $v_{r',c'} \in D_{r',c'} \setminus \{d\}$ in every valid completion. $\square$

**Lemma 1 (Naked single).** If $|D_{r,c}| = 1$ after propagation, say $D_{r,c} = \{d\}$, then $v_{r,c} = d$ in every valid completion.

**Proof.** The variable must take a value in its domain. Since the domain is a singleton, the assignment is forced. $\square$

**Lemma 2 (Hidden single).** If, within some unit $U$, a value $d$ appears in exactly one cell's domain, say $D_{r,c}$, then $v_{r,c} = d$ in every valid completion.

**Proof.** The all-different constraint requires every value in $\{1, \ldots, 9\}$ to appear in $U$. If $d$ can only be placed in cell $(r, c)$, then $v_{r,c} = d$ is forced. $\square$

**Theorem 2 (Completeness of backtracking with propagation).** Depth-first backtracking search, combined with constraint propagation (naked singles and hidden singles), is a complete algorithm: it finds a solution if one exists, and correctly reports unsatisfiability otherwise.

**Proof.** The algorithm explores a search tree where each node is a partial assignment consistent with the constraints (after propagation). At each node, an unassigned variable is selected and each value in its current domain is tried. Constraint propagation (Theorem 1, Lemmas 1--2) removes only values that cannot appear in any valid completion extending the current partial assignment. If propagation empties some domain, the current branch is infeasible and we backtrack. Since the search tree is finite (depth $\le 81$, branching factor $\le 9$), the algorithm terminates. If a solution exists, at least one root-to-leaf path in the (unpruned) tree leads to it; propagation never removes values that appear in a valid completion, so this path survives pruning. $\square$

**Theorem 3 (MRV heuristic).** Selecting the unassigned cell with the minimum remaining values (smallest $|D_{r,c}|$ among unassigned cells) minimizes the branching factor at each decision point, thereby reducing the size of the explored search tree in expectation.

**Proof.** At a branch point where the selected variable has $b$ remaining values, the search explores at most $b$ subtrees. Choosing the variable with the smallest $b$ yields the smallest immediate branching factor. While globally optimal variable ordering is NP-hard to determine, the MRV (or "fail-first") heuristic is provably optimal for a single decision under the assumption of uniform subtree sizes, and performs near-optimally in practice on Sudoku. $\square$

## Editorial
Solve all 50 Sudoku puzzles and find the sum of the 3-digit numbers from the top-left corner of each solution. Algorithm: Constraint propagation (naked singles + hidden singles) with backtracking search using the MRV heuristic.

## Pseudocode

```text
    Initialize D[r][c] for all cells from givens
    Propagate constraints (naked singles + hidden singles)
    Return BACKTRACK(D)

    if all cells assigned: return solution
    if any D[r][c] is empty: return FAILURE
    (r*, c*) = argmin |D[r][c]| among unassigned cells // MRV
    For each each d in D[r*][c*]:
        save state
        assign d to (r*, c*); propagate
        result = BACKTRACK(D)
        if result != FAILURE: return result
        restore state
    Return FAILURE
```

## Complexity Analysis

**Time:** Worst-case $O(9^{81})$ for an empty grid (all branches explored). In practice, constraint propagation reduces well-posed Sudoku puzzles to search trees of depth $\le 5$ with branching factor $\le 3$, so each puzzle solves in constant time. For 50 puzzles: $O(1)$.

**Space:** $O(81 \times 9) = O(1)$ for domain sets. Backtracking stack depth $\le 81$ with $O(81 \times 9)$ state per level: $O(1)$.

## Answer

$$\boxed{24702}$$
