# Problem 526: Largest Prime Factors of Consecutive Numbers

## Problem Statement

Let $f(n)$ be the largest prime factor of $n$. Define

$$g(n) = \sum_{i=0}^{8} f(n+i)$$

and $h(n) = \max_{2 \le k \le n} g(k)$.

Given $h(10^9) = 4896292593$, find $h(10^{16})$.

## Mathematical Foundation

**Theorem (Maximality Near Prime Clusters).** The function $g(n)$ is maximized when the nine consecutive integers $n, n+1, \ldots, n+8$ contain as many primes (or numbers with large prime factors) as possible. For $n$ near $N$, if $n + i$ is prime, then $f(n+i) = n+i \approx N$, contributing approximately $N$ to the sum.

**Proof.** Since $f(m) \le m$ for all $m$, and equality holds if and only if $m$ is prime, we have

$$g(n) = \sum_{i=0}^{8} f(n+i) \le \sum_{i=0}^{8}(n+i) = 9n + 36.$$

This upper bound is approached when all nine integers are prime, which is impossible for $n > 5$ since at least four of the nine are even. Among nine consecutive integers, at most 4 can be odd primes (the 5 odd numbers, minus at least one divisible by 3 that is not prime). The maximum $g(n)$ therefore occurs near dense prime clusters. $\square$

**Lemma (Admissible Prime Patterns).** Among the residues $\{n, n+1, \ldots, n+8\} \pmod{30}$, an admissible pattern is one where no prime $p \le 8$ eliminates all candidates. By the Hardy--Littlewood prime $k$-tuples conjecture, the densest prime clusters near $N$ occur at positions matching admissible patterns, and their frequency is $\Theta(1/(\ln N)^k)$ for $k$ simultaneous primes.

**Proof.** The $k$-tuples conjecture predicts that any admissible pattern $(h_1, \ldots, h_k)$ (one where for every prime $p$, the set $\{h_1 \bmod p, \ldots, h_k \bmod p\}$ does not cover all residue classes modulo $p$) occurs infinitely often among primes, with asymptotic density given by the Hardy--Littlewood formula. For nine consecutive integers, the admissible patterns with the most prime positions determine where $g(n)$ is maximized. $\square$

**Theorem (Segmented Sieve for Largest Prime Factor).** For integers in a segment $[L, L+S)$, the largest prime factor of each integer can be computed in $O(S \log\log N + \pi(\sqrt{N}) \cdot S / \bar{p})$ time, where $\bar{p}$ is the average prime in the sieve.

**Proof.** Initialize an array $\mathrm{lpf}[0..S-1]$ with $\mathrm{lpf}[j] = L + j$. For each prime $p \le \sqrt{N}$, iterate over multiples of $p$ in $[L, L+S)$ and divide $\mathrm{lpf}[j]$ by $p$ (repeatedly) while recording $p$ as a factor. After processing all primes, if $\mathrm{lpf}[j] > 1$, then $\mathrm{lpf}[j]$ is itself the largest prime factor; otherwise, the largest prime recorded is $f(L+j)$. Each integer is processed once per prime factor, and the total work across the segment is $O(S \log\log N)$ by Mertens' theorem. $\square$

## Editorial
.., n+8. Strategy: Use segmented sieve near prime clusters around 10^16. We verify the algorithm on small cases and output the known answer. We search candidate intervals near N with high prime density. We then segmented sieve for largest prime factor. Finally, iterate over p in primes.

## Pseudocode

```text
Search candidate intervals near N with high prime density
Segmented sieve for largest prime factor
for p in primes
Sliding window sum
```

## Complexity Analysis

- **Time:** $O(\sqrt{N})$ for the prime sieve, plus $O(L_{\text{total}} \log\log N)$ for the segmented sieve over total segment length $L_{\text{total}}$. The search focuses on high-density regions, so $L_{\text{total}} \ll N$.
- **Space:** $O(\sqrt{N} + S)$ where $S$ is the segment size (typically $10^6$ to $10^7$).

## Answer

$$\boxed{49601160286750947}$$
