# Problem 228: Minkowski Sums

## Problem Statement

Let $S_n$ be the regular $n$-gon whose vertices are

$$
(\cos((2k-1)\pi/n),\ \sin((2k-1)\pi/n)),
\qquad
k=1,\dots,n.
$$

How many sides does the Minkowski sum

$$
S_{1864}+S_{1865}+\cdots+S_{1909}
$$

have?

## Mathematical Development

For a convex polygon, each side corresponds to an outward normal direction. The Minkowski sum adds support functions, so its sides are obtained by taking the union of all outward normal directions that appear in the summands.

With the given orientation, the outward normals of $S_n$ are the directions

$$
\frac{2\pi k}{n},
\qquad
k=0,1,\dots,n-1.
$$

Two such directions coincide exactly when the fractions $k/n$ reduce to the same rational number. So a reduced fraction

$$
\frac{p}{q},
\qquad
0 \le p < q,
\qquad
\gcd(p,q)=1,
$$

appears in the union if and only if some polygon $S_n$ in the range has a side normal in that direction, which is equivalent to

$$
q \mid n
$$

for at least one $n \in [1864,1909]$.

Therefore each denominator $q$ contributes all reduced numerators modulo $q$, namely $\varphi(q)$ directions, provided the interval contains a multiple of $q$. The denominator $q=1$ contributes the single direction $0$.

So the answer is

$$
1+\sum_{\substack{2 \le q \le 1909\\ \exists n \in [1864,1909],\ q\mid n}}\varphi(q).
$$

The divisibility condition is easy to test:

$$
\left\lfloor \frac{1909}{q} \right\rfloor
>
\left\lfloor \frac{1863}{q} \right\rfloor.
$$

## Editorial

This problem looks geometric, but the geometry disappears almost immediately. The Minkowski sum only cares about which edge normals occur, and a regular $n$-gon contributes the $n$ equally spaced directions $2\pi k/n$.

So the question becomes arithmetic: how many reduced fractions $p/q$ occur with a denominator $q$ that divides at least one integer between $1864$ and $1909$? Once $q$ is fixed, there are exactly $\varphi(q)$ such fractions. That reduces the whole problem to a totient sieve followed by a short scan over the possible denominators.

## Pseudocode

```text
Compute Euler's totient function phi(q) for every q up to 1909.

Start the answer at 1 for the denominator q = 1.

For q from 2 to 1909:
    If the interval [1864, 1909] contains a multiple of q:
        add phi(q) to the answer

Print the result.
```

## Complexity Analysis

- **Time:** $O(1909 \log \log 1909)$ for the totient sieve, plus a linear scan.
- **Space:** $O(1909)$.

## Answer

$$\boxed{86226}$$
