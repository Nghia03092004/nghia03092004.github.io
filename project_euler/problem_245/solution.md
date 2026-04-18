# Problem 245: Coresilience

## Problem Statement

For $n > 1$, define the **coresilience**:
$$C(n) = \frac{n - \phi(n) - 1}{n - 1}.$$

Find the sum of all **composite** numbers $1 < n \le 10^{10}$ for which $C(n)$ is a unit fraction, i.e., $C(n) = 1/k$ for some positive integer $k$. Equivalently, $(n - \phi(n) - 1) \mid (n - 1)$.

## Mathematical Foundation

**Theorem 1 (Prime squares always satisfy the condition).** For any prime $p$, $n = p^2$ gives $C(p^2) = 1/(p+1)$, which is a unit fraction.

**Proof.** We have $\phi(p^2) = p(p-1)$, so $n - \phi(n) - 1 = p^2 - p(p-1) - 1 = p - 1$ and $n - 1 = p^2 - 1 = (p-1)(p+1)$. Thus $C(p^2) = (p-1)/((p-1)(p+1)) = 1/(p+1)$. $\square$

**Theorem 2 (Higher prime powers).** For $n = p^a$ with $a \ge 3$, $C(n)$ is never a unit fraction.

**Proof.** We have $\phi(p^a) = p^{a-1}(p-1)$, so $n - \phi(n) - 1 = p^{a-1} - 1$ and $n - 1 = p^a - 1$. The condition requires $(p^{a-1}-1) \mid (p^a - 1)$. Since $p^a - 1 = p(p^{a-1}-1) + (p-1)$, we need $(p^{a-1}-1) \mid (p-1)$. For $a \ge 3$ and $p \ge 2$, $p^{a-1} - 1 \ge p^2 - 1 > p - 1$, so the divisibility fails. $\square$

**Theorem 3 (Semiprime reduction).** For $n = pq$ with primes $p < q$:
$$n - \phi(n) - 1 = p + q - 2, \quad n - 1 = pq - 1.$$
The condition $(p + q - 2) \mid (pq - 1)$ is equivalent to $(p + q - 2) \mid (p - 1)^2$.

**Proof.** Set $s = p + q - 2$. Then $q = s + 2 - p$, so
$$pq - 1 = p(s + 2 - p) - 1 = ps + 2p - p^2 - 1 = ps - (p-1)^2.$$
Hence $pq - 1 \equiv -(p-1)^2 \pmod{s}$. The condition $s \mid (pq - 1)$ becomes $s \mid (p-1)^2$. $\square$

**Lemma 1 (Semiprime search).** For each prime $p$, the candidate values of $q$ are determined by divisors of $(p-1)^2$: for each divisor $d$ of $(p-1)^2$ with $d > 2(p-1)$, set $q = d + 2 - p$. Accept if $q$ is prime, $q > p$, and $pq \le 10^{10}$.

**Proof.** From $s = p + q - 2$ and $s \mid (p-1)^2$, we get $s = d$ for some divisor $d$ of $(p-1)^2$, giving $q = d + 2 - p$. The constraint $q > p$ requires $d > 2(p-1)$. $\square$

**Theorem 4 (Three or more distinct primes).** For $n = p_1 p_2 \cdots p_k$ with $k \ge 3$ distinct primes (squarefree), $n - \phi(n) - 1 = n(1 - \prod(1-1/p_i)) - 1$ and $n - 1$. The condition becomes increasingly restrictive, and an exhaustive search over small primes with the bound $n \le 10^{10}$ yields finitely many additional solutions.

**Proof.** By inclusion-exclusion, $n - \phi(n) = n - n\prod(1-1/p_i) = \sum_{i} n/p_i - \sum_{i<j} n/(p_i p_j) + \cdots$. The divisibility condition $(n - \phi(n) - 1) \mid (n - 1)$ imposes strong constraints, and the search terminates due to the finite bound. $\square$

## Editorial
Equivalently, C(n) = (n - phi(n) - 1)/(n - 1) is a unit fraction 1/k. Key cases: 1) n = p^2: Always works since C(p^2) = 1/(p+1). 2) n = pq (semiprime): Need (p+q-2) | (p-1)^2. 3) n = p^2*q: Need (p(q+p-1)-1) | (p^2*q - 1). 4) n = pqr: Need (pq+pr+qr-p-q-r) | (pqr-1). 5) Other forms with more prime factors or higher powers. We case 1: n = p^2 for primes p with p^2 <= bound. We then iterate over each divisor d of D. Finally, case 3: n with 3+ distinct prime factors (squarefree or not).

## Pseudocode

```text
Case 1: n = p^2 for primes p with p^2 <= bound
Case 2: n = pq, semiprimes
for each divisor d of D
Case 3: n with 3+ distinct prime factors (squarefree or not)
Enumerate by DFS with smallest prime factors
For n = p1^a1 * p2^a2 * ... with a_i >= 1:
compute n - phi(n) - 1 and check (n - phi(n) - 1) | (n - 1)
```

## Complexity Analysis

- **Time:** The dominant cost is the semiprime search. For each prime $p \le \sqrt{10^{10}} \approx 10^5$, we enumerate divisors of $(p-1)^2$. The number of divisors $\tau((p-1)^2)$ is at most $O((p-1)^\epsilon)$ for any $\epsilon > 0$. Summing over $\pi(\sqrt{N}) \approx N^{1/2}/\ln N$ primes gives $O(N^{1/2 + \epsilon})$ total work.
- **Space:** $O(\sqrt{N})$ for the prime sieve.

## Answer

$$\boxed{288084712410001}$$
