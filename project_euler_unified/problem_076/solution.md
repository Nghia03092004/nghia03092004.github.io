# Problem 76: Counting Summations

## Problem Statement

It is possible to write five as a sum in exactly six different ways:

```
4 + 1
3 + 2
3 + 1 + 1
2 + 2 + 1
2 + 1 + 1 + 1
1 + 1 + 1 + 1 + 1
```

How many different ways can one hundred be written as a sum of at least two positive integers?

## Mathematical Foundation

**Definition 1 (Integer Partition).** A *partition* of a non-negative integer $n$ is a multiset $\{a_1, a_2, \ldots, a_k\}$ of positive integers satisfying $\sum_{i=1}^{k} a_i = n$. Two partitions are identical if and only if they contain the same parts with the same multiplicities. The *partition function* $p(n)$ counts the number of partitions of $n$. By convention, $p(0) = 1$ (the empty partition).

**Definition 2 (Restricted Partition).** Let $p_{\le m}(n)$ denote the number of partitions of $n$ into parts each at most $m$. Equivalently, $p_{\le m}(n)$ counts the number of solutions in non-negative integers $(c_1, c_2, \ldots, c_m)$ to $\sum_{k=1}^{m} k \cdot c_k = n$, where $c_k$ represents the multiplicity of part $k$. Note that $p(n) = p_{\le n}(n)$.

**Theorem 1 (Generating Function for $p(n)$).** *The ordinary generating function for the partition function is*
$$\sum_{n=0}^{\infty} p(n)\,x^n = \prod_{k=1}^{\infty} \frac{1}{1 - x^k}.$$

*Proof.* For each positive integer $k$, the geometric series $\frac{1}{1-x^k} = \sum_{c_k=0}^{\infty} x^{c_k \cdot k}$ encodes the choice of $c_k \geq 0$ copies of part $k$ contributing $c_k \cdot k$ to the total. The Cauchy product of all such series over $k \geq 1$ yields
$$\prod_{k=1}^{\infty} \frac{1}{1-x^k} = \sum_{n=0}^{\infty} \left(\sum_{\substack{c_1, c_2, \ldots \geq 0 \\ \sum_k k c_k = n}} 1\right) x^n = \sum_{n=0}^{\infty} p(n)\,x^n.$$
Absolute convergence for $|x| < 1$ follows from $\sum_{k=1}^{\infty} \log\frac{1}{1-|x|^k} \leq \sum_{k=1}^{\infty} \frac{|x|^k}{1-|x|^k} < \infty$, since the terms are $O(|x|^k)$ and $\sum |x|^k$ converges. $\blacksquare$

