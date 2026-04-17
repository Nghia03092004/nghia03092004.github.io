# Problem 60: Prime Pair Sets

## Problem Statement

Define two primes $p$ and $q$ to be a *prime pair* if both concatenations $p \| q$ and $q \| p$ are themselves prime. Find the smallest sum of a set of five primes in which every pair of distinct elements forms a prime pair.

## Mathematical Development

**Definition 1 (Concatenation).** For positive integers $a$ and $b$, define the *concatenation*

$$a \| b = a \cdot 10^{d(b)} + b,$$

where $d(b) = \lfloor \log_{10} b \rfloor + 1$ is the number of decimal digits of $b$.

**Definition 2 (Prime Pair).** Primes $p$ and $q$ form a *prime pair*, written $p \sim q$, if both $p \| q$ and $q \| p$ are prime.

**Definition 3 (Prime Pair Set).** A set $S$ of primes is a *prime pair set of size $k$* if every pair of distinct elements in $S$ satisfies the prime pair relation. Equivalently, $S$ is a clique of size $k$ in the *prime pair graph*.

**Definition 4 (Prime Pair Graph).** The *prime pair graph* is the undirected graph $G = (V, E)$ where $V$ is a set of primes and $\{p, q\} \in E$ if and only if $p \sim q$.

**Theorem 1 (Modular Necessary Condition).** Let $p, q > 3$ be distinct primes. If $p \not\equiv q \pmod{3}$, then $p \not\sim q$.

*Proof.* Since $10 \equiv 1 \pmod{3}$, we have $10^d \equiv 1 \pmod{3}$ for all $d \geq 1$. Therefore

$$p \| q = p \cdot 10^{d(q)} + q \equiv p + q \pmod{3}.$$

For primes $p, q > 3$, we have $p, q \not\equiv 0 \pmod{3}$, so $p \in \{1, 2\} \pmod{3}$ and likewise for $q$. If $p \equiv 1$ and $q \equiv 2$ (or vice versa), then $p + q \equiv 0 \pmod{3}$. Since $p \| q > 3$, divisibility by 3 implies $p \| q$ is composite. By the same argument, $q \| p$ is also composite. Hence $p \not\sim q$. $\square$

**Corollary 1 (Residue Class Constraint).** In any prime pair set of size $\geq 2$ consisting entirely of primes $> 3$, all elements share the same residue modulo 3.

*Proof.* Immediate from Theorem 1 applied to each pair. $\square$

**Lemma 1 (The Prime 3 is Special).** The prime 3 can form a prime pair with primes of either residue class modulo 3. Specifically, for $p > 3$: $3 + p \equiv p \pmod{3}$, which is nonzero since $p \not\equiv 0 \pmod{3}$. Hence the mod-3 obstruction does not apply to pairs involving 3.

*Proof.* $3 \| p = 3 \cdot 10^{d(p)} + p \equiv 3 + p \equiv p \pmod{3} \not\equiv 0$. Similarly, $p \| 3 = 10p + 3 \equiv p + 3 \equiv p \pmod{3} \not\equiv 0$. So neither concatenation is divisible by 3 (though they may still be composite for other reasons). $\square$

**Theorem 2 (Graph-Theoretic Reduction).** Finding a prime pair set of size 5 with minimum sum is equivalent to finding a minimum-weight 5-clique in the prime pair graph $G$.

*Proof.* By Definition 3, a prime pair set of size 5 is a 5-clique in $G$. The sum of the five primes is the weight. Minimizing over all 5-cliques gives the answer. $\square$

**Lemma 2 (Search Bound).** It suffices to search primes below some bound $B$. If a 5-clique with sum $\Sigma$ is found, then $B = \Sigma$ suffices, since any clique containing a prime $> \Sigma$ has sum $> \Sigma$.

*Proof.* Each prime in the clique is at most $\Sigma$ (since all five are positive and sum to $\Sigma$). More precisely, the largest prime is at most $\Sigma - 4 \cdot 2 = \Sigma - 8$ (since the smallest prime is 2). In practice, $B = 10{,}000$ is sufficient as confirmed by computation. $\square$

