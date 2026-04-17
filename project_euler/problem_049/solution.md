# Problem 49: Prime Permutations

## Problem Statement

The arithmetic sequence $1487, 4817, 8147$, in which each term increases by $3330$, is unusual in two ways: (i) each of the three terms is prime, and (ii) each of the three 4-digit numbers are permutations of one another. There is one other 4-digit increasing arithmetic sequence with these properties. What 12-digit number do you form by concatenating the three terms?

## Mathematical Development

### Formal Development

**Definition 1 (Digit permutation equivalence).** For non-negative integers $a$ and $b$, write $a \sim b$ if $a$ and $b$ have the same multiset of decimal digits. Define the *canonical signature* $\sigma(n)$ as the string obtained by sorting the digits of $n$ in non-decreasing order.

**Lemma 1 (Characterization of $\sim$).** *$a \sim b$ if and only if $\sigma(a) = \sigma(b)$.*

*Proof.* Two multisets are equal if and only if their sorted representations coincide. The function $\sigma$ produces the sorted representation of the digit multiset. $\square$

**Definition 2 (Arithmetic progression of primes with digit permutation).** A triple $(p_1, p_2, p_3)$ is an *AP-permutation triple* if:
1. $p_1 < p_2 < p_3$ are all prime,
2. $p_2 - p_1 = p_3 - p_2$ (arithmetic progression with common difference $d = p_2 - p_1$),
3. $p_1 \sim p_2 \sim p_3$ (mutual digit permutations).

**Lemma 2 (Bound on common difference).** *For a 4-digit AP-permutation triple $(p_1, p_2, p_3)$, the common difference satisfies $d \leq 4499$.*

*Proof.* All three terms must be 4-digit numbers, so $1000 \leq p_1$ and $p_3 = p_1 + 2d \leq 9999$. Thus $2d \leq 9999 - 1000 = 8999$, giving $d \leq 4499$. $\square$

**Lemma 3 (Grouping strategy).** *If $(p_1, p_2, p_3)$ is an AP-permutation triple, then $\sigma(p_1) = \sigma(p_2) = \sigma(p_3)$. Hence all members of the triple belong to the same $\sigma$-equivalence class.*

*Proof.* Immediate from condition (3) of Definition 2 and Lemma 1. $\square$

**Theorem 1 (Exhaustive enumeration).** *The only 4-digit AP-permutation triples are:*
1. *$(1487, 4817, 8147)$ with common difference $d = 3330$, and*
2. *$(2969, 6299, 9629)$ with common difference $d = 3330$.*

*Proof.* **Step 1.** Generate all 4-digit primes $\mathcal{P}_4 = \{p : 1000 \leq p \leq 9999,\ p \text{ prime}\}$ via the Sieve of Eratosthenes. There are $|\mathcal{P}_4| = \pi(9999) - \pi(999) = 1061$ such primes.

**Step 2.** Partition $\mathcal{P}_4$ into equivalence classes under $\sim$ using the canonical signature $\sigma$. For each class $G$ with $|G| \geq 3$, enumerate all ordered pairs $(p_i, p_j) \in G \times G$ with $p_i < p_j$, and check whether $p_k := 2p_j - p_i$ lies in $G$ (this is the arithmetic progression condition $p_k - p_j = p_j - p_i$).

**Step 3.** The enumeration yields exactly two triples:

- $\sigma(1487) = \sigma(4817) = \sigma(8147) = \text{``1478''}$, with $d = 3330$.
- $\sigma(2969) = \sigma(6299) = \sigma(9629) = \text{``2699''}$, with $d = 3330$.

**Verification of the second triple:**
- $2969$ is prime: $\lfloor\sqrt{2969}\rfloor = 54$; not divisible by any prime up to $54$.
- $6299$ is prime: $\lfloor\sqrt{6299}\rfloor = 79$; not divisible by any prime up to $79$.
- $9629$ is prime: $\lfloor\sqrt{9629}\rfloor = 98$; not divisible by any prime up to $98$.
- Arithmetic: $6299 - 2969 = 3330 = 9629 - 6299$. $\checkmark$
- Digits: $\sigma(2969) = \sigma(6299) = \sigma(9629) = \text{``2699''}$. $\checkmark$

Since the exhaustive search over all $\binom{1061}{2}$ pairs within each group is complete, no other triple exists. $\square$

**Corollary 1.** *The answer (excluding the known triple starting at 1487) is the concatenation $296962999629$.*

## Algorithm

We generate all four-digit primes and group them by their sorted-digit signatures, so each group contains precisely the prime permutations of one another. Inside each group, we inspect ordered pairs $p_i < p_j$ and compute the only possible third term of an arithmetic progression, namely $p_k = 2p_j - p_i$. A set lookup determines whether this third value is present in the same permutation class, and excluding the known sequence beginning with $1487$ leaves the required triple.

## Pseudocode

```text
Algorithm: Prime Permutation Arithmetic Progression
Require: The set of all four-digit primes.
Ensure: The 12-digit concatenation of the nontrivial AP-permutation triple.
1: Partition the four-digit primes into equivalence classes according to their sorted-digit signatures.
2: For each class G with at least three members, examine pairs p_i < p_j in G and compute p_k ← 2p_j - p_i; if p_k also belongs to G and p_i ≠ 1487, return the concatenation of p_i, p_j, and p_k.
3: Return failure.
```

## Complexity Analysis

**Proposition (Time complexity).** *The algorithm runs in $O(N \log \log N + \pi_4 \cdot g_{\max}^2)$ time, where $N = 9999$, $\pi_4 = 1061$ is the number of 4-digit primes, and $g_{\max}$ is the size of the largest equivalence class.*

*Proof.* The sieve of Eratosthenes on $[0, N]$ costs $O(N \log \log N)$. Computing $\sigma(p)$ for each 4-digit prime costs $O(\pi_4 \cdot d \log d)$ where $d = 4$ (sorting 4 digits), which is $O(\pi_4)$. For each equivalence class of size $g$, the nested loop examines $\binom{g}{2}$ pairs with $O(1)$ set lookup each. The total over all classes is $\sum_{G} \binom{|G|}{2} \leq \pi_4 \cdot g_{\max}$. In practice, $g_{\max} \leq 15$, so this is $O(\pi_4)$. $\square$

**Proposition (Space complexity).** *$O(\pi_4)$ for storing the grouped primes and the set lookup structures.*

## Answer

$$\boxed{296962999629}$$
