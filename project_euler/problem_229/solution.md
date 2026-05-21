# Problem 229: Four Representations Using Squares

## Problem Statement

We seek the positive integers $n \le 2 \times 10^9$ that admit all four representations

$$
\begin{aligned}
n &= a_1^2 + b_1^2, \\
n &= a_2^2 + 2b_2^2, \\
n &= a_3^2 + 3b_3^2, \\
n &= a_7^2 + 7b_7^2,
\end{aligned}
$$

where every $a_k$ and $b_k$ is positive.

How many such $n$ are there?

## Mathematical Development

For each coefficient $k \in \{1,2,3,7\}$, define

$$
R_k=\{a^2+k b^2 : a,b \ge 1\}.
$$

The answer is the size of

$$
R_1 \cap R_2 \cap R_3 \cap R_7
$$

inside $[1,2\times 10^9]$.

The direct issue is memory. A full Boolean table up to $2\times 10^9$ would be far too large, even before storing four copies. The key monotonicity observation is that for fixed $a$ and fixed $k$, the values

$$
a^2+k b^2
$$

increase strictly with $b$. Therefore, if we process the search interval in slices

$$
[L,R),
$$

then for each pair $(a,k)$ the admissible $b$ values inside that slice form one consecutive interval of integers. When we move to the next slice, the first relevant $b$ can never move backwards.

So it is enough to store four rolling pointers:

- $b_1[a]$ for $a^2+b^2$,
- $b_2[a]$ for $a^2+2b^2$,
- $b_3[a]$ for $a^2+3b^2$,
- $b_7[a]$ for $a^2+7b^2$.

Inside one slice we mark every hit with a four-bit mask. At the end of the slice, a number belongs to the intersection exactly when all four bits are set.

This computes the intersection exactly, but only uses memory proportional to the slice size.

## Editorial

The brute-force formulation is not actually the enemy here; global storage is. For each of the four quadratic forms, the set of represented values is sparse, but the search interval is enormous. The useful observation is that the curves

$$
a^2 + k b^2
$$

are monotone in $b$, so once a certain $b$ has moved past the current slice, it never needs to be revisited.

That is why the implementation keeps the interval in rolling blocks of one million integers. For every $a$, it remembers where each form last left off in $b$, resumes from there, and marks only the values that land in the current block. A single byte per value is enough: four bits record which of the four forms hit that value. After all $(a,b)$ pairs that can reach the block have been processed, the bytes equal to `1111` are exactly the numbers represented in all four ways.

## Pseudocode

```text
Choose a block size B.
For every a up to sqrt(N), initialize four pointers:
    b1[a], b2[a], b3[a], b7[a] = 1.

For each block [L, R):
    clear a byte array of length R - L

    For a from 1 upward while a^2 + b1[a]^2 < R:
        continue the sequence a^2 + b^2 from b = b1[a]
            and set bit 0 for every hit inside the block
        continue the sequence a^2 + 2b^2 from b = b2[a]
            and set bit 1
        continue the sequence a^2 + 3b^2 from b = b3[a]
            and set bit 2
        continue the sequence a^2 + 7b^2 from b = b7[a]
            and set bit 3

        store the first unused b back into the corresponding pointer

    Count how many entries in the block have all four bits set.
Add those counts over all blocks.
```

## Complexity Analysis

- **Time:** Proportional to the total number of marked values
  $a^2 + k b^2 \le N$ across the four forms.
- **Space:** $O(B + \sqrt N)$ for the current slice and the rolling $b$ pointers.

## Answer

$$\boxed{11325263}$$