**Lemma 3 (Primality of Concatenations).** For primes below $B = 10{,}000$, all concatenations satisfy $p \| q < 10^8$. Deterministic Miller-Rabin primality testing with witness set $\{2, 3, 5, 7, 11, 13\}$ is correct for all integers below $3.2 \times 10^{14}$ (Jaeschke, 1993), which amply covers our range.

*Proof.* The maximum concatenation is $9999 \| 9973 < 10^4 \cdot 10^4 + 10^4 = 10^8 + 10^4 < 3.2 \times 10^{14}$. The cited result guarantees deterministic correctness. $\square$

**Theorem 3 (Incremental Clique Search with Pruning).** The 5-clique search can be organized as a depth-first enumeration with the following pruning:

(i) *Ordering:* enumerate clique elements in increasing order $p_1 < p_2 < p_3 < p_4 < p_5$.

(ii) *Adjacency intersection:* at depth $\ell$, the candidate set for $p_{\ell+1}$ is the intersection of the adjacency lists of $p_1, \ldots, p_\ell$ restricted to elements $> p_\ell$.

(iii) *Bound pruning:* if the current partial sum plus the minimum possible contribution from remaining elements exceeds the best known sum, prune.

(iv) *Mod-3 filter:* by Corollary 1, restrict candidates at depth $\geq 2$ to primes sharing the residue class of $p_1$ modulo 3 (unless $p_1 = 3$).

*Proof of correctness.* (i) avoids redundant permutations. (ii) ensures every pair in the clique is adjacent. (iii) is a standard branch-and-bound argument. (iv) follows from Theorem 1 and Lemma 1. $\square$

## Algorithm

```
PRIME_PAIR_SETS(B = 10000):
    primes <- sieve_primes(B)

    // Build adjacency lists
    for i <- 0 to |primes| - 1:
        for j <- i + 1 to |primes| - 1:
            if PAIR_OK(primes[i], primes[j]):
                adj[i].append(j)

    best_sum <- infinity
    // Depth-first 5-clique search
    for each p1 in primes:
        if 5 * p1 >= best_sum: break
        S1 <- adj[p1]
        for each p2 in S1:
            if p1 + 4 * p2 >= best_sum: break
            S2 <- {p in S1 : p > p2 and PAIR_OK(p2, p)}
            for each p3 in S2:
                if p1 + p2 + 3 * p3 >= best_sum: break
                S3 <- {p in S2 : p > p3 and PAIR_OK(p3, p)}
                for each p4 in S3:
                    if p1 + p2 + p3 + 2 * p4 >= best_sum: break
                    S4 <- {p in S3 : p > p4 and PAIR_OK(p4, p)}
                    for each p5 in S4:
                        s <- p1 + p2 + p3 + p4 + p5
                        best_sum <- min(best_sum, s)
    return best_sum

PAIR_OK(p, q):
    return IS_PRIME(CONCAT(p, q)) and IS_PRIME(CONCAT(q, p))

CONCAT(a, b):
    d <- number of digits in b
    return a * 10^d + b
```

## Complexity Analysis

**Theorem 4 (Pair Precomputation Cost).** Let $P = \pi(B)$ denote the number of primes below $B$ ($P = 1229$ for $B = 10{,}000$). The adjacency list construction requires $\binom{P}{2} = O(P^2)$ pair tests, each costing $O(\log^2 B)$ with Miller-Rabin. Total: $O(P^2 \log^2 B)$.

*Proof.* There are $\binom{1229}{2} = 754{,}506$ pairs. Each pair test involves two concatenations and two Miller-Rabin primality tests on numbers up to $10^8$. Each Miller-Rabin test with a constant number of witnesses costs $O(\log^2 n) = O(\log^2 B)$. $\square$

**Theorem 5 (Clique Search Cost).** The worst-case search is $O(P^5)$, but the pruning from adjacency intersection and bound checks reduces the practical cost to far below this. The dominant cost is the $O(P^2)$ pair precomputation.

**Space:** $O(P^2)$ for the full adjacency structure, or $O(P \cdot \bar{d})$ using adjacency lists where $\bar{d}$ is the average degree.

## Answer

$$\boxed{26033}$$
