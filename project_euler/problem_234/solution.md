# Problem 234: Semidivisible Numbers

## Problem Statement

For an integer $n$, define $\mathrm{lps}(n)$ as the largest prime $\leq \sqrt{n}$ and $\mathrm{ups}(n)$ as the smallest prime $\geq \sqrt{n}$. A number $n$ is **semidivisible** if exactly one of $\mathrm{lps}(n)$ or $\mathrm{ups}(n)$ divides it.

Find the sum of all semidivisible numbers not exceeding $999\,966\,663\,333$.

## Mathematical Foundation

**Theorem (Interval Decomposition).** For consecutive primes $p < q$, every integer $n$ with $p^2 < n < q^2$ satisfies $\mathrm{lps}(n) = p$ and $\mathrm{ups}(n) = q$.

**Proof.** Since $p$ and $q$ are consecutive primes and $p^2 < n < q^2$, we have $p < \sqrt{n} < q$. As $p$ is the largest prime $\leq p < \sqrt{n}$ and there is no prime between $p$ and $q$, $p$ is the largest prime $\leq \sqrt{n}$, so $\mathrm{lps}(n) = p$. Similarly, $q$ is the smallest prime $\geq \sqrt{n}$, so $\mathrm{ups}(n) = q$. $\square$

**Lemma (Semidivisibility Criterion).** For $n$ in the interval $(p^2, q^2)$ with consecutive primes $p < q$, $n$ is semidivisible if and only if exactly one of $p \mid n$ or $q \mid n$ holds.

**Proof.** By the theorem, $\mathrm{lps}(n) = p$ and $\mathrm{ups}(n) = q$. By definition, $n$ is semidivisible when exactly one of these divides $n$. $\square$

**Lemma (Arithmetic Sum Formula).** The sum of semidivisible numbers in the open interval $(p^2, \min(q^2, L))$ equals

$$\Sigma_p + \Sigma_q - 2\Sigma_{pq}$$

where $\Sigma_d$ denotes the sum of all multiples of $d$ in the interval.

**Proof.** Let $A$ be the set of multiples of $p$ and $B$ the set of multiples of $q$ in the interval. Semidivisible numbers form the symmetric difference $A \triangle B = (A \setminus B) \cup (B \setminus A)$. Using $\sum_{A \triangle B} = \sum_A + \sum_B - 2\sum_{A \cap B}$ and $A \cap B$ is the set of multiples of $\mathrm{lcm}(p,q) = pq$ (since $p, q$ are distinct primes), the result follows. $\square$

Each $\Sigma_d$ is computed as the sum of an arithmetic progression: for multiples of $d$ in $(a, b)$, the first term is $d \cdot \lceil (a+1)/d \rceil$, the last is $d \cdot \lfloor (b-1)/d \rfloor$, and the sum is $d \cdot (k_1 + k_2)(k_2 - k_1 + 1)/2$.

## Editorial
A number n is semidivisible if exactly one of lps(n), ups(n) divides n, where lps(n) = largest prime <= sqrt(n), ups(n) = smallest prime >= sqrt(n). We sum of multiples of d in open interval (lo, hi). Finally, sum of multiples of d in (lo, hi) exclusive.

## Pseudocode

```text
L = 999966663333
Sum of multiples of d in open interval (lo, hi)
Sum of multiples of d in (lo, hi) exclusive
```

## Complexity Analysis

- **Time:** $O(\sqrt{L} \log \log \sqrt{L})$ for the sieve, plus $O(\pi(\sqrt{L}))$ for iterating over consecutive prime pairs. Each pair requires $O(1)$ arithmetic. Total: $O(\sqrt{L} \log \log \sqrt{L})$.
- **Space:** $O(\sqrt{L})$ for the sieve.

## Answer

$$\boxed{1259187438574927161}$$
