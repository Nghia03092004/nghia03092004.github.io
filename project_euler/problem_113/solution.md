# Problem 113: Non-bouncy Numbers

## Problem Statement

A positive integer is *increasing* if its digits are non-decreasing from left to right, *decreasing* if non-increasing, and *bouncy* otherwise. How many numbers below a googol ($10^{100}$) are not bouncy?

## Mathematical Development

**Theorem 1 (Counting increasing numbers).** *The number of positive increasing integers with at most $n$ digits is*
$$I(n) = \binom{n+9}{9} - 1.$$

*Proof.* An increasing number with at most $n$ digits corresponds bijectively to a non-decreasing sequence $(d_1, d_2, \ldots, d_n) \in \{0, 1, \ldots, 9\}^n$, where leading zeros represent shorter numbers. The number of such non-decreasing sequences is the number of multisets of size $n$ drawn from an alphabet of size 10. By the stars-and-bars theorem, this equals
$$\binom{n + 10 - 1}{10 - 1} = \binom{n + 9}{9}.$$
This count includes the all-zeros sequence $(0, 0, \ldots, 0)$, which represents the integer 0, not a positive integer. Subtracting this single case yields $I(n) = \binom{n+9}{9} - 1$. $\blacksquare$

**Theorem 2 (Counting decreasing numbers).** *The number of positive decreasing integers with at most $n$ digits is*
$$D(n) = \binom{n+10}{10} - 1 - n.$$

*Proof.* A decreasing number of exactly $k$ digits is a non-increasing sequence $(d_1, \ldots, d_k) \in \{0, \ldots, 9\}^k$ with $d_1 \geq d_2 \geq \cdots \geq d_k$ and $d_1 \geq 1$. By reversal, the count of non-increasing sequences of length $k$ from $\{0, \ldots, 9\}$ equals the count of non-decreasing sequences, which is $\binom{k+9}{9}$. Exactly one of these (the all-zeros sequence) has $d_1 = 0$, so the count with $d_1 \geq 1$ is $\binom{k+9}{9} - 1$.

Summing over all lengths $k = 1, 2, \ldots, n$:
$$D(n) = \sum_{k=1}^{n} \left[\binom{k+9}{9} - 1\right] = \sum_{k=1}^{n} \binom{k+9}{9} - n.$$

By the hockey-stick (Christmas stocking) identity,
$$\sum_{k=1}^{n} \binom{k+9}{9} = \sum_{j=10}^{n+9} \binom{j}{9} = \binom{n+10}{10} - \binom{9}{9} = \binom{n+10}{10} - 1.$$

Therefore $D(n) = \binom{n+10}{10} - 1 - n$. $\blacksquare$

**Lemma 1 (Overlap: numbers both increasing and decreasing).** *The number of positive integers with at most $n$ digits that are simultaneously increasing and decreasing is $9n$.*

*Proof.* A number is both increasing and decreasing if and only if all its digits are identical, i.e., it is a repdigit. For each digit length $k \in \{1, 2, \ldots, n\}$, there are exactly 9 positive repdigits: $\underbrace{dd \cdots d}_k$ for $d \in \{1, 2, \ldots, 9\}$. The total is $9n$. $\blacksquare$

**Theorem 3 (Non-bouncy count by inclusion-exclusion).** *The number of non-bouncy positive integers below $10^n$ is*
$$\operatorname{NonBouncy}(n) = \binom{n+9}{9} + \binom{n+10}{10} - 10n - 2.$$

*Proof.* By inclusion-exclusion, the non-bouncy count equals $I(n) + D(n) - \text{Overlap}(n)$. Substituting:
$$\begin{aligned}
\operatorname{NonBouncy}(n) &= \left[\binom{n+9}{9} - 1\right] + \left[\binom{n+10}{10} - 1 - n\right] - 9n \\
&= \binom{n+9}{9} + \binom{n+10}{10} - 10n - 2.
\end{aligned}$$
$\blacksquare$

**Corollary 2 (Asymptotic non-bouncy fraction).** *The fraction of non-bouncy numbers below $10^n$ satisfies*
$$\frac{\operatorname{NonBouncy}(n)}{10^n - 1} = O\!\left(\frac{n^{10}}{10^n}\right) \to 0 \quad \text{as } n \to \infty.$$

*Proof.* The leading term of $\binom{n+10}{10}$ is $n^{10}/10!$, which is polynomial in $n$. Since polynomial growth is dominated by exponential growth, the ratio tends to 0. $\blacksquare$

## Editorial
This problem is purely a counting argument, so the algorithm is really just an evaluation of the closed forms proved above. Increasing numbers are counted by treating a number with at most 100 digits as a non-decreasing digit sequence with leading zeros allowed, while decreasing numbers are counted by the analogous non-increasing interpretation. Those two counts overlap exactly on the repdigits, so inclusion-exclusion finishes the job.

The implementation therefore does not enumerate any numbers at all. It computes the two binomial coefficients appearing in the formulas, subtracts the duplicated repdigits, and returns the resulting non-bouncy count below $10^{100}$.

## Pseudocode

```text
Set n = 100.
Count the increasing numbers with the formula involving binomial(n + 9, 9).
Count the decreasing numbers with the formula involving binomial(n + 10, 10).
Subtract the repdigits that were counted in both totals.
Return the resulting number of non-bouncy integers below 10^n.
```

## Complexity Analysis

- **Time.** $O(1)$ arithmetic operations: computing $\binom{109}{9}$ requires 9 multiplications and 9 divisions, and $\binom{110}{10}$ requires 10 of each. With big-integer arithmetic on numbers of $O(n)$ digits, each operation costs $O(n \log n)$ via FFT-based multiplication, giving $O(n \log n)$ total. For $n = 100$, this is negligible.
- **Space.** $O(1)$: a constant number of big integers, each with $O(n)$ digits.

## Answer

$$\boxed{51161058134250}$$
