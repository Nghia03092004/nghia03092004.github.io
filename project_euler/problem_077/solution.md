# Problem 77: Prime Summations

## Problem Statement

It is possible to write ten as the sum of primes in exactly five different ways:

```
7 + 3
5 + 5
5 + 3 + 2
3 + 3 + 2 + 2
2 + 2 + 2 + 2 + 2
```

What is the first value which can be written as the sum of primes in over five thousand different ways?

## Mathematical Foundation

**Definition 1 (Prime Partition Function).** Let $q(n)$ denote the number of partitions of $n$ into prime parts, i.e., the number of ways to express $n$ as an unordered sum of primes with repetition allowed. By convention, $q(0) = 1$ (the empty sum).

**Theorem 1 (Generating Function for Prime Partitions).** *The ordinary generating function for $q(n)$ is*
$$\sum_{n=0}^{\infty} q(n)\,x^n = \prod_{p \;\text{prime}} \frac{1}{1 - x^p}.$$

*Proof.* For each prime $p$, the factor $\frac{1}{1 - x^p} = \sum_{c_p=0}^{\infty} x^{c_p \cdot p}$ encodes the choice of $c_p \geq 0$ copies of $p$ contributing $c_p \cdot p$ to the sum. Taking the product over all primes, the coefficient of $x^n$ in the resulting series counts the number of solutions in non-negative integers $\{c_p\}_{p\;\text{prime}}$ to $\sum_p c_p \cdot p = n$, which is $q(n)$.

Absolute convergence for $|x| < 1$ follows from $\sum_{p} |x|^p \leq \sum_{k=2}^{\infty} |x|^k = \frac{|x|^2}{1 - |x|} < \infty$, so the infinite product converges. $\blacksquare$

**Definition 2 (Restricted Prime Partition).** Let $q_{\le p_i}(n)$ denote the number of partitions of $n$ using only primes from $\{p_1, p_2, \ldots, p_i\}$, where $p_1 < p_2 < \cdots$ is the sequence of primes.

**Theorem 2 (Correctness of the Prime Partition DP).** *Initialize $\mathrm{dp}[0] = 1$ and $\mathrm{dp}[j] = 0$ for $j \geq 1$. For each prime $p \leq N$ in increasing order, update $\mathrm{dp}[j] \mathrel{+}= \mathrm{dp}[j - p]$ for $j = p, p+1, \ldots, N$. Then $\mathrm{dp}[n] = q(n)$ for all $0 \leq n \leq N$.*

*Proof.* We prove by induction on the index $i$ (over the prime sequence $p_1 < p_2 < \cdots$) that after processing prime $p_i$, the value $\mathrm{dp}[j] = q_{\le p_i}(j)$ for all $0 \leq j \leq N$.

*Base case ($i = 0$):* Before processing any prime, $\mathrm{dp}[0] = 1 = q_{\le \emptyset}(0)$ and $\mathrm{dp}[j] = 0 = q_{\le \emptyset}(j)$ for $j \geq 1$.

*Inductive step:* Suppose $\mathrm{dp}[j] = q_{\le p_{i-1}}(j)$ for all $j$. The update $\mathrm{dp}[j] \leftarrow \mathrm{dp}[j] + \mathrm{dp}[j - p_i]$ for $j = p_i, p_i + 1, \ldots, N$ (in increasing order) adds the count of partitions containing at least one copy of $p_i$. The forward iteration permits unbounded repetition of $p_i$, since $\mathrm{dp}[j - p_i]$ already reflects earlier uses of $p_i$ in the same pass. After this pass:
$$\mathrm{dp}[j] = \sum_{m=0}^{\lfloor j/p_i \rfloor} q_{\le p_{i-1}}(j - m \cdot p_i) = q_{\le p_i}(j),$$
by the identity that conditions on the multiplicity $m$ of $p_i$.

After all primes $p \leq N$ are processed, $\mathrm{dp}[n] = q(n)$ for $n \leq N$, since no prime exceeding $n$ can appear in a partition of $n$. $\blacksquare$

**Lemma 1 (Sufficient Search Bound).** *There exists $n \leq 100$ with $q(n) > 5000$.*

*Proof.* The function $q(n)$ is monotonically non-decreasing for $n \geq 2$ (since every partition of $n-1$ into primes can be extended by adding $1$ to some part or by inclusion). Direct computation yields $q(71) = 5007 > 5000$, confirming the bound. $\blacksquare$

## Editorial
The structure is the same as the ordinary partition DP, except the allowed part sizes are now primes. We first generate all primes up to a safe search bound, then process them one by one in increasing order. After a prime $p$ has been processed, each table entry counts the representations that use only primes up to $p$.

Adding prime $p$ works exactly like the unbounded-knapsack transition: every way to make $j-p$ can be extended by one more copy of $p$, so it contributes to the count for $j$. Using primes in increasing order prevents different orders of the same sum from being counted separately. Once the table is filled, we scan upward and stop at the first value whose count exceeds 5000.

## Pseudocode

```text
Choose a search bound that is known to contain the answer.
Generate every prime up to that bound.

Create a counting table for totals up to the bound and set the value for 0 to 1.

For each prime:
    For each total that is at least that prime:
        add the number of representations for the reduced total

Scan the totals from 2 upward.
Return the first total whose count is greater than 5000.
```

## Complexity Analysis

**Time:** The Sieve of Eratosthenes computes all primes up to $N$ in $O(N \log \log N)$ time. The DP phase performs $\sum_{p \leq N} (N - p + 1) \leq \pi(N) \cdot N$ iterations, each in $O(1)$. By the Prime Number Theorem, $\pi(N) \sim N / \ln N$, so the DP cost is $O(N^2 / \log N)$. For $N = 100$: $\pi(100) = 25$, yielding at most $25 \times 100 = 2500$ operations.

**Space:** $O(N)$ for the DP array and the prime list.

## Answer

$$\boxed{71}$$
