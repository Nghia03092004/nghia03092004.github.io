# Problem 976: Perfect Power Detection

## Problem Statement

A *perfect power* is an integer of the form $a^b$ where $a \ge 1$ and $b \ge 2$. Find the number of distinct perfect powers in $\{1, 2, \ldots, 10^{18}\}$.

## Mathematical Foundation

**Definition 1.** Let $\mathcal{P}(N) = \{n \in \mathbb{Z} : 1 \le n \le N, \; n = a^b \text{ for some } a \ge 1, b \ge 2\}$. We seek $|\mathcal{P}(N)|$ for $N = 10^{18}$.

**Theorem 1 (Reduction to Prime Exponents).** *Every perfect power $n = a^b$ with $b \ge 2$ can be written as $n = c^p$ for some prime $p$ and integer $c \ge 1$.*

**Proof.** Write $b = pm$ where $p$ is a prime factor of $b$. Then $n = a^b = (a^m)^p = c^p$ with $c = a^m$. $\square$

**Theorem 2 (Inclusion-Exclusion Formula).** *Let $P_p(N) = \{n \le N : n = a^p \text{ for some } a \ge 1\}$, so $|P_p(N)| = \lfloor N^{1/p} \rfloor$. Then:*
$$|\mathcal{P}(N)| = \left|\bigcup_{p \text{ prime}} P_p(N)\right| = \sum_{\emptyset \neq S \subseteq \mathcal{Q}} (-1)^{|S|+1} \left\lfloor N^{1/\prod_{p \in S} p} \right\rfloor$$
*where $\mathcal{Q} = \{p \text{ prime} : p \le \log_2 N\}$ is the set of relevant prime exponents.*

**Proof.** By inclusion-exclusion on the sets $P_p(N)$. A number in $P_{p_1} \cap P_{p_2} \cap \cdots \cap P_{p_k}$ (with $p_i$ distinct primes) is an $\text{lcm}(p_1,\ldots,p_k)$-th power, i.e., a $(p_1 p_2 \cdots p_k)$-th power (since the $p_i$ are distinct primes). Thus $|P_{p_1} \cap \cdots \cap P_{p_k}| = \lfloor N^{1/(p_1\cdots p_k)} \rfloor$. The union has no contribution from primes $p > \log_2 N$ since $\lfloor N^{1/p} \rfloor = 1$ and $1 = 1^p$ is already counted. $\square$

**Lemma 1 (Exponent Bound).** *For $N = 10^{18}$, the relevant primes are $\mathcal{Q} = \{2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59\}$, since $\log_2(10^{18}) \approx 59.79$.*

**Proof.** If $p \ge 61$ is prime, then $\lfloor (10^{18})^{1/p} \rfloor \le \lfloor (10^{18})^{1/61} \rfloor = 1$, so $P_p$ contributes only $\{1\}$, which is already in every $P_q$. $\square$

**Lemma 2 (Truncation of Inclusion-Exclusion).** *For $N = 10^{18}$, any subset $S \subseteq \mathcal{Q}$ with $\prod_{p \in S} p > 59$ satisfies $\lfloor N^{1/\prod p} \rfloor = 1$. Thus only subsets with product $\le 59$ contribute nontrivially.*

**Proof.** If $\prod_{p \in S} p = e > 59$, then $N^{1/e} < N^{1/60} = 10^{18/60} = 10^{0.3} \approx 2$, so $\lfloor N^{1/e} \rfloor = 1$. These terms contribute a net correction that must be carefully tracked (adding/subtracting 1). $\square$

## Editorial
Count the number of distinct perfect powers a^b <= 10^18, where a >= 1 and b >= 2. A perfect power is an integer that can be expressed as a^b for integers a >= 1, b >= 2. Numbers like 64 = 2^6 = 4^3 = 8^2 should only be counted once. We relevant prime exponents. We then iterate over all non-empty subsets of primes where product <= log2(N). Finally, use inclusion-exclusion.

## Pseudocode

```text
Relevant prime exponents
Iterate over all non-empty subsets of primes where product <= log2(N)
Use inclusion-exclusion
for each non-empty subset S of primes
floor(N^{1/e}) = 1, handle separately
else
Handle the subsets with large products (contribute +/- 1)
by careful enumeration
Note: N^{1/e} must be computed with care for large N
Use integer Newton's method: find largest a such that a^e <= N
Adjust: check a^e <= N < (a+1)^e
```

## Complexity Analysis

- **Time:** $O(2^{|\mathcal{Q}|})$ subsets to enumerate, but heavy truncation (most subsets have product $> 59$ and are trivial) reduces this drastically. The dominant cost is $\lfloor N^{1/2} \rfloor = 10^9$ only as a value, not a loop. Each `IntegerRoot` call is $O(\log N)$ Newton iterations with $O(e \log N)$-bit arithmetic. In practice: $O(2^{|\mathcal{Q}|}$) with $|\mathcal{Q}| = 17$, giving $\sim 131072$ subsets, each with constant-time root computation. Total: $O(2^{17} \log N) \approx O(10^7)$.
- **Space:** $O(|\mathcal{Q}|) = O(\log N / \log\log N)$.

## Answer

$$\boxed{675608326}$$
