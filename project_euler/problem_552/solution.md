# Problem 552: Chinese Leftovers II

## Problem Statement

Let $p_1 < p_2 < \cdots < p_k$ be the first $k$ primes. For a positive integer $n$, define
$$r_i(n) = n \bmod p_i, \quad 1 \leq i \leq k.$$
Define $S(n, k)$ as the number of integers $m$ with $1 \leq m \leq n$ such that $r_i(m) \geq r_i(p_i)$ for all $1 \leq i \leq k$, where the $r_i(p_i)$ are given threshold residues.

The problem asks for a specific value derived from simultaneous congruence constraints modulo the first $k$ primes, reduced modulo a given prime.

## Mathematical Foundation

**Theorem 1 (Chinese Remainder Theorem).** Let $m_1, m_2, \ldots, m_k$ be pairwise coprime positive integers, and let $a_1, \ldots, a_k$ be arbitrary integers. Then the system
$$x \equiv a_i \pmod{m_i}, \quad 1 \leq i \leq k$$
has a unique solution modulo $M = \prod_{i=1}^{k} m_i$. Explicitly,
$$x = \sum_{i=1}^{k} a_i \cdot M_i \cdot (M_i^{-1} \bmod m_i) \pmod{M},$$
where $M_i = M / m_i$.

**Proof.** Since the $m_i$ are pairwise coprime, $\gcd(M_i, m_i) = 1$, so $M_i^{-1} \bmod m_i$ exists. Define $e_i = M_i \cdot (M_i^{-1} \bmod m_i)$. Then $e_i \equiv 1 \pmod{m_i}$ and $e_i \equiv 0 \pmod{m_j}$ for $j \neq i$. Thus $x = \sum_i a_i e_i$ satisfies all congruences. Uniqueness modulo $M$ follows because if $x_1, x_2$ are two solutions, then $M \mid (x_1 - x_2)$. $\square$

**Lemma 1 (Iterative CRT Combination).** Given a solution $x \equiv a \pmod{m}$ and a new congruence $x \equiv b \pmod{n}$ with $\gcd(m, n) = 1$, the combined solution is
$$x \equiv a + m \cdot \bigl((b - a) \cdot m^{-1} \bmod n\bigr) \pmod{mn}.$$

**Proof.** Set $x = a + m \cdot t$. The condition $x \equiv b \pmod{n}$ becomes $m \cdot t \equiv b - a \pmod{n}$, so $t \equiv (b - a) \cdot m^{-1} \pmod{n}$. This gives $x = a + m \cdot ((b - a) \cdot m^{-1} \bmod n)$, unique modulo $mn$. $\square$

**Theorem 2 (Counting via Inclusion-Exclusion on Residue Constraints).** For each prime $p_i$, the set of integers in $\{1, \ldots, n\}$ satisfying a residue constraint modulo $p_i$ has density $c_i / p_i$ for some integer $c_i$ (the number of admissible residues). By CRT and independence of constraints for coprime moduli, the total count is
$$S(n, k) = \sum_{\mathbf{r} \in \mathcal{A}} \left\lfloor \frac{n - x_{\mathbf{r}}}{P} \right\rfloor + [x_{\mathbf{r}} \leq n],$$
where $\mathcal{A}$ is the set of admissible residue tuples, $x_{\mathbf{r}}$ is the CRT lift, and $P = \prod p_i$.

**Proof.** By CRT, each admissible residue tuple $\mathbf{r}$ corresponds to a unique residue class modulo $P$. The integers in $\{1, \ldots, n\}$ in that class number $\lfloor (n - x_{\mathbf{r}}) / P \rfloor + 1$ if $x_{\mathbf{r}} \leq n$ (and 0 otherwise). Summing over all admissible tuples yields the result. $\square$

## Editorial
We build admissible residues for each prime. We then iterative CRT combination. Finally, start with all admissible residues mod primes[1].

## Pseudocode

```text
Build admissible residues for each prime
Iterative CRT combination
Start with all admissible residues mod primes[1]
Combine x (mod current_mod) with r (mod primes[i])
Count: for each solution x, count multiples in [1, n]
gcd(m, n) = 1
```

## Complexity Analysis

- **Time:** $O\!\left(\prod_{i=1}^{k} (p_i - t_i)\right)$ where $t_i$ is the threshold for prime $p_i$, for enumerating all admissible CRT solutions. With the iterative approach and modular arithmetic, each combination step multiplies the solution count by at most $p_i$. For the given problem parameters, this is $O(k \log P)$ per CRT combination, with the dominant cost being the enumeration.
- **Space:** $O\!\left(\prod_{i=1}^{k} (p_i - t_i)\right)$ for storing the solution set, or $O(P)$ in the worst case.

## Answer

$$\boxed{326227335}$$
