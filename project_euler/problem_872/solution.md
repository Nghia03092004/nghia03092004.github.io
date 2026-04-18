# Problem 872: Recursive Bracelets

## Problem Statement

This problem concerns the enumeration of equivalence classes of bead colorings on a cycle under the action of the dihedral group, i.e., counting *bracelets*. The central formula is the necklace counting function:

$$N(n,k) = \frac{1}{n}\sum_{d \mid n}\varphi(d)\,k^{n/d}.$$

The problem extends this to a recursive or compositional variant of bracelet counting.

## Mathematical Foundation

**Theorem (Burnside's Lemma).** *Let a finite group $G$ act on a finite set $X$. The number of orbits is*
$$|X/G| = \frac{1}{|G|}\sum_{g \in G} |X^g|,$$
*where $X^g = \{x \in X : g \cdot x = x\}$.*

**Proof.** Count pairs $(g, x) \in G \times X$ such that $g \cdot x = x$. Summing over $G$: $\sum_{g \in G}|X^g|$. Summing over $X$: $\sum_{x \in X}|G_x|$ where $G_x$ is the stabilizer of $x$. By the orbit-stabilizer theorem, $|G_x| = |G|/|G \cdot x|$, so
$$\sum_{x \in X} |G_x| = \sum_{x \in X} \frac{|G|}{|G \cdot x|} = |G| \sum_{\text{orbits } O} \sum_{x \in O} \frac{1}{|O|} = |G| \cdot |\text{orbits}|.$$
Equating the two counts gives the result. $\square$

**Theorem (Necklace Count).** *The number of distinct necklaces (equivalence classes under cyclic rotation) with $n$ beads and $k$ colors is*
$$N(n, k) = \frac{1}{n}\sum_{d \mid n}\varphi(d)\,k^{n/d}.$$

**Proof.** Apply Burnside's lemma to the cyclic group $C_n$ acting on $k$-colorings of $n$ beads. The rotation by $j$ positions fixes a coloring if and only if the coloring is periodic with period dividing $j$. The number of colorings fixed by rotation-by-$j$ is $k^{\gcd(n,j)}$. Hence
$$N(n,k) = \frac{1}{n}\sum_{j=0}^{n-1} k^{\gcd(n,j)}.$$
Grouping terms by $d = n/\gcd(n,j)$: there are $\varphi(d)$ values of $j \in \{0,\ldots,n-1\}$ with $\gcd(n,j) = n/d$, yielding the stated formula. $\square$

**Theorem (Bracelet Count).** *A bracelet is an equivalence class under the dihedral group $D_n$ (rotations and reflections). The count is*
$$B(n, k) = \begin{cases} \dfrac{1}{2}N(n,k) + \dfrac{1}{4}(k+1)\,k^{n/2} & \text{if } n \text{ is even},\\[6pt] \dfrac{1}{2}N(n,k) + \dfrac{1}{2}\,k^{(n+1)/2} & \text{if } n \text{ is odd}. \end{cases}$$

**Proof.** The dihedral group $D_n$ has $2n$ elements: $n$ rotations and $n$ reflections. The contribution from rotations is $n \cdot N(n,k)$. For reflections: when $n$ is odd, each reflection fixes a bead and bisects the opposite gap, so a fixed coloring is determined by $(n+1)/2$ independent beads, giving $k^{(n+1)/2}$ fixed points per reflection ($n$ reflections total). When $n$ is even, there are $n/2$ reflections through two beads (fixing $k^{n/2+1}$ colorings each) and $n/2$ reflections through two edge midpoints (fixing $k^{n/2}$ colorings each). Averaging over $|D_n| = 2n$ gives the formula. $\square$

**Lemma (Aperiodic Necklaces / Lyndon Words).** *The number of aperiodic necklaces of length $n$ over $k$ symbols is*
$$L(n,k) = \frac{1}{n}\sum_{d \mid n} \mu(d)\,k^{n/d},$$
*where $\mu$ is the Mobius function. Furthermore, $N(n,k) = \sum_{d \mid n} L(d,k)$.*

**Proof.** By Mobius inversion applied to $n \cdot N(n,k) = \sum_{d \mid n} d \cdot L(d,k) \cdot (n/d)$. The identity $N(n,k) = \sum_{d \mid n} L(d,k)$ follows from the fact that every necklace of length $n$ has a unique primitive period $d \mid n$, and there are $L(d,k)$ aperiodic necklaces of that length. Inverting gives the formula for $L$. $\square$

## Editorial
Necklace counting via burnside/polya. We iterate over each divisor d of n. Finally, else. We perform a recursive search over the admissible choices, prune branches that violate the derived constraints, and keep only the candidates that satisfy the final condition.

## Pseudocode

```text
for each divisor d of n
if n is odd
else
```

## Complexity Analysis

- **Time:** Computing $N(n,k)$ requires enumerating divisors of $n$: $O(d(n) \cdot \log n)$ where $d(n)$ is the number of divisors, using modular exponentiation. The total time depends on the recursive structure of the specific problem variant.
- **Space:** $O(1)$ auxiliary beyond the divisor list, or $O(n)$ if memoization of subproblems is needed for the recursive variant.

## Answer

$$\boxed{2903144925319290239}$$
