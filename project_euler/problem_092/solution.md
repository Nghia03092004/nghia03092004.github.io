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
The crucial reduction is that numbers below \(10^7\) collapse after one move into the tiny state space \(\{0,\dots,567\}\). So instead of following ten million chains separately, we only need to know which of those 568 sums eventually fall into the \(89\)-cycle.

Once those destinations are known, the counting part becomes a digit DP. We treat every number below \(10^7\) as a 7-digit string with leading zeros and count how many such strings produce each possible digit-square-sum. That explores the search space by digit choices rather than by raw integers. The final answer is obtained by summing the DP counts for exactly those sums whose chains terminate at \(89\).

## Pseudocode

```text
Compute, for every sum \(s\) from 1 to 567, whether repeated digit-square steps end at \(89\).

Initialize a DP table for digit strings with \(dp[0]=1\).

Repeat for 7 digit positions:
    Create a fresh table of counts
    For each current sum and its number of realizations:
        For each possible next digit from 0 to 9:
            Update the count for the new square-digit-sum
    Replace the old table with the new one

Add the final counts for all sums whose destination is \(89\).
Return that total.
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
