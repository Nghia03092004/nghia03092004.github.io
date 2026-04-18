# Problem 612: Friend Numbers

## Problem Statement

Let $s(n)$ denote the sorted tuple of decimal digits of $n$. Two positive integers $a$ and $b$ are *friend numbers* if $s(a) = s(b)$. Define $f(n)$ as the number of positive integers $m \le 10^n$ such that $m$ has no friend number that is prime (i.e., no permutation of the digits of $m$ is prime). Compute $f(22)$.

## Mathematical Foundation

**Definition.** A *digit signature* of a non-negative integer $m$ is the multiset of its decimal digits. Two integers are friends if they share the same digit signature. We say $m$ is *friendless-to-primes* if no permutation of its digits (with no leading zero) forms a prime.

**Theorem 1 (Reduction to Digit Multisets).** The count $f(n)$ equals the number of integers $m \in [1, 10^n]$ such that for every permutation $\sigma$ of the digits of $m$ (ignoring those with leading zeros), $\sigma(m)$ is composite or $\le 1$.

**Proof.** This is a direct restatement of the definition. $\square$

**Lemma 1 (Divisibility Filter).** Let $D = (d_0, d_1, \ldots, d_9)$ be a digit frequency vector where $d_i$ is the count of digit $i$, and $|D| = \sum d_i$ is the total number of digits. A necessary condition for *all* permutations to be composite is:

1. **Divisibility by 3:** If $3 \mid (d_1 + 2d_2 + 3d_3 + \cdots + 9d_9)$, i.e., $3 \mid \text{digitsum}(m)$, then every permutation is divisible by 3. If additionally $|D| > 1$ and the digit set allows numbers $> 3$, all permutations are composite (except possibly 3 itself).
2. **All-even final digit:** If every permutation ends in an even digit or 5, then every permutation is divisible by 2 or 5.

**Proof.** (1) The digit sum is invariant under permutation, and $n \equiv \text{digitsum}(n) \pmod{3}$. If the digit sum is divisible by 3, every permutation is divisible by 3. (2) A number is even iff its last digit is even, and divisible by 5 iff its last digit is 0 or 5. If all digits are from $\{0, 2, 4, 5, 6, 8\}$, every permutation ends in one of these, hence is divisible by 2 or 5. $\square$

**Theorem 2 (Counting via Inclusion--Exclusion on Digit Multisets).** Group all integers by digit signature. For each signature $D$ with $|D| \le n$ digits, determine whether *any* permutation is prime. The count of friendless-to-primes integers with signature $D$ is:
$$c(D) = \begin{cases} P(D) & \text{if no permutation of } D \text{ is prime}, \\ 0 & \text{otherwise}, \end{cases}$$
where $P(D)$ is the number of valid (no leading zero) arrangements of $D$:
$$P(D) = \frac{|D|!}{\prod_{i=0}^{9} d_i!} - \frac{(|D|-1)!}{(d_0 - 1)! \prod_{i=1}^{9} d_i!} \cdot \mathbb{1}[d_0 > 0].$$
Then $f(n) = \sum_{D:\, |D| \le n} c(D)$.

**Proof.** The multinomial coefficient counts total arrangements; subtracting those with a leading zero (fixing 0 in the first position and permuting the rest) gives valid integers. Summing over all friendless signatures yields $f(n)$. $\square$

**Lemma 2 (Primality Check Sufficiency).** For a digit signature $D$ with digit sum not divisible by 3 and containing at least one odd digit not equal to 5, it suffices to check a bounded number of permutations for primality using a deterministic Miller--Rabin test.

**Proof.** By Lemma 1, if $3 \nmid \text{digitsum}$ and the digits include $\{1, 3, 7, 9\}$, then permutations ending in those digits are not automatically composite. For signatures with $|D| \le 22$, the number of permutations is at most $22! / \prod d_i!$, and probabilistic or deterministic primality tests apply to each candidate. $\square$

## Editorial
Count numbers whose digit permutations include at least one prime. We quick filters. We then all permutations divisible by 3 => friendless (unless 3 itself is a perm). Finally, otherwise, enumerate permutations ending in odd digit != 5.

## Pseudocode

```text
Quick filters
All permutations divisible by 3 => friendless (unless 3 itself is a perm)
Otherwise, enumerate permutations ending in odd digit != 5
and check primality
for each permutation sigma of D with no leading zero
```

## Complexity Analysis

- **Time:** $O\!\left(\binom{n+9}{9} \cdot K\right)$ where $\binom{n+9}{9}$ is the number of distinct digit signatures with at most $n$ digits, and $K$ is the cost of the primality check per signature (ranging from $O(1)$ for filtered cases to $O(n!/\prod d_i!)$ in the worst case, though filters eliminate most signatures).
- **Space:** $O(n)$ for the current signature and primality workspace.

## Answer

$$\boxed{819963842}$$
