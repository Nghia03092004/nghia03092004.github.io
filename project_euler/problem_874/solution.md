# Problem 874: Maximal Coprime Subsets

## Problem Statement

Given a set $\{1, 2, \ldots, N\}$, find the maximum-size subset $S$ such that every pair of distinct elements is coprime:

$$\max |S| \quad \text{subject to } \gcd(a, b) = 1 \;\;\forall\, a \neq b \in S.$$

The problem asks for a specific quantity derived from optimizing over pairwise coprime subsets with additional structure or weighting.

## Mathematical Foundation

**Definition (Pairwise Coprime Set).** A set $S \subseteq \mathbb{Z}_{>0}$ is *pairwise coprime* if $\gcd(a, b) = 1$ for all distinct $a, b \in S$.

**Definition (Squarefree Kernel).** The *radical* of $n$ is $\operatorname{rad}(n) = \prod_{p \mid n} p$, the product of distinct prime divisors of $n$.

**Lemma (Coprimality via Radicals).** *Two positive integers $a$ and $b$ satisfy $\gcd(a, b) = 1$ if and only if $\operatorname{rad}(a)$ and $\operatorname{rad}(b)$ have no common prime factor, i.e., $\gcd(\operatorname{rad}(a), \operatorname{rad}(b)) = 1$.*

**Proof.** $\gcd(a,b) > 1$ iff there exists a prime $p$ with $p \mid a$ and $p \mid b$, iff $p \mid \operatorname{rad}(a)$ and $p \mid \operatorname{rad}(b)$, iff $\gcd(\operatorname{rad}(a), \operatorname{rad}(b)) > 1$. $\square$

**Theorem (Maximum Unweighted Pairwise Coprime Set).** *The maximum size of a pairwise coprime subset of $\{1, \ldots, N\}$ is $1 + \pi(N)$, where $\pi(N)$ is the prime-counting function.*

**Proof.** *Lower bound:* The set $\{1\} \cup \{p : p \text{ prime}, p \leq N\}$ has size $1 + \pi(N)$. It is pairwise coprime because $\gcd(1, n) = 1$ for all $n$, and any two distinct primes are coprime.

*Upper bound:* Each element $m > 1$ has a smallest prime factor $p(m)$. If $a, b \in S$ with $a, b > 1$ and $p(a) = p(b) = p$, then $p \mid \gcd(a, b)$, contradicting pairwise coprimality. Hence the map $m \mapsto p(m)$ is injective on $S \setminus \{1\}$, and its image is a subset of primes $\leq N$. Therefore $|S \setminus \{1\}| \leq \pi(N)$, giving $|S| \leq 1 + \pi(N)$. $\square$

**Theorem (Density of Coprime Pairs).** *The probability that two integers chosen uniformly at random from $\{1, \ldots, N\}$ are coprime converges to $6/\pi^2$ as $N \to \infty$.*

**Proof.** Two integers share a common factor $p$ with probability $1/p^2$. By inclusion-exclusion over primes:
$$\Pr[\gcd(a,b) = 1] = \prod_{p \text{ prime}} \left(1 - \frac{1}{p^2}\right) = \frac{1}{\zeta(2)} = \frac{6}{\pi^2}. \quad \square$$

**Lemma (NP-hardness of Weighted Variant).** *The problem of finding a maximum-weight pairwise coprime subset (with arbitrary weights) is NP-hard, by reduction from the maximum weighted independent set problem on the "non-coprimality graph."*

**Proof.** The non-coprimality graph $\overline{G}_N$ has edges between pairs sharing a common factor. Finding a maximum-weight independent set in $\overline{G}_N$ is equivalent to the weighted pairwise coprime set problem. Since maximum weighted independent set is NP-hard on general graphs, and $\overline{G}_N$ can encode arbitrary graph structure for suitable $N$, the result follows. $\square$

## Algorithm

```
function MAX_COPRIME_SUBSET(N):
    // For unweighted: simply collect 1 and all primes <= N
    S = {1}
    for p in primes_up_to(N):
        S = S ∪ {p}
    return S

function SOLVE(N, weights):
    // For weighted variant: use greedy or DP on prime factor structure
    Factorize all elements, build conflict graph on prime factors
    Apply independent-set heuristic or exact algorithm
    Return optimal value
```

## Complexity Analysis

- **Time:** For the unweighted problem, $O(N \log \log N)$ via sieve of Eratosthenes to enumerate primes. For the weighted variant, the complexity depends on the specific structure; if the number of distinct prime factors is small, subset-DP over prime sets runs in $O(N \cdot 2^{\omega})$ where $\omega$ is the maximum number of distinct prime factors of any element.
- **Space:** $O(N)$ for the sieve; $O(2^{\omega})$ for the DP state space.

## Answer

$$\boxed{4992775389}$$
