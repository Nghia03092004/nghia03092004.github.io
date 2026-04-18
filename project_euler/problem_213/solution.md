# Problem 213: Flea Circus

## Problem Statement

A $30 \times 30$ grid starts with exactly one flea on each square. Each round, every flea independently jumps to a uniformly random adjacent square (up, down, left, right). Corner fleas have 2 choices, edge fleas have 3, and interior fleas have 4.

After 50 rounds, what is the expected number of empty squares?

## Mathematical Development

**Definition 1.** Let $G = (V, E)$ be the grid graph with $V = \{(r, c) : 0 \leq r, c < 30\}$ and edges between horizontally or vertically adjacent squares. Let $N = |V| = 900$. Index the squares $1, \ldots, N$.

**Definition 2.** The *transition matrix* $T \in \mathbb{R}^{N \times N}$ of the random walk on $G$ is defined by

$$T_{ij} = \begin{cases} 1/\deg(i) & \text{if } j \sim i, \\ 0 & \text{otherwise}, \end{cases}$$

where $\deg(i) \in \{2, 3, 4\}$ is the degree of vertex $i$ and $j \sim i$ means $j$ is adjacent to $i$.

**Lemma 1 (Row stochasticity).** *The matrix $T$ is row-stochastic: $\sum_{j=1}^{N} T_{ij} = 1$ for all $i$.*

*Proof.* For any vertex $i$, $\sum_{j} T_{ij} = \sum_{j \sim i} 1/\deg(i) = \deg(i) \cdot (1/\deg(i)) = 1$. $\square$

**Theorem 1 (Chapman--Kolmogorov).** *The entry $(T^k)_{ij}$ equals the probability that a random walk starting at vertex $i$ is at vertex $j$ after exactly $k$ steps.*

*Proof.* By induction on $k$. The base case $k = 1$ holds by definition of $T$. For the inductive step, $(T^{k+1})_{ij} = \sum_{\ell} (T^k)_{i\ell} \cdot T_{\ell j}$, which by the inductive hypothesis equals $\sum_{\ell} P(\text{at } \ell \text{ after } k \text{ steps from } i) \cdot P(\text{move to } j \text{ from } \ell) = P(\text{at } j \text{ after } k+1 \text{ steps from } i)$, by the law of total probability. $\square$

**Theorem 2 (Expected empty squares).** *Let $\mathbf{1}_i$ be the indicator random variable for the event that square $i$ is empty after 50 rounds. Then*

$$E\!\left[\sum_{i=1}^{N} \mathbf{1}_i\right] = \sum_{i=1}^{N} \prod_{j=1}^{N} \bigl(1 - (T^{50})_{ji}\bigr).$$

*Proof.* Step 1: By linearity of expectation (which holds without any independence assumption),

$$E\!\left[\sum_{i=1}^{N} \mathbf{1}_i\right] = \sum_{i=1}^{N} P(\text{square } i \text{ is empty}).$$

Step 2: Square $i$ is empty if and only if none of the $N$ fleas occupies it. Since fleas move independently (each flea's random walk is independent of all others), the events $\{\text{flea } j \neq i \text{ after 50 rounds}\}$ are mutually independent across distinct $j$. Therefore:

$$P(\text{square } i \text{ empty}) = \prod_{j=1}^{N} P(\text{flea } j \neq i) = \prod_{j=1}^{N} \bigl(1 - (T^{50})_{ji}\bigr),$$

where $(T^{50})_{ji}$ is the probability that flea $j$ (starting at square $j$) is at square $i$ after 50 steps, by Theorem 1. $\square$

**Remark.** To avoid numerical underflow in the product $\prod_j (1 - (T^{50})_{ji})$, one may compute $\sum_j \log(1 - (T^{50})_{ji})$ and exponentiate. The function $\log(1 - x)$ is well-conditioned for small $x$, and the library function `log1p(-x)` provides enhanced precision.

## Editorial
We build transition matrix T. We then compute T^R via matrix exponentiation. Finally, accumulate expected empty squares. We enumerate the admissible parameter range, discard candidates that violate the derived bounds or arithmetic constraints, and update the final set or total whenever a candidate passes the acceptance test.

## Pseudocode

```text
Build transition matrix T
Compute T^R via matrix exponentiation
Accumulate expected empty squares
```

## Complexity Analysis

**Time.** Matrix exponentiation via repeated squaring requires $O(\log R)$ matrix multiplications, each costing $O(N^3)$. Thus the total is $O(N^3 \log R)$. For $N = 900$ and $R = 50$, this is approximately $900^3 \cdot 6 \approx 4.4 \times 10^9$. Alternatively, propagating each of the $N$ flea distributions individually over $R$ rounds costs $O(N \cdot R \cdot N) = O(N^2 R) = O(900^2 \cdot 50) \approx 4 \times 10^7$, which is more practical. The product computation adds $O(N^2)$. Overall: $O(N^2 R)$.

**Space.** $O(N^2)$ for storing the full probability matrix, or $O(N)$ if accumulating products column-by-column.

## Answer

$$\boxed{330.721154}$$
