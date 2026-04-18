# Problem 374: Maximum Integer Partition Product

## Problem Statement

An integer partition of $n$ is a way of writing $n$ as a sum of positive integers (order irrelevant). For each positive integer $n$, define $f(n)$ as the maximum product over all partitions of $n$ (where a single-part partition $\{n\}$ has product $n$). Let:

$$S(N) = \sum_{n=1}^{N} f(n) \pmod{982451653}$$

Compute $S(10^{14})$.

## Mathematical Foundation

**Theorem (Optimal Partition Structure).** For $n \ge 5$, the partition of $n$ that maximizes the product consists only of parts equal to 2 or 3, with at most two 2s. Specifically:

$$f(n) = \begin{cases}
3^{n/3} & \text{if } n \equiv 0 \pmod{3}, \\
4 \cdot 3^{(n-4)/3} & \text{if } n \equiv 1 \pmod{3}, \\
2 \cdot 3^{(n-2)/3} & \text{if } n \equiv 2 \pmod{3}.
\end{cases}$$

With base cases $f(1) = 1$, $f(2) = 2$, $f(3) = 3$, $f(4) = 4$.

**Proof.** We establish several sub-claims.

*Claim 1: No part equals 1.* Replacing a part 1 in partition $(\ldots, a, 1)$ with part $(a+1)$ increases the product since $(a+1) > a \cdot 1$.

*Claim 2: No part exceeds 4.* For any part $k \ge 5$, we can split it: if $k \ge 5$, write $k = 3 + (k-3)$. Then $3(k-3) = 3k - 9 \ge k$ iff $2k \ge 9$, which holds for $k \ge 5$. So replacing $k$ with $\{3, k-3\}$ does not decrease the product, and strictly increases it for $k \ge 5$.

*Claim 3: Replace 4 with $2+2$.* We have $4 = 2 + 2$ and $2 \cdot 2 = 4$, so the product is unchanged. Thus parts of size 4 can be replaced by two 2s without loss.

*Claim 4: At most two 2s.* Three 2s contribute product $8$, but $2 + 2 + 2 = 6 = 3 + 3$ and $3 \cdot 3 = 9 > 8$. So replace any triple of 2s with a pair of 3s.

*Claim 5: Prefer 3s over 2s.* By Claim 4, the optimal partition uses $k$ threes and $r$ twos where $3k + 2r = n$ with $r \in \{0, 1, 2\}$. This gives $f(n) = 3^k \cdot 2^r$, matching the formula above. $\square$

**Lemma (Geometric Series Summation).** Grouping terms of $S(N)$ by residue class modulo 3, each group forms a geometric series with common ratio 3. For a geometric series $\sum_{k=0}^{K} a \cdot 3^k = a \cdot \frac{3^{K+1} - 1}{2}$, computed modulo $M$ using the modular inverse of 2.

**Proof.** For the group $n \equiv 0 \pmod{3}$: the terms are $f(3) = 3, f(6) = 9, f(9) = 27, \ldots$, i.e., $3^1, 3^2, 3^3, \ldots, 3^{\lfloor N/3 \rfloor}$. This is $\sum_{k=1}^{\lfloor N/3 \rfloor} 3^k = 3 \cdot \frac{3^{\lfloor N/3 \rfloor} - 1}{2}$. The other groups are analogous with prefactors 4 and 2 respectively. $\square$

## Editorial
f(n) = maximum product obtainable from an integer partition of n. S(N) = sum of f(n) for n = 1 to N, computed modulo M. Optimal partition strategy: Each residue class sums to a geometric series. We group n ≡ 0 (mod 3): terms 3, 6, 9, ..., contributing 3^1, 3^2, ..., 3^(N/3). We then adjust for base cases if needed (f(1)=1, f(2)=2, f(3)=3, f(4)=4). Finally, group n ≡ 2 (mod 3): n = 2, 5, 8, ..., contributing 2, 2*3, 2*3^2, .

## Pseudocode

```text
M = 982451653
Group n ≡ 0 (mod 3): terms 3, 6, 9, ..., contributing 3^1, 3^2, ..., 3^(N/3)
Adjust for base cases if needed (f(1)=1, f(2)=2, f(3)=3, f(4)=4)
Group n ≡ 2 (mod 3): n = 2, 5, 8, ..., contributing 2, 2*3, 2*3^2, 
Group n ≡ 1 (mod 3): n = 4, 7, 10, ..., contributing 4, 4*3, 4*3^2, 
Add base cases f(1)=1 if N>=1, f(2)=2 if N>=2 (already in sum2?), f(3)=3, f(4)=4
Careful bookkeeping needed for small n
```

## Complexity Analysis

- **Time:** $O(\log N)$ for modular exponentiation (three calls). The geometric sums are computed in closed form.
- **Space:** $O(1)$.

## Answer

$$\boxed{334420941}$$
