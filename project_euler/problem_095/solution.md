# Problem 95: Amicable Chains

## Problem Statement

Define $s(n) = \sigma(n) - n$, the sum of proper divisors of $n$. An *amicable chain* of length $k$ is a cycle $n_1 \to n_2 \to \cdots \to n_k \to n_1$ under the map $s$, where all $n_i$ are distinct. Find the smallest element of the longest amicable chain with every element at most $10^6$.

## Mathematical Foundation

**Definition 1 (Sum of proper divisors).** For $n \ge 1$, define
$$s(n) = \sigma(n) - n = \sum_{\substack{d \mid n \\ 1 \le d < n}} d.$$
Note $s(1) = 0$.

**Definition 2 (Amicable chain).** An *amicable chain* of length $k \ge 1$ is a sequence of $k$ distinct positive integers $(n_1, n_2, \ldots, n_k)$ satisfying $s(n_i) = n_{i+1}$ for $1 \le i < k$ and $s(n_k) = n_1$. The cases $k = 1$ (perfect numbers) and $k = 2$ (amicable pairs) are classical.

**Theorem 1 (Divisor sum sieve).** The function $s(n)$ for all $n \le N$ can be computed in $O(N \log N)$ time and $O(N)$ space.

**Proof.** Initialize an array $s[1..N] = 0$. For each $d = 1, 2, \ldots, N$, add $d$ to $s[kd]$ for $k = 2, 3, \ldots, \lfloor N/d \rfloor$. After this process, $s[n] = \sum_{\substack{d \mid n, d < n}} d$ because every proper divisor $d < n$ of $n$ is enumerated exactly once when we process divisor $d$ and add it to $s[d \cdot (n/d)] = s[n]$.

The total number of additions is $\sum_{d=1}^{N}(\lfloor N/d \rfloor - 1) \le \sum_{d=1}^{N} N/d = N H_N = O(N \log N)$, where $H_N = \sum_{d=1}^{N} 1/d$ is the $N$-th harmonic number. $\square$

**Lemma 1 (Functional iteration and cycle detection).** Let $f: S \to S$ be a function on a finite set $S$. For any $x_0 \in S$, the sequence $x_0, f(x_0), f^2(x_0), \ldots$ eventually enters a cycle. If we track visited elements and the sequence returns to $x_0$, the elements visited form an amicable chain containing $x_0$.

**Proof.** Since $S$ is finite, the sequence must revisit some element. If the first revisited element is $x_0$ itself, the trajectory from $x_0$ back to $x_0$ forms a cycle, which is an amicable chain by definition. If the first revisited element is some $x_j \ne x_0$, then $x_0$ is on a "tail" leading into the cycle at $x_j$ and is not part of any cycle. $\square$

**Theorem 2 (Amortized complexity of chain detection).** Using a visited-flag array, the total work for chain detection across all starting points $n = 2, \ldots, N$ is $O(N)$.

**Proof.** Once a number is marked visited, it is never the starting point of a new exploration and causes any exploration encountering it to terminate immediately. Each number is visited at most once as a "fresh" element during some exploration and at most once as a "termination trigger" for a different exploration. Hence the total number of steps across all explorations is at most $2N$. $\square$

## Editorial
Phase 1: Sieve for sum of proper divisors s(n) in O(N log N). Phase 2: Detect cycles via functional iteration in O(N) total. We detect cycles.

## Pseudocode

```text
Compute s[n] for n = 1..N via sieve
Detect cycles
if n is in positions
```

## Complexity Analysis

**Time:** $O(N \log N)$ for the sieve (Theorem 1) plus $O(N)$ for cycle detection (Theorem 2). Total: $O(N \log N)$.

**Space:** $O(N)$ for the arrays $s[\cdot]$ and visited$[\cdot]$.

## Answer

$$\boxed{14316}$$