**Theorem 2 (Euler's Pentagonal Number Theorem).** *As formal power series (and for $|x| < 1$):*
$$\prod_{k=1}^{\infty}(1 - x^k) = \sum_{j=-\infty}^{\infty} (-1)^j x^{j(3j-1)/2} = 1 + \sum_{j=1}^{\infty} (-1)^j \bigl(x^{j(3j-1)/2} + x^{j(3j+1)/2}\bigr).$$

*Proof (Franklin's Involution).* The coefficient of $x^n$ on the left is $\sum_{\lambda \in \mathcal{D}_n} (-1)^{\ell(\lambda)}$, where $\mathcal{D}_n$ denotes the set of partitions of $n$ into distinct parts and $\ell(\lambda)$ is the number of parts.

For $\lambda = (\lambda_1 > \lambda_2 > \cdots > \lambda_r) \in \mathcal{D}_n$, define two parameters:
- $s(\lambda) = \lambda_r$: the smallest part.
- $t(\lambda)$: the length of the maximal sequence of consecutive integers descending from $\lambda_1$. That is, $t(\lambda) = \max\{m \geq 1 : \lambda_1 - i + 1 \in \{\lambda_1, \ldots, \lambda_r\} \text{ for all } 1 \leq i \leq m\}$.

Define the map $\phi: \mathcal{D}_n \to \mathcal{D}_n$ as follows:
- **Move A** (applicable when $s < t$, or when $s = t$ and $\lambda_r \neq \lambda_1 - t + 1$): Remove the smallest part $\lambda_r = s$ and add $1$ to each of the $s$ largest parts $\lambda_1, \lambda_2, \ldots, \lambda_s$. This produces a partition with $r - 1$ distinct parts.
- **Move B** (applicable when $s > t$, or when Move A is inapplicable): Subtract $1$ from each of the $t$ largest parts $\lambda_1, \ldots, \lambda_t$, and insert $t$ as a new smallest part. This produces a partition with $r + 1$ distinct parts.

One verifies that (i) Moves A and B are mutual inverses, (ii) each changes $\ell(\lambda)$ by exactly $\pm 1$, hence reverses the sign $(-1)^{\ell(\lambda)}$, and (iii) the map $\phi$ is undefined (yielding fixed points) precisely when the two moves coincide or become degenerate. This occurs when:
- $s = t$ and $\lambda_r = \lambda_1 - t + 1$: partition is $(r, r+1, \ldots, 2r-1)$, giving $n = r(3r-1)/2$.
- $s = t + 1$ and $r = s$: gives $n = r(3r+1)/2$.

At these fixed points the sign contribution is $(-1)^r$. For all other $\lambda$, the map $\phi$ pairs each partition with one of opposite sign, so the net contribution cancels. $\blacksquare$

**Theorem 3 (Partition Recurrence via Pentagonal Numbers).** *For $n \geq 1$:*
$$p(n) = \sum_{j=1}^{\infty} (-1)^{j+1} \left[p\!\left(n - \tfrac{j(3j-1)}{2}\right) + p\!\left(n - \tfrac{j(3j+1)}{2}\right)\right],$$
*where $p(0) = 1$, $p(m) = 0$ for $m < 0$, and the sum terminates when both pentagonal arguments become negative.*

*Proof.* Multiplying the generating functions from Theorems 1 and 2:
$$\left(\sum_{n=0}^{\infty} p(n)x^n\right)\left(\prod_{k=1}^{\infty}(1-x^k)\right) = 1.$$
Extracting the coefficient of $x^n$ for $n \geq 1$:
$$p(n) + \sum_{j=1}^{\infty} (-1)^j \left[p\!\left(n - \tfrac{j(3j-1)}{2}\right) + p\!\left(n - \tfrac{j(3j+1)}{2}\right)\right] = 0.$$
Rearranging and noting that $(-1)^{j} \cdot (-1) = (-1)^{j+1}$ yields the stated recurrence. $\blacksquare$

**Theorem 4 (Correctness of Partition DP).** *Define the array $\mathrm{dp}[0..n]$ by $\mathrm{dp}[0] = 1$, $\mathrm{dp}[j] = 0$ for $1 \leq j \leq n$. For $k = 1, 2, \ldots, n$ and for $j = k, k+1, \ldots, n$, perform $\mathrm{dp}[j] \mathrel{+}= \mathrm{dp}[j - k]$. Then upon termination, $\mathrm{dp}[n] = p(n)$.*

*Proof.* We prove by induction on $k$ that after processing part size $k$, the value $\mathrm{dp}[j]$ equals $p_{\le k}(j)$ for all $0 \leq j \leq n$.

*Base case ($k = 0$):* Before any updates, $\mathrm{dp}[0] = 1 = p_{\le 0}(0)$ and $\mathrm{dp}[j] = 0 = p_{\le 0}(j)$ for $j \geq 1$. This is correct since the only partition using parts $\leq 0$ is the empty partition of $0$.

*Inductive step:* Assume $\mathrm{dp}[j] = p_{\le k-1}(j)$ for all $j$ before processing part size $k$. The update loop sets $\mathrm{dp}[j] \leftarrow \mathrm{dp}[j] + \mathrm{dp}[j - k]$ for $j = k, k+1, \ldots, n$. After this pass, $\mathrm{dp}[j]$ accumulates:
$$\mathrm{dp}[j] = p_{\le k-1}(j) + \sum_{m=1}^{\lfloor j/k \rfloor} p_{\le k-1}(j - mk),$$
since the forward iteration (increasing $j$) allows the value $\mathrm{dp}[j - k]$ to already reflect uses of part $k$. This telescopes to the partition identity $p_{\le k}(j) = \sum_{m=0}^{\lfloor j/k \rfloor} p_{\le k-1}(j - mk)$, which counts partitions of $j$ with parts $\leq k$ by conditioning on the multiplicity $m$ of part $k$. After $k = n$, we have $\mathrm{dp}[n] = p_{\le n}(n) = p(n)$. $\blacksquare$

**Remark.** The problem asks for partitions into at least two parts, which excludes the trivial partition $n = n$. The answer is therefore $p(100) - 1$.

## Algorithm

```
function partition_count(n):
    dp = array[0..n] initialized to 0
    dp[0] = 1

    for k = 1 to n:
        for j = k to n:
            dp[j] += dp[j - k]

    return dp[n] - 1    # subtract 1 for the trivial partition n = n
```

## Complexity Analysis

**Time:** The nested loops execute $\sum_{k=1}^{n}(n - k + 1) = \sum_{i=1}^{n} i = n(n+1)/2 = O(n^2)$ iterations, each performing $O(1)$ arithmetic. For $n = 100$, this is $5050$ operations.

**Space:** The one-dimensional DP array requires $n + 1 = O(n)$ entries.

**Alternative:** The pentagonal number recurrence (Theorem 3) computes each $p(k)$ in $O(\sqrt{k})$ time, yielding $O(n\sqrt{n})$ total time.

## Answer

$$\boxed{190569291}$$
