# Problem 325: Stone Game II

## Problem Statement

A game is played with two piles of stones. On each turn, a player removes $k \cdot (\text{smaller pile})$ stones from the larger pile, where $k \geq 1$. The player who empties a pile wins.

A position $(a, b)$ with $a < b$ is *losing* if the current player loses under optimal play.

Define $S(N) = \sum (a + b)$ for all losing positions $(a, b)$ with $0 < a < b \leq N$.

Given: $S(10) = 211$, $S(10^4) = 230312207313$.

Find: $S(10^{16}) \bmod 7^{10}$.

## Mathematical Foundation

**Theorem 1 (Losing positions have quotient 1).** *All losing positions $(a, b)$ with $a < b$ satisfy $a < b < 2a$ (i.e., $\lfloor b/a \rfloor = 1$).*

**Proof.** Let $q = \lfloor b/a \rfloor$ and $r = b \bmod a$. From position $(a, b)$ with $q \geq 2$, the current player can move to $(r, a)$ (by choosing $k = q$) or to $(a, a + r)$ (by choosing $k = q - 1$). Now $(a, a + r)$ is losing iff $(r, a)$ is winning (since $(a, a+r)$ with quotient $\geq 2$ would recurse, but after one move from $(a, a+r)$ we reach $(r, a)$). Hence exactly one of $(r, a)$ and $(a, a+r)$ is losing, and the current player can always move to the losing one. Therefore $(a, b)$ is winning whenever $q \geq 2$. $\square$

**Theorem 2 (Beatty threshold).** *Position $(a, a + r)$ with $0 < r < a$ is losing if and only if $r \leq L(a)$, where*
$$L(a) = \left\lfloor \frac{a}{\varphi} \right\rfloor = \left\lfloor \frac{a(\sqrt{5} - 1)}{2} \right\rfloor.$$

**Proof.** Since all losing positions have quotient 1, the only move from $(a, a+r)$ is to $(r, a)$, so $(a, a+r)$ is losing iff $(r, a)$ is winning. Define the set of losing ratios $\mathcal{L} = \{r/a : (a, a+r) \text{ is losing}\}$. The game tree mirrors the Euclidean algorithm, and the parity of the first index at which the continued fraction $[q_1; q_2, \ldots]$ of $b/a$ has $q_i \geq 2$ determines the winner. Specifically, $(a, a+r)$ is losing iff the continued fraction of $a/r$ has its first entry $\geq 2$ at an odd index.

The losing ratios form the union of intervals:
$$\mathcal{L} = \bigcup_{k=0}^{\infty} \left(\frac{F_{2k}}{F_{2k+1}}, \frac{F_{2k+2}}{F_{2k+3}}\right]$$
where $F_i$ are the Fibonacci numbers. These intervals are contiguous and their union converges to $(0, 1/\varphi)$ where $\varphi = (1+\sqrt{5})/2$. Therefore $L(a) = \lfloor a/\varphi \rfloor = \lfloor a(\sqrt{5}-1)/2 \rfloor$. $\square$

**Lemma 1 (Sum decomposition).** *We have*
$$S(N) = \sum_{a=1}^{N-1} \sum_{r=1}^{R(a)} (2a + r)$$
*where $R(a) = \min(L(a), N - a)$, and this splits at $a^* = \max\{a : L(a) \leq N - a\} \approx N/\varphi$ into:*
- *Range 1 ($a \leq a^*$): $R(a) = L(a)$, contributing Beatty-type sums.*
- *Range 2 ($a > a^*$): $R(a) = N - a$, contributing a closed-form polynomial sum.*

**Proof.** Each losing position is $(a, a+r)$ with $1 \leq r \leq L(a)$ and $a + r \leq N$. The split at $a^*$ separates the regime where $L(a)$ is the binding constraint from the regime where $N - a$ is binding. $\square$

**Theorem 3 (Quadratic floor-sum algorithm).** *The sums*
$$f_0 = \sum_{x=1}^{A} \lfloor x\alpha \rfloor, \quad f_1 = \sum_{x=1}^{A} x\lfloor x\alpha \rfloor, \quad f_2 = \sum_{x=1}^{A} \lfloor x\alpha \rfloor^2$$
*with $\alpha = (\sqrt{5}-1)/2$ approximated by a sufficiently good rational $p/q$ (e.g., $F_{79}/F_{80}$), can be computed simultaneously in $O(\log A)$ arithmetic operations via a Euclidean reciprocal reduction.*

**Proof.** The standard floor-sum identity $\sum_{x=0}^{N-1} \lfloor (ax+b)/m \rfloor = NM - 1 + \text{reciprocal sum}$ where $M = \lfloor (a(N-1)+b)/m \rfloor$ reduces $(a, m)$ in the manner of the Euclidean algorithm. Extending this to track the quadratic and cross terms $f_1, f_2$ yields a system of six simultaneous recurrences that undergo the same Euclidean reduction. Since $\alpha \approx F_{79}/F_{80}$ (a ratio of consecutive Fibonacci numbers), the algorithm terminates in at most 80 steps. $\square$

## Editorial
S(N) = sum of (a+b) for all losing positions (a,b) with 0 < a < b <= N. A position (a, a+r) is losing iff r <= floor(a*(sqrt(5)-1)/2). Uses quadratic floor sum algorithm with Fibonacci rational approximation. We find a* = max a such that floor(a * alpha) <= N - a. We then range 1: a = 1..a_star, R(a) = floor(a * alpha). Finally, range 2: a = a_star+1..N-1, R(a) = N - a.

## Pseudocode

```text
Find a* = max a such that floor(a * alpha) <= N - a
Range 1: a = 1..a_star, R(a) = floor(a * alpha)
Range 2: a = a_star+1..N-1, R(a) = N - a
```

## Complexity Analysis

- **Time:** $O(\log N)$ for the quadratic floor-sum algorithm. The Euclidean recursion depth for $F_{79}/F_{80}$ is at most 80 steps, each involving $O(1)$ arithmetic operations modulo $7^{10}$.
- **Space:** $O(\log N)$ for the recursion stack.

## Answer

$$\boxed{54672965}$$
