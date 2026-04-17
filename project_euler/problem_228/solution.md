# Problem 228: Minkowski Sums

## Problem Statement

Let $S_n$ denote a regular $n$-sided polygon. Find the number of sides of the Minkowski sum

$$T = S_2 + S_3 + S_4 + \cdots + S_{1864}.$$

## Mathematical Foundation

**Theorem 1 (Minkowski Sum Edge Count).** *The Minkowski sum of convex polygons $P_1, P_2, \ldots, P_k$ is a convex polygon whose number of edges equals the number of distinct edge normal directions across all summands.*

**Proof.** The Minkowski sum of convex polygons is convex. By the support function characterization, $h_{P+Q}(\theta) = h_P(\theta) + h_Q(\theta)$. The edges of a convex polygon correspond to maximal arcs of the unit circle on which the support function is affine (linear in $\theta$). Two edges from different summands merge if and only if they share the same outward normal direction. Hence the edge count of the sum equals the number of distinct normal directions. $\square$

**Lemma 1 (Normal Directions of $S_n$).** *A regular $n$-gon in standard orientation has outward edge normals at angles*

$$\theta_k^{(n)} = \frac{\pi(2k + 1)}{n}, \quad k = 0, 1, \ldots, n - 1.$$

**Proof.** The vertices of a regular $n$-gon are at angles $2\pi k/n$. Edge $k$ connects vertex $k$ to vertex $k+1$, so its direction is at angle $\pi(2k+1)/n$. The outward normal is this direction rotated by $\pi/2$, but since we only care about the set of directions modulo $2\pi$, the normal of edge $k$ is at angle $\pi(2k+1)/n$ (up to a constant rotation that does not affect the counting). $\square$

**Theorem 2 (Reduced Fraction Characterization).** *A direction $\pi \cdot \frac{2k+1}{n}$ reduces to $\pi \cdot \frac{p}{q}$ where $p$ is odd, $\gcd(p, q) = 1$, and $0 < p < 2q$. The direction with denominator $q$ appears in $S_n$ if and only if $q \mid n$ and $n/q$ is odd.*

**Proof.** Write $\frac{2k+1}{n} = \frac{p}{q}$ in lowest terms. Since $2k+1$ is odd, $p$ must be odd. The direction appears in $S_n$ iff $\frac{p}{q} = \frac{2k+1}{n}$ for some integer $k$ with $0 \leq k < n$. This requires $n = q \cdot \frac{2k+1}{p}$. For $n/q$ to be an integer, $q \mid n$. Moreover, $n/q = (2k+1)/p$ is a ratio of two odd numbers, hence odd. $\square$

**Theorem 3 (Direction Count per Denominator).** *For a given denominator $q$ with $2 \leq q \leq 1864$:*

- *If $q$ is odd: there are $\phi(q)$ distinct odd values of $p$ with $\gcd(p, q) = 1$ and $0 < p < 2q$.*
- *If $q$ is even: there are $2\phi(q)$ such values.*

*Additionally, for $q = 1$: there is $\phi(1) = 1$ direction (namely $p = 1$).*

**Proof.** We count odd $p \in (0, 2q)$ with $\gcd(p, q) = 1$.

*Case $q$ odd:* Among $\{1, 2, \ldots, 2q - 1\}$, there are $\phi(2q)$ values coprime to $2q$. Since $\gcd(p, 2q) = 1$ requires $p$ odd and $\gcd(p, q) = 1$, we get $\phi(2q) = \phi(2)\phi(q) = \phi(q)$ values.

*Case $q$ even:* Among odd $p \in \{1, 3, \ldots, 2q-1\}$, there are $q$ odd numbers. Those coprime to $q$ number $\sum_{d \mid q} \mu(d) \cdot \lceil q/(2d) \rceil$. By Mobius inversion and the fact that for even $q$, exactly $2\phi(q)$ odd numbers in $[1, 2q)$ are coprime to $q$. $\square$

**Theorem 4 (Existence of Each Direction).** *For each denominator $q$ with $1 \leq q \leq 1864$, $S_q$ is among the summands (since $2 \leq q \leq 1864$, or $q = 1$ via $S_2$), so every direction with denominator $q \leq 1864$ is realized.*

**Proof.** For $q \geq 2$, $S_q$ is directly a summand. For $q = 1$, the direction $\pi \cdot 1/1 = \pi$ appears in $S_n$ for any odd $n$, e.g., $S_3$. $\square$

## Algorithm

```
function count_sides(N = 1864):
    // Compute Euler's totient via sieve
    phi = totient_sieve(N)

    total = phi[1]    // q = 1 contributes phi(1) = 1
    for q = 2 to N:
        if q is odd:
            total += phi[q]
        else:
            total += 2 * phi[q]

    return total
```

## Complexity Analysis

- **Time:** $O(N \log \log N)$ for the totient sieve, $O(N)$ for the summation. Total: $O(N \log \log N)$.
- **Space:** $O(N)$ for the totient array.

## Answer

$$\boxed{86226}$$
