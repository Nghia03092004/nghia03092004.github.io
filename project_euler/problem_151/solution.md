# Problem 151: Paper Sheets of Standard Sizes

## Problem Statement

A printing shop runs 16 batches per job. For each batch, they take one sheet of paper at random from an envelope. At the beginning, the envelope contains a single A1 sheet. Whenever an A$n$ sheet (where $n < 5$) is picked, it is cut successively to produce one each of A$(n+1)$, A$(n+2)$, ..., A5; one A5 is used for the batch and the remaining sheets are returned to the envelope. When an A5 sheet is picked, it is simply used.

Find the expected number of times (excluding the very first and very last batches) that the worker finds only a single sheet of paper in the envelope.

## Mathematical Development

**Definition 1.** A *state* is a tuple $\mathbf{s} = (a_2, a_3, a_4, a_5) \in \mathbb{Z}_{\ge 0}^4$, where $a_k$ denotes the number of A$k$ sheets in the envelope for $k \in \{2,3,4,5\}$. The A1 sheet is omitted because it is consumed in the first (excluded) batch.

**Lemma 1 (State Transition Rules).** *When an A$k$ sheet ($k \in \{2,3,4,5\}$) is selected from the envelope, the state transition is:*
- *$k = 5$: $a_5 \mapsto a_5 - 1$; all other components unchanged.*
- *$k = 4$: $a_4 \mapsto a_4 - 1,\; a_5 \mapsto a_5 + 1$.*
- *$k = 3$: $a_3 \mapsto a_3 - 1,\; a_4 \mapsto a_4 + 1,\; a_5 \mapsto a_5 + 1$.*
- *$k = 2$: $a_2 \mapsto a_2 - 1,\; a_3 \mapsto a_3 + 1,\; a_4 \mapsto a_4 + 1,\; a_5 \mapsto a_5 + 1$.*

**Proof.** An A$k$ sheet with $k < 5$ is cut by successive halving into sheets A$(k{+}1)$, A$(k{+}2)$, ..., A$5$, A$5$. The final halving of an A$4$ produces two A$5$ sheets, so the cut of A$k$ yields one sheet of each size from A$(k{+}1)$ through A$4$, plus two A$5$ sheets. One A$5$ is consumed for the batch. The net effect is: remove one A$k$, add one each of A$(k{+}1)$, ..., A$5$. For $k = 5$, the sheet is consumed outright. $\square$

**Lemma 2 (Initial State).** *The state after the first batch (which deterministically consumes the lone A1 sheet) is $(1, 1, 1, 1)$.*

**Proof.** Cutting A1 yields one each of A2, A3, A4, and two copies of A5. One A5 is consumed, leaving counts $(1, 1, 1, 1)$. $\square$

**Lemma 3 (Finiteness of the State Space).** *The reachable state space satisfies $a_2 \le 1$, $a_3 \le 2$, $a_4 \le 4$, $a_5 \le 8$, subject to the invariant $8a_2 + 4a_3 + 2a_4 + a_5 \le 15$. In particular, the number of reachable states $|\mathcal{S}|$ is at most $2 \times 3 \times 5 \times 9 = 270$.*

**Proof.** Define the *A5-equivalent count* $W(\mathbf{s}) = 8a_2 + 4a_3 + 2a_4 + a_5$, reflecting that each A$k$ sheet can be decomposed into $2^{5-k}$ sheets of A5. The initial state has $W = 8 + 4 + 2 + 1 = 15$. Each batch consumes exactly one A5-equivalent unit (one A5 is used), so $W$ decreases by 1 per batch. No transition creates additional A5-equivalent mass: when an A$k$ sheet ($k < 5$) is cut and one A5 consumed, the net change is $-2^{5-k} + (2^{5-(k+1)} + \cdots + 2^0 + 2^0) - 1 = 0 - 1 = -1$. Since no A2 is ever created (only removed), $a_2 \le 1$. Similarly, $a_3 \le 1 + a_2 \le 2$, $a_4 \le 1 + a_2 + a_3 \le 4$, and $a_5 \le 1 + a_2 + a_3 + a_4 \le 8$. $\square$

**Theorem 1 (Expectation Recursion).** *Define $E(\mathbf{s})$ as the expected number of single-sheet encounters from state $\mathbf{s}$ onward, excluding the terminal batch. Let $T(\mathbf{s}) = a_2 + a_3 + a_4 + a_5$ be the total sheet count. Then $E(\mathbf{0}) = 0$ and for $\mathbf{s} \neq \mathbf{0}$:*

$$E(\mathbf{s}) = \mathbf{1}\!\left[T(\mathbf{s}) = 1 \;\wedge\; \mathbf{s} \neq (0,0,0,1)\right] + \sum_{\substack{k=2 \\ a_k > 0}}^{5} \frac{a_k}{T(\mathbf{s})} \cdot E(\mathbf{s}_k)$$

*where $\mathbf{s}_k$ denotes the state obtained by selecting an A$k$ sheet from state $\mathbf{s}$ (as defined in Lemma 1).*

**Proof.** The process is a finite-state Markov chain on $\mathcal{S}$ with absorbing state $\mathbf{0}$. At each non-terminal state $\mathbf{s}$, the worker observes a single sheet if and only if $T(\mathbf{s}) = 1$. The problem excludes the first batch (state $(1,1,1,1)$, where $T = 4$) and the last batch. The last batch occurs at state $(0,0,0,1)$, which is the unique pre-terminal state; it transitions to $\mathbf{0}$ with certainty. We exclude this state from the single-sheet count by the indicator condition $\mathbf{s} \neq (0,0,0,1)$.

Each sheet of size $k$ is selected with probability $a_k / T(\mathbf{s})$ (uniform random draw). After selection, the state transitions to $\mathbf{s}_k$ and the process continues. By the tower property of conditional expectation:

$$E(\mathbf{s}) = \mathbf{1}[\text{single-sheet event}] + \sum_{k} \frac{a_k}{T(\mathbf{s})} E(\mathbf{s}_k).$$

The recursion terminates at $\mathbf{0}$ since no further batches occur. Since $W$ strictly decreases, there are no cycles, ensuring the recursion is well-founded. $\square$

**Corollary.** The answer to the problem is $E(1,1,1,1)$.

## Editorial
We single-sheet indicator (exclude terminal state). Finally, recurse over each possible selection. We use dynamic programming over the state space implied by the derivation, apply each admissible transition, and read the answer from the final table entry.

## Pseudocode

```text
Single-sheet indicator (exclude terminal state)
Recurse over each possible selection
```

## Complexity Analysis

- **Time:** $O(|\mathcal{S}|)$ where $|\mathcal{S}| \le 270$ is the number of reachable states. Each state is visited once (via memoization) and requires $O(1)$ work (at most 4 recursive lookups and arithmetic operations). Total: $O(270) = O(1)$.
- **Space:** $O(|\mathcal{S}|) = O(1)$ for the memoization table.

## Answer

$$\boxed{0.464399}$$
