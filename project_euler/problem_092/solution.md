# Problem 92: Square Digit Chains

## Problem Statement

Define
\[
f(n)=\text{sum of the squares of the decimal digits of }n.
\]
Starting from a positive integer \(n\), repeatedly apply \(f\). Every chain eventually enters a cycle. We must count how many starting numbers \(n<10^7\) eventually arrive at \(89\).

## Step 1: Reduce the state space

If \(n<10^7\), then \(n\) has at most \(7\) digits, so
\[
f(n)\le 7\cdot 9^2 = 567.
\]
Thus after one application of \(f\), every chain enters the finite set
\[
\{0,1,2,\dots,567\}.
\]

Therefore the eventual destination of \(n\) depends only on the value \(f(n)\). In particular, if we know for each \(s\in\{1,\dots,567\}\) whether repeated application of \(f\) reaches \(1\) or \(89\), then we only need to count how many numbers below \(10^7\) have initial digit-square-sum equal to such an \(s\).

## Step 2: Count numbers by their digit-square-sum

Every number \(0\le n<10^7\) has a unique 7-digit representation with leading zeros allowed. So it is equivalent to count 7-digit strings
\[
d_1d_2\cdots d_7,\qquad d_i\in\{0,1,\dots,9\},
\]
by the value
\[
d_1^2+d_2^2+\cdots+d_7^2.
\]

Let \(C_k(s)\) be the number of length-\(k\) digit strings whose digit-square-sum is \(s\). Then

- \(C_0(0)=1\),
- \(C_0(s)=0\) for \(s\ne 0\),
- and for \(k\ge 0\),
\[
C_{k+1}(s)=\sum_{d=0}^{9} C_k(s-d^2),
\]
where terms with negative index are interpreted as \(0\).

This recurrence is immediate: to build a \((k+1)\)-digit string with total square-sum \(s\), choose its last digit \(d\), and the first \(k\) digits must contribute \(s-d^2\).

After computing \(C_7(s)\) for all \(0\le s\le 567\), the answer is
\[
\sum_{\substack{0\le s\le 567\\ s\leadsto 89}} C_7(s),
\]
where \(s\leadsto 89\) means the chain starting from \(s\) eventually reaches \(89\).

The all-zero string contributes only to \(s=0\), and \(0\) does not reach \(89\), so it does not affect the final count.

## Why only 1 and 89 matter

Since every chain enters the finite set \(\{1,\dots,567\}\), every orbit is eventually periodic. A direct computation on this finite set shows that the only periodic attractors are
\[
1\to 1
\]
and
\[
89\to 145\to 42\to 20\to 4\to 16\to 37\to 58\to 89.
\]
Hence each starting value below \(10^7\) ends at either \(1\) or \(89\).

## Editorial
Count starting numbers below 10,000,000 that arrive at 89 by grouping numbers according to their first square-digit-sum. We iterate over every \(s\in\{1,\dots,567\}\), compute whether its chain ends at \(89\). We then run the DP recurrence for \(k=0,1,\dots,7\) to obtain the counts \(C_7(s)\). Finally, sum \(C_7(s)\) over all \(s\) whose chain ends at \(89\).

## Pseudocode

```text
For every \(s\in\{1,\dots,567\}\), compute whether its chain ends at \(89\)
Run the DP recurrence for \(k=0,1,\dots,7\) to obtain the counts \(C_7(s)\)
Sum \(C_7(s)\) over all \(s\) whose chain ends at \(89\)
```

## Correctness

**Theorem.** The method described above computes exactly the quantity requested in the problem statement.

*Proof.* The preceding analysis identifies the admissible objects and derives the formula, recurrence, or exhaustive search carried out by the algorithm. The computation evaluates exactly that specification, so every valid contribution is included once and no invalid contribution is counted. Therefore the returned value is the required answer. $\square$

## Complexity Analysis

The state space for the DP has

- \(7\) digit positions,
- \(568\) possible sums,
- \(10\) digit choices.

So the running time is
\[
O(7\cdot 568\cdot 10),
\]
and the memory usage is
\[
O(568).
\]

In other words, this is a tiny constant-size computation.

## Answer

$$\boxed{8581146}$$
