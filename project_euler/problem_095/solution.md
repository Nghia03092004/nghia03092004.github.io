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
The map \(n \mapsto s(n)\) turns the problem into a functional graph on the integers up to \(10^6\). The first phase builds that graph efficiently with a divisor-sum sieve, so after that every node has exactly one outgoing edge.

The second phase is then standard cycle hunting in a functional graph. Starting from each unseen value, we follow successors until one of three things happens: the chain leaves the allowed range, it reaches a node already settled by an earlier exploration, or it revisits a node from the current walk. Only the third case creates a new amicable chain. The local position map identifies the cycle portion exactly, and the global visited array prevents the same tail from being explored again.

## Pseudocode

```text
Compute the sum of proper divisors \(s(n)\) for every \(n \le N\) with a sieve.

Create a global visited array.
Set the best chain length and best minimum element to 0.

For each starting value from 2 to \(N\):
    If it is already globally visited, skip it

    Follow the map \(n \mapsto s(n)\), recording:
        the current chain in order
        the position where each value first appears

    Stop when the chain leaves the range, reaches a globally visited value, or repeats a value from the current walk

    If a repeat occurred inside the current walk:
        extract the cycle part
        update the best answer if this cycle is longer

    Mark every value from the explored walk as globally visited

Return the smallest element from the longest cycle found.
```

## Complexity Analysis

**Time:** $O(N \log N)$ for the sieve (Theorem 1) plus $O(N)$ for cycle detection (Theorem 2). Total: $O(N \log N)$.

**Space:** $O(N)$ for the arrays $s[\cdot]$ and visited$[\cdot]$.

## Answer

$$\boxed{14316}$$
