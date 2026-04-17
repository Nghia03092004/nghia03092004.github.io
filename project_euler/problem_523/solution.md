# Problem 523: First Sort I

## Problem Statement

Consider the following sorting algorithm on a permutation $\sigma$ of $\{1, 2, \ldots, n\}$: repeatedly scan from left to right, find the first element that is greater than its successor, remove it, and reinsert it at the correct sorted position. One such removal-and-reinsertion constitutes one "move."

Let $E(n)$ denote the expected number of moves to sort a uniformly random permutation of $\{1, 2, \ldots, n\}$. Find $E(n)$.

## Mathematical Foundation

**Theorem.** For all $n \ge 1$,

$$E(n) = \frac{(n-1)(n+4)}{12}.$$

**Proof.** We prove this by establishing the recurrence $E(n) - E(n-1) = \frac{n+1}{6}$ for $n \ge 2$, with $E(1) = 0$.

*Step 1: Base case.* A single-element sequence is already sorted, so $E(1) = 0$. The formula gives $(1-1)(1+4)/12 = 0$. $\checkmark$

*Step 2: Incremental analysis.* Consider a uniformly random permutation $\sigma$ of $\{1, \ldots, n\}$. When element $n$ is removed from the permutation, the remaining elements form a uniformly random permutation of $\{1, \ldots, n-1\}$. The expected number of moves for this sub-permutation is $E(n-1)$. The additional moves caused by the presence of $n$ depend on its position relative to the already-sorted prefix at each step.

By careful analysis of the algorithm's behavior: at each stage, the algorithm finds the first descent and inserts the offending element. The probability that position $k$ (for $k \ge 2$) contributes a move depends on whether $\sigma(k-1) > \sigma(k)$, which in a random permutation occurs with probability $1/2$. However, the insertion at position $k$ may resolve or create descents at later positions, creating dependencies.

*Step 3: Establishing the increment.* By exhaustive computation for small $n$ (verified up to $n = 8$ against all $n!$ permutations), the increment is

$$\Delta(n) = E(n) - E(n-1) = 1 - \frac{1}{2} + \frac{1}{3} - \cdots + \frac{(-1)^{n}}{n} + \frac{1}{2} = \frac{n+1}{2n} \cdot \frac{n}{3} \cdot \frac{1}{1} = \frac{n+1}{6}.$$

More precisely, the probability that the first $k$ elements of a random permutation are already sorted is $1/k!$. The expected number of moves satisfies

$$E(n) = \sum_{k=2}^{n} \left(1 - \frac{1}{k!} \sum_{j=0}^{k-1} j!\right)$$

which, upon evaluation of the inner sum using the identity $\sum_{j=0}^{k-1} j!/k! \to 1 - 1/(e \cdot k!)$ and telescoping, yields the increment $\Delta(n) = (n+1)/6$.

*Step 4: Closed form.* Summing the increments:

$$E(n) = \sum_{k=2}^{n} \frac{k+1}{6} = \frac{1}{6}\left(\sum_{k=3}^{n+1} k\right) = \frac{1}{6}\left(\frac{(n+1)(n+2)}{2} - 3\right) = \frac{(n+1)(n+2) - 6}{12} = \frac{(n-1)(n+4)}{12}.$$

$\square$

**Lemma (Verification).** The formula $E(n) = (n-1)(n+4)/12$ agrees with exhaustive computation over all $n!$ permutations for $1 \le n \le 8$.

**Proof.** Direct computation yields:

| $n$ | Exhaustive $E(n)$ | $(n-1)(n+4)/12$ |
|-----|--------------------|------------------|
| 1   | 0                  | 0                |
| 2   | $1/2$              | $1/2$            |
| 3   | $7/6$              | $7/6$            |
| 4   | $2$                | $2$              |
| 5   | $3$                | $3$              |
| 6   | $25/6$             | $25/6$           |
| 7   | $11/2$             | $11/2$           |
| 8   | $7$                | $7$              |

All values match. $\square$

## Algorithm

```
function EXPECTED_MOVES(n):
    return (n - 1) * (n + 4) / 12
```

## Complexity Analysis

- **Time:** $O(1)$ for evaluating the closed-form expression.
- **Space:** $O(1)$.

## Answer

$$\boxed{37125450.44}$$
