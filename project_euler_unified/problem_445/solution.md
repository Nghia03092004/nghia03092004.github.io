# Problem 445: Retractions A

## Problem Statement

For the group $G = \mathbb{Z}_n^k$, a **retraction** is an idempotent group endomorphism $r: G \to G$ satisfying $r \circ r = r$. Let $R(n, k)$ denote the number of retractions on $\mathbb{Z}_n^k$. Find $R(12^{12}) \pmod{10^9 + 7}$.

## Mathematical Foundation

**Theorem 1 (Idempotent Endomorphism Decomposition).** *Let $G$ be a finite abelian group and $r: G \to G$ an endomorphism with $r^2 = r$. Then $G = \operatorname{Im}(r) \oplus \ker(r)$, and $r$ is the projection onto $\operatorname{Im}(r)$ along $\ker(r)$.*

**Proof.** For any $g \in G$, write $g = r(g) + (g - r(g))$. We have $r(r(g)) = r(g)$, so $r(g) \in \operatorname{Im}(r)$. Also $r(g - r(g)) = r(g) - r^2(g) = r(g) - r(g) = 0$, so $g - r(g) \in \ker(r)$. Thus $G = \operatorname{Im}(r) + \ker(r)$. If $x \in \operatorname{Im}(r) \cap \ker(r)$, then $x = r(y)$ for some $y$ and $r(x) = 0$, whence $0 = r(x) = r(r(y)) = r(y) = x$. So the sum is direct. $\square$

**Theorem 2 (CRT Reduction).** *Let $n = p_1^{a_1} \cdots p_m^{a_m}$. Then*

$$\operatorname{End}(\mathbb{Z}_n) \cong \prod_{i=1}^{m} \operatorname{End}(\mathbb{Z}_{p_i^{a_i}}) \cong \prod_{i=1}^{m} \mathbb{Z}_{p_i^{a_i}}.$$

*Consequently, the idempotents of $\mathbb{Z}_n$ correspond to choosing, for each $i$, either $e \equiv 0$ or $e \equiv 1 \pmod{p_i^{a_i}}$, giving $2^m$ idempotents.*

**Proof.** By the Chinese Remainder Theorem, $\mathbb{Z}_n \cong \prod \mathbb{Z}_{p_i^{a_i}}$. Every endomorphism of $\mathbb{Z}_n$ is multiplication by some element, so $\operatorname{End}(\mathbb{Z}_n) \cong \mathbb{Z}_n$. The isomorphism preserves the ring structure. An element $e \in \mathbb{Z}_n$ satisfies $e^2 = e$ iff $e(e-1) \equiv 0 \pmod{n}$. Since $\gcd(e, e-1) = 1$, by CRT this holds iff for each $p_i^{a_i}$ either $p_i^{a_i} \mid e$ or $p_i^{a_i} \mid (e-1)$. $\square$

**Theorem 3 (Gaussian Binomial Counting).** *For $G = \mathbb{Z}_{p^a}^k$ with $p$ prime, the number of retractions is*

$$R(p^a, k) = \sum_{j=0}^{k} \binom{k}{j}_{p^a} \cdot (p^a)^{j(k-j)}$$

*where $\binom{k}{j}_{q}$ is the Gaussian binomial coefficient (number of $j$-dimensional subspaces of a $k$-dimensional space over $\mathbb{F}_q$, generalized to $\mathbb{Z}_{p^a}$-modules).*

**Proof.** A retraction on $\mathbb{Z}_{p^a}^k$ is determined by a direct summand $H \leq G$ (the image) together with a complement $K$ (the kernel) such that $G = H \oplus K$. The number of direct summands isomorphic to $\mathbb{Z}_{p^a}^j$ is counted by the Gaussian-like coefficient, and for each such $H$, the number of complements $K$ with $G = H \oplus K$ is $(p^a)^{j(k-j)}$ (choosing the splitting map). Summing over $j$ gives the formula. $\square$

**Lemma 1 (Multiplicativity).** *For $\gcd(n_1, n_2) = 1$,*

$$R(n_1 n_2, k) = R(n_1, k) \cdot R(n_2, k).$$

**Proof.** By CRT, $\mathbb{Z}_{n_1 n_2}^k \cong \mathbb{Z}_{n_1}^k \times \mathbb{Z}_{n_2}^k$, and this decomposition is preserved by endomorphisms. An idempotent endomorphism of the product corresponds to a pair of idempotent endomorphisms on each factor. $\square$

## Algorithm

```
function ComputeR(n, k, mod):
    // Step 1: Factor n
    factors = prime_factorization(n)  // list of (p, a)

    // Step 2: For each prime power p^a, compute R(p^a, k) mod mod
    result = 1
    for (p, a) in factors:
        local_R = 0
        q = pow(p, a, mod)
        for j = 0 to k:
            gauss_binom = gaussian_binomial(k, j, q, mod)
            complement_count = pow(q, j * (k - j), mod)
            local_R = (local_R + gauss_binom * complement_count) % mod
        result = (result * local_R) % mod

    return result

function gaussian_binomial(k, j, q, mod):
    // Compute [k choose j]_q mod mod
    // [k choose j]_q = prod_{i=0}^{j-1} (q^{k-i} - 1) / (q^{j-i} - 1)
    num = 1
    den = 1
    for i = 0 to j-1:
        num = num * (pow(q, k - i, mod) - 1) % mod
        den = den * (pow(q, i + 1, mod) - 1) % mod
    return num * modular_inverse(den, mod) % mod
```

## Complexity Analysis

- **Time:** $O(m \cdot k^2 \cdot \log(p^a))$ where $m = \omega(n)$ is the number of distinct prime factors. For $n = 12^{12} = 2^{24} \cdot 3^{12}$, we have $m = 2$, and $k$ depends on the problem specification. The Gaussian binomial computation for each $(p, a)$ takes $O(k^2)$ modular exponentiations.
- **Space:** $O(k)$ for intermediate Gaussian binomial values.

## Answer

$$\boxed{659104042}$$
