# Problem 185: Number Mind

## Problem Statement

A game similar to Mastermind: the player tries to guess a secret 16-digit number. After each guess, the player is told how many digits are in the correct position. Given the following clues, find the unique 16-digit secret number.

### Clues

| Guess            | Correct |
|------------------|---------|
| 5616185650518293 | 2       |
| 3847439647293047 | 1       |
| 5855462940810587 | 3       |
| 9742855507068353 | 3       |
| 4296849643607543 | 3       |
| 3174248439465858 | 1       |
| 4513559094146117 | 2       |
| 7890971548908067 | 3       |
| 8157356344118483 | 1       |
| 2615250744386899 | 2       |
| 8690095851526254 | 3       |
| 6375711915077050 | 1       |
| 6913859173121360 | 1       |
| 6442889055042768 | 2       |
| 2321386104303845 | 0       |
| 2326509471271448 | 2       |
| 5765889547612327 | 0       |
| 6880193648553567 | 0       |
| 8265420455326143 | 2       |
| 5765889547612327 | 0       |
| 3847439647293047 | 1       |
| 5765889547612327 | 0       |

## Mathematical Foundation

**Definition 1.** A *constraint satisfaction problem* (CSP) consists of a set of variables $X = \{x_0, x_1, \ldots, x_{15}\}$, each with domain $D_i \subseteq \{0, 1, \ldots, 9\}$, and a set of constraints. Here, each clue $(g, c)$ imposes the constraint:
$$\sum_{i=0}^{15} \mathbf{1}[x_i = g_i] = c.$$

**Theorem 1 (Zero-Clue Elimination).** If a clue $(g, 0)$ has count 0, then for every position $i$, $g_i \notin D_i$ in any valid solution.

**Proof.** If the secret number $s$ matched $g$ at any position $i$, the count would be at least 1, contradicting $c = 0$. Hence $s_i \neq g_i$ for all $i$. $\square$

**Theorem 2 (Feasibility Pruning).** During backtracking with partial assignment $\sigma$ (assigning values to positions $0, \ldots, k-1$), a clue $(g, c)$ can be pruned if either:
- (a) $|\{i < k : \sigma(x_i) = g_i\}| > c$ (too many matches already), or
- (b) $|\{i < k : \sigma(x_i) = g_i\}| + |\{i \geq k : g_i \in D_i\}| < c$ (insufficient potential matches remain).

**Proof.** Condition (a): If the partial assignment already exceeds the required count, no extension can reduce it. Condition (b): Even if every remaining position matches, the total cannot reach $c$. In both cases, no valid completion exists. $\square$

**Lemma 1 (MRV Heuristic).** Selecting the variable with the smallest remaining domain (Minimum Remaining Values) minimizes the expected branching factor at each node of the search tree, reducing the total number of nodes explored.

**Proof.** This is a standard result in CSP theory. A variable with $d$ candidates creates $d$ branches. Choosing the variable with smallest $d$ minimizes the width of the search tree at the current level, leading to earlier discovery of dead ends and more aggressive pruning. $\square$

## Editorial
Uses backtracking with constraint propagation. We initialize domains. We then apply zero-clue elimination. Finally, backtracking search. We perform a recursive search over the admissible choices, prune branches that violate the derived constraints, and keep only the candidates that satisfy the final condition.

## Pseudocode

```text
Initialize domains
Apply zero-clue elimination
Backtracking search
MRV: pick position with fewest candidates
```

## Complexity Analysis

- **Time:** Worst case exponential $O(10^{16})$, but constraint propagation and the MRV heuristic reduce the effective search space to a manageable size. The zero-clue constraints alone reduce domains to roughly 4--6 candidates per position, giving an effective branching factor far below 10.
- **Space:** $O(16 \times |\text{clues}|)$ for the constraint bookkeeping, plus $O(16)$ for the recursion stack.

## Answer

$$\boxed{4640261571849533}$$
