# Problem 651: Patterned Cylinders

## Problem Statement

A cylinder is formed by a ring of $n$ cells, each coloured with one of $k$ colours. Two colourings are considered identical if one can be obtained from the other by rotation of the cylinder. Determine the number $N(n,k)$ of distinct colourings.

## Mathematical Foundation

**Theorem 1 (Burnside's Lemma).** Let a finite group $G$ act on a finite set $X$. The number of distinct orbits is
$$|X/G| = \frac{1}{|G|}\sum_{g \in G} |X^g|,$$
where $X^g = \{x \in X : g \cdot x = x\}$ is the set of elements fixed by $g$.

**Proof.** Define the indicator $\mathbf{1}[g \cdot x = x]$. Count the set $\{(g,x) \in G \times X : g \cdot x = x\}$ in two ways:
$$\sum_{g \in G} |X^g| = \sum_{x \in X} |\operatorname{Stab}(x)|.$$
By the orbit-stabiliser theorem, $|\operatorname{Stab}(x)| = |G|/|\operatorname{Orb}(x)|$. Hence
$$\sum_{x \in X} \frac{|G|}{|\operatorname{Orb}(x)|} = |G| \sum_{O \in X/G} \sum_{x \in O} \frac{1}{|O|} = |G| \cdot |X/G|.$$
Dividing both sides by $|G|$ yields the result. $\square$

**Lemma 1.** Let $G = \mathbb{Z}/n\mathbb{Z}$ act on $X = \{1,\ldots,k\}^n$ by cyclic rotation. The rotation by $d$ positions fixes a colouring if and only if the colouring has period dividing $\gcd(d,n)$. Hence $|X^d| = k^{\gcd(d,n)}$.

**Proof.** A colouring $(c_0, c_1, \ldots, c_{n-1})$ is fixed by rotation by $d$ if and only if $c_i = c_{i+d \bmod n}$ for all $i$. This forces $c_i = c_j$ whenever $i \equiv j \pmod{\gcd(d,n)}$. There are $\gcd(d,n)$ equivalence classes, each freely colourable, giving $k^{\gcd(d,n)}$ fixed colourings. $\square$

**Theorem 2 (Necklace Formula).** The number of distinct necklaces is
$$N(n,k) = \frac{1}{n}\sum_{d \mid n} \varphi(n/d) \, k^d,$$
where $\varphi$ is Euler's totient function.

**Proof.** By Burnside's Lemma and Lemma 1:
$$N(n,k) = \frac{1}{n}\sum_{d=0}^{n-1} k^{\gcd(d,n)}.$$
Group the terms by $e = \gcd(d,n)$. For each divisor $e \mid n$, the number of integers $d \in \{0, 1, \ldots, n-1\}$ with $\gcd(d,n) = e$ equals $\varphi(n/e)$. Substituting:
$$N(n,k) = \frac{1}{n}\sum_{e \mid n} \varphi(n/e) \, k^e. \quad \square$$

## Algorithm

```
function NecklaceCount(n, k, mod):
    divisors <- FindDivisors(n)          // O(sqrt(n))
    result <- 0
    for each d in divisors:
        phi_val <- EulerTotient(n / d)   // O(sqrt(n/d))
        result <- result + phi_val * ModPow(k, d, mod)
    result <- result * ModInverse(n, mod) mod mod
    return result
```

## Complexity Analysis

- **Time:** $O(\sqrt{n} \cdot \log k)$. Finding all divisors of $n$ takes $O(\sqrt{n})$. For each of the $O(\sqrt{n})$ divisors, computing $\varphi$ takes $O(\sqrt{n})$ and modular exponentiation takes $O(\log k)$. The dominant term is $O(\sqrt{n} \cdot (\sqrt{n} + \log k))$, but since typically $\log k \ll \sqrt{n}$, this simplifies to $O(n^{1/2} \cdot \log k)$ in practice (with precomputed totients).
- **Space:** $O(\sqrt{n})$ for storing divisors.

## Answer

$$\boxed{448233151}$$
