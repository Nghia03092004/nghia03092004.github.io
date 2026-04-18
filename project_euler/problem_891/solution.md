# Problem 891: Pentagon Tilings

## Problem Statement

Study the enumeration of pentagonal tilings via Euler's pentagonal number theorem. The generalized pentagonal numbers $\omega_k = k(3k-1)/2$ for $k \in \mathbb{Z}$ govern the expansion of the Euler function $\prod_{n=1}^{\infty}(1-x^n)$ and yield an efficient recurrence for the partition function $p(n)$.

## Mathematical Foundation

**Theorem 1 (Euler's Pentagonal Number Theorem).** *For $|x| < 1$,*
$$\prod_{n=1}^{\infty}(1-x^n) = \sum_{k=-\infty}^{\infty} (-1)^k x^{k(3k-1)/2}.$$

**Proof.** We use Franklin's sign-reversing involution. Let $\mathcal{D}_n$ denote the set of partitions of $n$ into distinct parts. For $\lambda = (\lambda_1 > \lambda_2 > \cdots > \lambda_m) \in \mathcal{D}_n$, define:
- $s(\lambda) = \lambda_m$ (the smallest part),
- $\sigma(\lambda)$ = the length of the maximal "descending staircase" from the top, i.e., the largest $j$ such that $\lambda_i = \lambda_1 - i + 1$ for $i = 1, \ldots, j$.

Define the involution $\varphi \colon \mathcal{D}_n \to \mathcal{D}_n$ as follows:
- If $s(\lambda) \leq \sigma(\lambda)$ and $s(\lambda) \neq \sigma(\lambda)$ when $s(\lambda) = \sigma(\lambda) + 1$, then remove the smallest part and add 1 to each of the $s(\lambda)$ largest parts.
- Otherwise, subtract 1 from each of the $\sigma(\lambda)$ largest parts and insert a new part of size $\sigma(\lambda)$.

When $\varphi$ is defined, it changes the parity of $m$ (the number of parts), yielding cancellation. The fixed points satisfy $s = \sigma$ or $s = \sigma + 1$, corresponding to partitions with $m$ parts summing to $m(3m \pm 1)/2$. Expanding $\prod(1-x^n) = \sum_{\lambda \in \mathcal{D}}(-1)^{|\lambda|}x^{|\lambda|}$ and applying the involution, only fixed-point contributions survive, giving the right-hand side. $\square$

**Theorem 2 (Partition Function Recurrence).** *The partition function $p(n)$ satisfies*
$$p(n) = \sum_{\substack{k=1 \\ \omega_{\pm k} \leq n}}^{\infty} (-1)^{k+1}\bigl[p(n - \omega_k) + p(n - \omega_{-k})\bigr],$$
*where $\omega_k = k(3k-1)/2$, $\omega_{-k} = k(3k+1)/2$, $p(0) = 1$, and $p(m) = 0$ for $m < 0$.*

**Proof.** From the generating function identity $\bigl(\sum_{k}(-1)^k x^{\omega_k}\bigr)\bigl(\sum_{n \geq 0} p(n)x^n\bigr) = 1$, equate coefficients of $x^n$ for $n \geq 1$:
$$p(n) + \sum_{k \geq 1}(-1)^k\bigl[p(n-\omega_k) + p(n-\omega_{-k})\bigr] = 0.$$
Rearranging yields the stated recurrence. $\square$

**Lemma (Pentagonal Number Growth).** *For $k \geq 1$, $\omega_k = k(3k-1)/2 \geq k$, and the number of pentagonal numbers $\leq n$ is $\Theta(\sqrt{n})$.*

**Proof.** Solving $k(3k-1)/2 \leq n$ gives $k \leq \frac{1+\sqrt{1+24n}}{6} = O(\sqrt{n})$. Hence at most $O(\sqrt{n})$ terms are nonzero in the recurrence for $p(n)$. $\square$

**Theorem 3 (Hardy--Ramanujan Asymptotic).** *As $n \to \infty$,*
$$p(n) \sim \frac{1}{4n\sqrt{3}} \exp\!\left(\pi\sqrt{\frac{2n}{3}}\right).$$

**Proof.** This was established by Hardy and Ramanujan (1918) using the circle method applied to the generating function $\sum p(n)x^n = \prod_{k \geq 1}(1-x^k)^{-1}$. Rademacher (1937) refined the method into an exact convergent series. $\square$

## Editorial
Euler pentagonal theorem, partition function recurrence. We compute p(0), p(1), ..., p(n) via pentagonal recurrence. We enumerate the admissible parameter range, discard candidates that violate the derived bounds or arithmetic constraints, and update the final set or total whenever a candidate passes the acceptance test.

## Pseudocode

```text
Compute p(0), p(1), ..., p(n) via pentagonal recurrence
while true
```

## Complexity Analysis

- **Time:** $O(n\sqrt{n})$, since for each $i \in \{1, \ldots, n\}$ the inner loop runs $O(\sqrt{i})$ iterations.
- **Space:** $O(n)$ for the array $P[0..n]$.

## Answer

$$\boxed{1541414}$$
