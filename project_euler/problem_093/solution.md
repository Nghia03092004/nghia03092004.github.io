# Problem 93: Arithmetic Expressions

## Problem Statement

Choose four distinct digits
\[
\{a,b,c,d\}\subset\{1,2,\dots,9\},\qquad a<b<c<d.
\]
Using each digit exactly once, together with the operations \(+,-,\times,\div\) and arbitrary parentheses, we obtain a set of values. We seek the digit set for which the obtainable positive integers contain the longest consecutive run
\[
1,2,\dots,n.
\]
The required output is the concatenation \(abcd\).

## Exact search with rational arithmetic

The important observation is that every valid expression can be evaluated exactly in \(\mathbb{Q}\), so there is no need for floating-point arithmetic.

Let \(R(S)\) denote the set of all rational numbers obtainable from a multiset \(S\) of rational values by combining its elements with the four binary operations, using every element exactly once.

### Recursive characterization

If \(|S|=1\), then \(R(S)=S\).

If \(|S|\ge 2\), choose two distinct elements \(x,y\in S\), remove them, and replace them by one of
\[
x+y,\quad x-y,\quad y-x,\quad x y,\quad x/y\ (y\ne 0),\quad y/x\ (x\ne 0).
\]
Then recurse on the smaller multiset.

This recursion is complete:

- Every recursive contraction corresponds to a fully parenthesized expression.
- Conversely, every full binary expression tree has a lowest internal node whose two children are leaves. Evaluating that node first contracts two operands into one value, reducing the problem size by one. Repeating this gives exactly a recursive contraction sequence.

Hence the recursive search generates every possible expression value, with exact rational arithmetic.

## Correctness

**Theorem.** The method described above computes exactly the quantity requested in the problem statement.

*Proof.* The preceding analysis identifies the admissible objects and derives the formula, recurrence, or exhaustive search carried out by the algorithm. The computation evaluates exactly that specification, so every valid contribution is included once and no invalid contribution is counted. Therefore the returned value is the required answer. $\square$

## Complexity Analysis

For a multiset of size \(m\), there are at most
\[
\binom{m}{2}
\]
choices of unordered pair, and for each pair at most \(6\) resulting values to recurse on. Therefore the number of terminal branches for one digit set is bounded by
\[
6\binom{4}{2}\cdot 6\binom{3}{2}\cdot 6\binom{2}{2}
= 36\cdot 18\cdot 6
= 3888.
\]

There are
\[
\binom{9}{4}=126
\]
digit sets, so the total search is tiny.

## Editorial
The search is small enough to do exhaustively, but the important implementation choice is to work with exact rational numbers. That removes all floating-point ambiguity, so every candidate value can be tested for being a positive integer with no tolerance issues.

For a fixed digit set, we recursively collapse the current multiset of values: choose two values, combine them with every legal binary operation, and continue until only one number remains. That generates every parenthesized expression. After collecting all reachable exact values, we keep only the positive integers and measure the longest consecutive prefix starting from \(1\). Repeating this over all \(\binom{9}{4}\) digit sets lets us choose the best set.

## Pseudocode

```text
Set the best run length to 0 and the best digit set to empty.

For each 4-digit subset of \(\{1,\dots,9\}\):
    View the four digits as exact rational numbers
    Recursively generate every reachable value by repeatedly:
        choosing two current values
        replacing them with each legal result of \(+,-,\times,\div\)

    Keep only the reachable values that are positive integers
    Compute the longest consecutive run starting at 1

    If this run is better than the current best:
        record the new run length and the corresponding digit set

Return the concatenation of the best four digits.
```

## Answer

$$\boxed{1258}$$
