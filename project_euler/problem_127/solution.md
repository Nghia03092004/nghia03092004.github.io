# Problem 127: abc-hits

## Problem Statement

The radical of $n$, $\text{rad}(n)$, is the product of the distinct prime factors of $n$. A triple $(a, b, c)$ is an *abc-hit* if:
1. $\gcd(a, b) = 1$
2. $a < b$
3. $a + b = c$
4. $\text{rad}(abc) < c$

Find $\sum c$ for all abc-hits with $c < 120000$.

## Mathematical Foundation

**Theorem 1 (Pairwise coprimality).** *If $\gcd(a, b) = 1$ and $a + b = c$, then $a$, $b$, $c$ are pairwise coprime.*

**Proof.** Suppose a prime $p$ divides both $a$ and $c = a + b$. Then $p \mid (c - a) = b$, contradicting $\gcd(a, b) = 1$. By symmetry (swapping $a$ and $b$), $\gcd(b, c) = 1$ as well. $\square$

**Theorem 2 (Radical factorization for coprime triples).** *If $a$, $b$, $c$ are pairwise coprime, then $\text{rad}(abc) = \text{rad}(a) \cdot \text{rad}(b) \cdot \text{rad}(c)$.*

**Proof.** Since $a$, $b$, $c$ share no common prime factor, the set of primes dividing $abc$ is the disjoint union of primes dividing $a$, $b$, and $c$ respectively. Therefore $\prod_{p \mid abc} p = \prod_{p \mid a} p \cdot \prod_{p \mid b} p \cdot \prod_{p \mid c} p$. $\square$

**Lemma 1 (Reformulated condition).** *The abc-hit condition $\text{rad}(abc) < c$ is equivalent to $\text{rad}(a) \cdot \text{rad}(b) < c / \text{rad}(c)$.*

**Proof.** By Theorem 2, $\text{rad}(abc) = \text{rad}(a) \cdot \text{rad}(b) \cdot \text{rad}(c)$. The inequality $\text{rad}(a) \cdot \text{rad}(b) \cdot \text{rad}(c) < c$ divides both sides by $\text{rad}(c) > 0$. $\square$

**Lemma 2 (Coprimality via radicals).** *For positive integers $a$ and $b$, $\gcd(a, b) = 1$ if and only if $\gcd(\text{rad}(a), \text{rad}(b)) = 1$.*

**Proof.** $(\Rightarrow)$: If a prime $p$ divides both $\text{rad}(a)$ and $\text{rad}(b)$, then $p \mid a$ and $p \mid b$, contradicting $\gcd(a,b) = 1$. $(\Leftarrow)$: If $p \mid \gcd(a,b)$, then $p \mid \text{rad}(a)$ and $p \mid \text{rad}(b)$, contradicting $\gcd(\text{rad}(a), \text{rad}(b)) = 1$. $\square$

**Theorem 3 (Search strategy).** *For each $c$, sorting candidates $a$ by $\text{rad}(a)$ in ascending order allows early termination: once $\text{rad}(a) \geq c / \text{rad}(c)$, no further $a$ can satisfy the condition (since $\text{rad}(b) \geq 1$).*

**Proof.** If $\text{rad}(a) \geq c / \text{rad}(c)$, then $\text{rad}(a) \cdot \text{rad}(b) \geq \text{rad}(a) \geq c / \text{rad}(c)$ for all $b$. Thus the condition $\text{rad}(a) \cdot \text{rad}(b) < c / \text{rad}(c)$ fails, and all subsequent $a$ (with equal or larger radical) also fail. $\square$

## Algorithm

```
function abc_hits(N):
    # Step 1: Sieve for radicals
    rad[1..N-1] = 1
    for p = 2 to N-1:
        if rad[p] == 1:           # p is prime
            for m = p, 2p, ... < N:
                rad[m] *= p

    # Step 2: Sort indices by radical
    sorted_by_rad = sort(1..N-1, key=rad)

    # Step 3: Find abc-hits
    total = 0
    for c = 3 to N-1:
        threshold = c / rad[c]
        if threshold <= 1: continue     # rad(c) >= c means no abc-hit
        for a in sorted_by_rad:
            if rad[a] >= threshold: break
            if a >= c: break
            b = c - a
            if b <= a: continue         # need a < b
            if rad[a] * rad[b] >= threshold: continue
            if gcd(rad[a], rad[b]) != 1: continue
            total += c
    return total
```

## Complexity Analysis

- **Time:** $O(N \log \log N)$ for the sieve. $O(N \log N)$ for sorting. The main search loop: for each $c$, the inner loop terminates early due to the radical threshold. Empirically, the total work across all $c$ is $O(N \log N)$.
- **Space:** $O(N)$ for the radical array and sorted index.

## Answer

$$\boxed{18407904}$$
