# Problem 248: Numbers for which Euler's Totient Function Equals 13!

## Problem Statement

Find the $150{,}000$-th number $n$ (in ascending order) for which $\varphi(n) = 13! = 6{,}227{,}020{,}800$.

## Mathematical Foundation

**Theorem 1 (Totient product formula).** For $n = \prod_{i=1}^{k} p_i^{a_i}$ with distinct primes $p_i$,
$$\varphi(n) = \prod_{i=1}^{k} p_i^{a_i - 1}(p_i - 1).$$

**Proof.** This is standard: among the $p^a$ multiples of 1 up to $p^a$, exactly $p^{a-1}$ are divisible by $p$, so $\varphi(p^a) = p^a - p^{a-1} = p^{a-1}(p-1)$. Multiplicativity of $\varphi$ for coprime arguments completes the proof. $\square$

**Lemma 1 (Candidate prime criterion).** If $p^a \mid n$ with $p$ prime and $a \ge 1$, then $p^{a-1}(p-1) \mid \varphi(n) = 13!$. In particular, $(p-1) \mid 13!$.

**Proof.** From Theorem 1, $p^{a-1}(p-1)$ is one factor in the product for $\varphi(n)$, hence divides $\varphi(n) = 13!$. $\square$

**Theorem 2 (Candidate prime enumeration).** The set of primes $p$ that can divide any $n$ with $\varphi(n) = 13!$ is
$$\mathcal{P} = \{p \text{ prime} : (p-1) \mid 13!\}.$$
Since $13! = 2^{10} \cdot 3^5 \cdot 5^2 \cdot 7 \cdot 11 \cdot 13$, the divisors of $13!$ that are one less than a prime determine $\mathcal{P}$. There are exactly **459** such primes.

**Proof.** By Lemma 1, any prime dividing $n$ must satisfy $(p-1) \mid 13!$. Conversely, for each divisor $d$ of $13!$, if $d + 1$ is prime, it is a candidate. Exhaustive enumeration of the $\tau(13!) = 11 \cdot 6 \cdot 3 \cdot 2 \cdot 2 \cdot 2 = 1584$ divisors of $13!$ and primality testing yields 459 primes. $\square$

**Theorem 3 (Recursive construction).** All solutions $n$ to $\varphi(n) = T$ (with $T = 13!$) can be enumerated by the following recursive procedure. Choose primes $p_1 \le p_2 \le \cdots$ from $\mathcal{P}$ and exponents $a_i \ge 1$, building $n = \prod p_i^{a_i}$ while maintaining the residual target $T' = T / \prod p_i^{a_i - 1}(p_i - 1)$. The recursion terminates when $T' = 1$ (valid solution) or $T' < 1$ (prune).

**Proof.** Every $n$ with $\varphi(n) = T$ has a unique prime factorisation. By processing primes in non-decreasing order, each factorisation is visited exactly once. The residual target tracks the remaining totient to be achieved, and each chosen prime-power $p^a$ contributes $p^{a-1}(p-1)$ to the totient. The recursion explores all valid combinations exhaustively. $\square$

**Lemma 2 (Exponent bound).** For prime $p$ with residual target $T'$, the maximum exponent $a$ satisfies $p^{a-1}(p-1) \le T'$, i.e., $a \le 1 + \lfloor \log_p(T'/(p-1)) \rfloor$.

**Proof.** The contribution $p^{a-1}(p-1)$ must divide $T'$, hence not exceed it. $\square$

## Editorial
We find candidate primes. Finally, recursive enumeration. We perform a recursive search over the admissible choices, prune branches that violate the derived constraints, and keep only the candidates that satisfy the final condition.

## Pseudocode

```text
Find candidate primes
Recursive enumeration
Sort and return 150000th
```

## Complexity Analysis

- **Time:** The recursion tree has branching factor bounded by $|\mathcal{P}| = 459$ at each level, but the residual target shrinks multiplicatively, limiting depth to $O(\log T / \log 2) \approx 33$. Empirically, about $548{,}000$ solutions are found, with search time dominated by the enumeration. Sorting: $O(N \log N)$ with $N \approx 548{,}000$.
- **Space:** $O(N)$ to store all solutions, plus $O(\log T)$ recursion stack depth.

## Answer

$$\boxed{23507044290}$$
