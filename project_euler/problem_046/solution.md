# Problem 46: Goldbach's Other Conjecture

## Problem Statement

It was proposed by Christian Goldbach that every odd composite number can be written as the sum of a prime and twice a square. It turns out that the conjecture was false. What is the smallest odd composite that cannot be written as the sum of a prime and twice a square?

## Mathematical Development

### Formal Development

**Definition 1 (Goldbach Representability).** An odd integer $n > 1$ is *Goldbach-representable* if there exist a prime $p$ and an integer $k \geq 1$ such that $n = p + 2k^2$. Let $\mathcal{G}$ denote the set of all Goldbach-representable odd integers.

**Definition 2 (Odd Composites).** Let $\mathcal{C}_{\mathrm{odd}} = \{n \in \mathbb{Z} : n > 1,\ n \text{ is odd},\ n \text{ is not prime}\}$.

Goldbach's other conjecture asserts that $\mathcal{C}_{\mathrm{odd}} \subseteq \mathcal{G}$. We seek $\min(\mathcal{C}_{\mathrm{odd}} \setminus \mathcal{G})$.

**Lemma 1 (Bound on the square index).** *Let $n \in \mathcal{C}_{\mathrm{odd}}$. If $n \in \mathcal{G}$, then there exists a witness $(p, k)$ with $1 \leq k \leq K(n)$, where $K(n) := \lfloor\sqrt{(n - 2)/2}\rfloor$.*

*Proof.* Suppose $n = p + 2k^2$ with $p$ prime and $k \geq 1$. Since $p \geq 2$, we have $2k^2 = n - p \leq n - 2$, whence $k^2 \leq (n - 2)/2$. Therefore $k \leq \lfloor\sqrt{(n-2)/2}\rfloor = K(n)$. $\square$

**Lemma 2 (Parity of the remainder).** *For any odd integer $n$ and any integer $k \geq 1$, the value $r_k := n - 2k^2$ is odd.*

*Proof.* Since $n$ is odd and $2k^2$ is even, their difference is odd. $\square$

**Lemma 3 (Reduction to primality testing).** *An odd composite $n$ belongs to $\mathcal{G}$ if and only if there exists $k \in \{1, 2, \ldots, K(n)\}$ such that $n - 2k^2$ is prime.*

*Proof.* ($\Rightarrow$) By definition, $n = p + 2k^2$ with $p$ prime and $k \geq 1$. By Lemma 1, $k \leq K(n)$, so $p = n - 2k^2$ is prime for some $k$ in the stated range.

($\Leftarrow$) If $n - 2k^2$ is prime for some $1 \leq k \leq K(n)$, set $p = n - 2k^2$. Then $n = p + 2k^2$ with $p$ prime and $k \geq 1$, so $n \in \mathcal{G}$. $\square$

**Theorem 1.** *The smallest odd composite number that is not Goldbach-representable is $5777$. That is, $\min(\mathcal{C}_{\mathrm{odd}} \setminus \mathcal{G}) = 5777$.*

*Proof.* The proof proceeds in three parts.

*Part 1 (5777 is an odd composite).* We verify $5777 = 53 \times 109$, where $53$ and $109$ are both prime. Hence $5777$ is composite. Since $53$ and $109$ are both odd, their product is odd. Thus $5777 \in \mathcal{C}_{\mathrm{odd}}$.

*Part 2 (5777 is not Goldbach-representable).* By Lemma 3, it suffices to show that $5777 - 2k^2$ is composite for every $k \in \{1, 2, \ldots, K(5777)\}$. We compute $K(5777) = \lfloor\sqrt{(5777 - 2)/2}\rfloor = \lfloor\sqrt{2887.5}\rfloor = 53$. For each $k \in \{1, \ldots, 53\}$, define $r_k = 5777 - 2k^2$. By Lemma 2, each $r_k$ is odd, so the question reduces to whether any $r_k$ is an odd prime. Exhaustive computation confirms that none of $r_1, r_2, \ldots, r_{53}$ is prime. Representative values:

| $k$ | $r_k = 5777 - 2k^2$ | Factorization |
|-----|----------------------|---------------|
| 1   | 5775                 | $3 \times 5^2 \times 7 \times 11$ |
| 2   | 5769                 | $3 \times 1923$ |
| 3   | 5759                 | $13 \times 443$ |
| $\vdots$ | $\vdots$ | composite |
| 53  | 159                  | $3 \times 53$ |

Since no witness exists, $5777 \notin \mathcal{G}$.

*Part 3 (Minimality).* By exhaustive computation (via sieve-assisted primality lookup), every odd composite $n$ with $9 \leq n < 5777$ satisfies $n \in \mathcal{G}$: for each such $n$, at least one $k \in \{1, \ldots, K(n)\}$ yields a prime remainder $n - 2k^2$.

Combining Parts 1--3, $5777 = \min(\mathcal{C}_{\mathrm{odd}} \setminus \mathcal{G})$. $\square$

## Algorithm

We first build a primality table up to a search limit using the Sieve of Eratosthenes. Then we inspect odd composite integers in increasing order, and for each such $n$ we test all values of $k$ with $2k^2 < n$ to see whether the remainder $n - 2k^2$ is prime. The first odd composite for which no such witness exists is the smallest counterexample to Goldbach's other conjecture.

## Pseudocode

```text
Algorithm: Smallest Odd Composite Violating Goldbach's Other Conjecture
Require: A prime table on a search interval.
Ensure: The smallest odd composite n that cannot be written as p + 2k^2.
1: Build a primality table on the search interval.
2: For each odd composite n in increasing order do:
3:     For each k ≥ 1 with 2k^2 < n, test whether n - 2k^2 is prime.
4:     If no such k succeeds, return n.
```

## Complexity Analysis

**Proposition (Time complexity).** *Let $N = 5777$ denote the answer. The algorithm runs in $O(N\sqrt{N} + N\log\log N)$ time.*

*Proof.* The sieve of Eratosthenes on $[0, L]$ with $L = O(N)$ runs in $O(L \log \log L) = O(N \log \log N)$ time (classical result). The main loop iterates over at most $(N - 9)/2 + 1 = O(N)$ odd integers. For each odd composite $n$, the inner loop runs at most $K(n) = O(\sqrt{n}) = O(\sqrt{N})$ iterations, each performing an $O(1)$ sieve lookup. The total work in the main loop is therefore $\sum_{n \leq N} O(\sqrt{n}) = O(N\sqrt{N})$. Since $N\sqrt{N}$ dominates $N \log\log N$, the overall complexity is $O(N\sqrt{N})$. With $N = 5777$, this yields approximately $4.4 \times 10^5$ operations. $\square$

**Proposition (Space complexity).** *The algorithm uses $O(N)$ space.*

*Proof.* The sieve array of size $L + 1 = O(N)$ is the only data structure. All other variables are $O(1)$. $\square$

## Answer

$$\boxed{5777}$$
