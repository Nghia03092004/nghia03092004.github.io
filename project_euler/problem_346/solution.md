# Problem 346: Strong Repunits

## Problem Statement

A positive integer $n$ is a **repunit** in base $b$ if its base-$b$ representation consists entirely of 1s, i.e., $n = \sum_{i=0}^{k-1} b^i$ for some $k \ge 2$. A positive integer is a **strong repunit** if it is a repunit in at least two bases $b \ge 2$.

Find the sum of all strong repunits below $10^{12}$.

## Mathematical Foundation

**Theorem 1 (Repunit Formula).** The repunit with $k$ digits in base $b$ is
$$R(b,k) = \sum_{i=0}^{k-1} b^i = \frac{b^k - 1}{b-1}.$$

**Proof.** This is the standard geometric series formula: $\sum_{i=0}^{k-1} b^i = \frac{b^k - 1}{b - 1}$ for $b \ne 1$. $\square$

**Theorem 2 (Trivial Repunit Property).** Every integer $n \ge 2$ is the repunit $\underbrace{11}_{2\text{ digits}}$ in base $b = n - 1$, since $1 \cdot (n-1) + 1 = n$.

**Proof.** $R(n-1, 2) = 1 + (n-1) = n$. $\square$

**Lemma 1 (Strong Repunit Characterization).** An integer $n \ge 2$ is a strong repunit if and only if $n$ is a repunit with $k \ge 3$ digits in at least one base $b \ge 2$. The number 1 is also a strong repunit (it is "1" in every base, and $R(b,1) = 1$ for all $b$).

**Proof.** By Theorem 2, every $n \ge 2$ is already a 2-digit repunit in base $n-1$. So $n$ is a strong repunit iff it is a repunit in at least one *additional* base, which means it must be a repunit with $k \ge 3$ digits in some base $b \ge 2$ (a 2-digit repunit in a different base would also suffice, but $R(b,2) = b+1$ and being equal to $R(b',2) = b'+1$ for $b \ne b'$ is impossible). For $n = 1$: $R(b,1) = 1$ for all $b \ge 2$, so 1 qualifies trivially. $\square$

**Lemma 2 (Enumeration Bounds).** For $k \ge 3$ and $R(b,k) < L = 10^{12}$:
- For $k = 3$: $b < \sqrt{L} \approx 10^6$.
- For $k = 40$: $R(2, 40) = 2^{40} - 1 \approx 1.1 \times 10^{12}$, so $k \le 40$ suffices.
- The total number of $(b,k)$ pairs is $O\!\left(\sum_{k=3}^{40} L^{1/(k-1)}\right) = O(10^6)$.

**Proof.** $R(b,k) \ge b^{k-1}$, so $b \le L^{1/(k-1)}$. For $k = 3$, $b \le L^{1/2} = 10^6$. For $k = 40$, $b \le L^{1/39} < 3$. Summing gives a geometric-like series dominated by the $k=3$ term. $\square$

## Editorial
We enumerate the admissible parameter range, discard candidates that violate the derived bounds or arithmetic constraints, and update the final set or total whenever a candidate passes the acceptance test.

## Pseudocode

```text
    repunits = empty set

    For k from 3 to 40:
        For b from 2 to ...:
            r = (b^k - 1) / (b - 1)
            If r >= L then stop this loop
            repunits.add(r)

    repunits.add(1) // 1 is a strong repunit
    Return sum(repunits)
```

## Complexity Analysis

- **Time:** $O\!\left(\sum_{k=3}^{40} L^{1/(k-1)}\right) = O(10^6)$ for enumeration plus $O(|\text{set}| \log |\text{set}|)$ for deduplication. Total: $O(10^6)$.
- **Space:** $O(|\text{set}|) = O(10^6)$ for storing distinct repunits.

## Answer

$$\boxed{336108797689259276}$$
