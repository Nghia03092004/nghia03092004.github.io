# Problem 424: Kakuro

## Problem Statement

A Kakuro puzzle is a cross-sum puzzle on a grid where blank cells must be filled with digits $1$--$9$ such that each horizontal and vertical run of consecutive blank cells sums to a given clue, with no digit repeated within any run. Given a collection of Kakuro puzzles, solve each and compute the sum of the 3-digit numbers formed by specific cells.

## Mathematical Foundation

**Theorem 1 (Constraint Characterization).** For a run of length $\ell$ with target sum $s$, the set of valid digit assignments is in bijection with the subsets
$$\mathcal{C}(\ell, s) = \{D \subseteq \{1, \ldots, 9\} : |D| = \ell \text{ and } \textstyle\sum_{d \in D} d = s\}.$$

**Proof.** A valid assignment to a run of length $\ell$ uses $\ell$ distinct digits from $\{1, \ldots, 9\}$ summing to $s$. Each such assignment corresponds to a subset $D \in \mathcal{C}(\ell, s)$ together with a permutation of the elements of $D$ into the run's cells. The subset $D$ characterizes which digits appear; the arrangement is then determined by further constraints from crossing runs. $\square$

**Theorem 2 (Finiteness and Uniqueness).** Each well-posed Kakuro puzzle has exactly one solution.

**Proof.** The search space is finite: each cell has at most 9 possible values, and there are finitely many cells. The puzzle constraints (sum and distinctness within runs) define a system of constraints over this finite domain. By the design guarantee of Kakuro puzzles, this system has a unique satisfying assignment. $\square$

**Lemma 1 (Arc Consistency).** For a cell $c$ at the intersection of a horizontal run $H$ and a vertical run $V$, the domain of $c$ is
$$\mathrm{dom}(c) = \bigcup_{D_H \in \mathcal{C}(\ell_H, s_H)} D_H \;\cap\; \bigcup_{D_V \in \mathcal{C}(\ell_V, s_V)} D_V.$$
Constraint propagation iteratively reduces these domains: when a cell is assigned a value, that value is removed from the domains of all cells sharing a run, and any run-subset that includes an impossible value is eliminated.

**Proof.** The value of $c$ must appear in some valid subset for $H$ and some valid subset for $V$. Taking the intersection of the unions of these subsets gives all feasible values. Propagation preserves soundness because removing a value that cannot participate in any valid completion does not eliminate any solution. $\square$

**Lemma 2 (Backtracking Termination).** The backtracking search with constraint propagation terminates in finite time and finds the unique solution if one exists.

**Proof.** At each branching point, we select a cell with $|\mathrm{dom}(c)| \ge 2$ and try each value. Each guess strictly reduces the search space (either by fixing a cell or by causing a domain wipeout that triggers backtracking). Since the domain is finite and each branch reduces it, the search tree is finite. $\square$

## Editorial
Project Euler. We parse grid into cells and runs. We then initialize domains. Finally, iterate over each cell c. We perform a recursive search over the admissible choices, prune branches that violate the derived constraints, and keep only the candidates that satisfy the final condition.

## Pseudocode

```text
Parse grid into cells and runs
Initialize domains
for each cell c
for each cell c in run
Constraint propagation + backtracking
while changed
Remove subsets incompatible with current domains
If a cell has |dom| = 1, remove that digit from peers
Update domains; set changed = true if any domain shrinks
for each puzzle
```

## Complexity Analysis

- **Time:** Worst case $O(9^n)$ where $n$ is the number of blank cells. In practice, constraint propagation reduces this to near-linear for well-constrained Kakuro puzzles. The preprocessing of valid subsets $\mathcal{C}(\ell, s)$ takes $O(2^9) = O(512)$ per run.
- **Space:** $O(n \cdot 9)$ for cell domains, plus $O(r \cdot 2^9)$ for storing valid subsets across $r$ runs.

## Answer

$$\boxed{1059760019628}$$
