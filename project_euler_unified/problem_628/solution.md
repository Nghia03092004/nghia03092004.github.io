# Problem 628: Open Chess Positions

## Problem Statement

Place `n` pawns on an `n x n` board so that each row and each column contains exactly one pawn. Let `f(n)` be the number of such positions for which a rook can start in the lower-left square, move only right or up, and reach the upper-right square without ever stepping on a pawn.

The problem gives `f(3) = 2` and `f(5) = 70`, and asks for

$$
f(10^8) \bmod 1008691207.
$$

## Mathematical Foundation

### 1. Permutation Model

Because there is exactly one pawn in each row and each column, every position is a permutation matrix.  
Write the permutation as

$$
\pi(1), \pi(2), \dots, \pi(n),
$$

where column `c` contains its pawn in row `\pi(c)`, with rows counted from bottom to top.

Thus there are `n!` total positions.

### 2. When Is a Position Closed?

For each anti-diagonal

$$
D_s = \{(c,r) : c + r = s\},
\qquad 2 \le s \le 2n,
$$

every monotone rook path visits exactly one square of `D_s`, since each move increases `c+r` by exactly `1`.

**Lemma 1.** If an anti-diagonal is completely filled with pawns, then the position is closed.

*Proof.* Any legal path must hit one square on that anti-diagonal, but all such squares are occupied. $\square$

The converse uses planarity.

**Lemma 2.** If the position is closed, then the pawns contain a complete anti-diagonal.

*Proof.* Consider the graph of empty squares, with edges between squares that differ by one legal rook move. If the lower-left square cannot reach the upper-right square, then these two corners are separated in this planar grid. By planar duality, there is a connected barrier of occupied squares running from the northwest boundary to the southeast boundary.

Because there is exactly one pawn in each row and each column, two pawns in such a barrier cannot share a row or a column. Therefore consecutive pawns in the barrier must differ by `(column + 1, row - 1)`, so the quantity `c+r` stays constant along the whole barrier. Hence the barrier is a complete anti-diagonal. $\square$

So a position is closed if and only if some anti-diagonal is fully occupied.

### 3. Full Anti-Diagonals Become Decreasing Prefixes or Suffixes

For `1 <= k <= n`, let:

- `A_k` be the set of permutations whose first `k` columns are

$$
k, k-1, \dots, 1.
$$

This is exactly the event that the anti-diagonal

$$
(1,k), (2,k-1), \dots, (k,1)
$$

is completely occupied.

- `B_k` be the set of permutations whose last `k` columns are

$$
n, n-1, \dots, n-k+1.
$$

This is exactly the event that the anti-diagonal

$$
(n-k+1,n), (n-k+2,n-1), \dots, (n,n-k+1)
$$

is completely occupied.

Hence the closed positions are precisely

$$
\bigcup_{k=1}^{n} A_k \;\cup\; \bigcup_{k=1}^{n} B_k.
$$

### 4. Counting Closed Positions

Each `A_k` fixes `k` columns, so

$$
|A_k| = (n-k)!.
$$

Likewise,

$$
|B_k| = (n-k)!.
$$

The sets within each family are disjoint. Intersections between the two families behave as follows:

- If `k + l <= n`, then `A_k \cap B_l` leaves `n-k-l` free columns, so

$$
|A_k \cap B_l| = (n-k-l)!.
$$

- There is one extra overlap when `k = l = n`: both conditions describe the single reverse permutation

$$
n, n-1, \dots, 1.
$$

Therefore the number `C_n` of closed positions is

$$
C_n
= 2\sum_{k=1}^{n}(n-k)!
 - \left(
1 + \sum_{\substack{k,l \ge 1 \\ k+l \le n}} (n-k-l)!
\right).
$$

Define the left factorial

$$
L_n = !n = \sum_{i=0}^{n-1} i!.
$$

Then

$$
\sum_{k=1}^{n}(n-k)! = L_n.
$$

For the double sum, write `t = n-k-l`. Then `0 <= t <= n-2`, and for fixed `t` there are `n-1-t` pairs `(k,l)`. So

$$
\sum_{\substack{k,l \ge 1 \\ k+l \le n}} (n-k-l)!
= \sum_{t=0}^{n-2}(n-1-t)t!.
$$

Now use

$$
\sum_{t=1}^{n-2} t\,t!
= \sum_{t=1}^{n-2}\bigl((t+1)! - t!\bigr)
= (n-1)! - 1.
$$

Hence

$$
\sum_{t=0}^{n-2}(n-1-t)t!
= (n-1)\sum_{t=0}^{n-2} t! - \bigl((n-1)! - 1\bigr)
= (n-1)L_n - n! + 1.
$$

Substituting back,

$$
C_n = n! - (n-3)L_n - 2.
$$

Therefore

$$
f(n) = n! - C_n = (n-3)L_n + 2.
$$

**Theorem.** For every `n >= 1`,

$$
f(n) = (n-3)\sum_{i=0}^{n-1} i! + 2.
$$

This matches the checks:

$$
f(3) = 2, \qquad f(5) = 70.
$$

## Algorithm

Compute the left factorial modulo

$$
M = 1008691207
$$

by iteratively maintaining `i! mod M`, then apply

$$
f(n) \equiv (n-3)L_n + 2 \pmod M.
$$

```text
fact = 1
left_factorial = 1          // 0!
for i = 1 to n-1:
    fact = fact * i mod M
    left_factorial = (left_factorial + fact) mod M

answer = ((n - 3) * left_factorial + 2) mod M
```

## Correctness

**Theorem.** The algorithm returns the correct value of `f(n) mod M`.

*Proof.* By the counting argument above,

$$
f(n) = (n-3)L_n + 2,
\qquad
L_n = \sum_{i=0}^{n-1} i!.
$$

The algorithm computes this left-factorial sum modulo `M` exactly by iterating through the factorial recurrence `i! = i \cdot (i-1)!`, then substitutes it into the closed formula for `f(n)`. Since modular addition and multiplication preserve equality modulo `M`, the final value is exactly `f(n) mod M`. $\square$

## Complexity Analysis

- **Time:** `O(n)` modular multiplications and additions.
- **Space:** `O(1)`.

## Answer

$$
\boxed{210286684}
$$
