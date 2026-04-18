# Problem 273: Sum of Squares

## Problem Statement

Consider equations of the form $a^2 + b^2 = n$ with $0 \le a \le b$ and $a, b, n$ integers. Let $\mathcal{P} = \{p \text{ prime} : p \equiv 1 \pmod{4},\; p < 150\}$. For each squarefree $N$ that is a product of a non-empty subset of $\mathcal{P}$, let $S(N)$ be the sum of all values $a$ across representations $a^2 + b^2 = N$ with $0 \le a \le b$. Compute $\sum_N S(N)$.

## Mathematical Foundation

**Theorem 1 (Fermat's Two-Square Theorem).** *A prime $p$ is representable as a sum of two squares if and only if $p = 2$ or $p \equiv 1 \pmod{4}$.*

**Proof.** ($\Longleftarrow$) If $p \equiv 1 \pmod{4}$, then $-1$ is a quadratic residue mod $p$, so $p \mid (m^2 + 1)$ for some $m$. In the Gaussian integers $\mathbb{Z}[i]$, $p \mid (m+i)(m-i)$ but $p \nmid (m \pm i)$, so $p$ is not a Gaussian prime. Hence $p = \pi \bar{\pi}$ with $|\pi|^2 = a^2 + b^2 = p$. ($\Longrightarrow$) If $a^2 + b^2 = p$ with $p$ odd, then $a^2 \equiv -b^2 \pmod{p}$, so $(ab^{-1})^2 \equiv -1 \pmod{p}$, forcing $p \equiv 1 \pmod{4}$. $\square$

**Theorem 2 (Gaussian Integer Representation).** *Let $N = p_1 p_2 \cdots p_k$ be a squarefree product of primes $p_j \equiv 1 \pmod{4}$, with Gaussian factorisation $p_j = \pi_j \bar{\pi}_j$. The representations $a^2 + b^2 = N$ with $a, b \ge 0$ are in bijection with*

$$\alpha = u \prod_{j=1}^{k} \pi_j^{\epsilon_j} \bar{\pi}_j^{1 - \epsilon_j}, \quad \epsilon_j \in \{0, 1\},\; u \in \{1, i, -1, -i\}$$

*where $\alpha = a + bi$ and we identify $(a, b) = (|\operatorname{Re}(\alpha)|, |\operatorname{Im}(\alpha)|)$.*

**Proof.** $\mathbb{Z}[i]$ is a PID, so $N = \prod_j \pi_j \bar{\pi}_j$ is the unique factorisation (up to units). Any Gaussian integer $\alpha$ with $|\alpha|^2 = N$ must be a product of exactly one element from each pair $\{\pi_j, \bar{\pi}_j\}$, times a unit. There are $2^k$ choices for the $\epsilon_j$ and 4 units, giving $4 \cdot 2^k$ Gaussian integers. Identifying $(a, b)$ with $(|a|, |b|)$ and $a \le b$ reduces the count by a factor of 8 (generically), yielding $2^{k-1}$ distinct representations when $k \ge 1$ (with possible boundary cases when $a = 0$ or $a = b$). $\square$

**Lemma 1 (Brahmagupta--Fibonacci Identity).** *$(a^2 + b^2)(c^2 + d^2) = (ac - bd)^2 + (ad + bc)^2 = (ac + bd)^2 + (ad - bc)^2$.*

**Proof.** Direct expansion. Alternatively, this is the multiplicativity of the Gaussian norm: $|(\alpha)(\beta)|^2 = |\alpha|^2 \cdot |\beta|^2$ where $\alpha = a + bi$, $\beta = c + di$. $\square$

**Theorem 3 (Incremental Construction).** *If $N_1$ has representations $\{(a_i, b_i)\}$ and $p = c^2 + d^2$ is a new prime, then $N_1 \cdot p$ has representations obtained by applying the Brahmagupta--Fibonacci identity to each $(a_i, b_i)$ with $(c, d)$:*

$$(a_i, b_i) \otimes (c, d) \;\longmapsto\; (|a_i c - b_i d|,\; a_i d + b_i c) \text{ and } (|a_i c + b_i d|,\; |a_i d - b_i c|)$$

*Each existing representation produces exactly two new ones (possibly after swapping coordinates to ensure $a \le b$).*

**Proof.** This follows from Lemma 1 and the Gaussian integer product: $(a_i + b_i \, i)(c + d\, i)$ and $(a_i + b_i \, i)(c - d\, i)$ yield the two new representations. $\square$

## Editorial
For squarefree N that is a product of primes p = 1 mod 4 with p < 150, let S(N) = sum of all a values where a^2 + b^2 = N, 0 <= a <= b. Find sum of S(N) over all such N. Primes = 1 mod 4 less than 150: 5,13,17,29,37,41,53,61,73,89,97,101,109,113,137,149 (16 primes) Uses Gaussian integers and Brahmagupta-Fibonacci identity. We 16 primes. We then iterate over each prime p, find (c, d) with c^2 + d^2 = p, c ≤ d. Finally, we run a depth-first search over the subsets while maintaining the current list of $(a,b)$ representations.

## Pseudocode

```text
16 primes
For each prime p, find (c, d) with c^2 + d^2 = p, c ≤ d
DFS over subsets, maintaining list of (a, b) representations
reps = list of (a, b) with a ≤ b for current product
if reps is not empty
for i from index to 15
Two new representations
Bootstrap: each single prime p = c^2 + d^2 gives one representation
```

## Complexity Analysis

- **Time:** Each subset of size $k$ produces $2^{k-1}$ representations. The total work over all $2^{16} - 1$ non-empty subsets is $\sum_{k=1}^{16} \binom{16}{k} \cdot 2^{k-1} = \frac{1}{2}(3^{16} - 1) \approx 2.15 \times 10^7$. Each representation requires $O(1)$ arithmetic. Total: $O(3^{16})$.
- **Space:** The DFS stack depth is at most 16. At each level, the representation list has at most $2^{15}$ entries. Space: $O(2^{16})$.

## Answer

$$\boxed{2032447591196869022}$$
