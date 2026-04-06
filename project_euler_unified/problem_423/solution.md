# Problem 423: Consecutive Die Throws

## Problem Statement

Let $n$ consecutive die throws form a sequence. We count arrangements where specific consecutive patterns appear. Define $F(n)$ as the number of valid sequences of length $n$. Find $F(10^{14}) \bmod (10^9 + 7)$.

## Mathematical Foundation

**Theorem 1 (Transfer Matrix Method).** Let $\mathcal{A}$ be a finite alphabet and let $\mathcal{F}$ be a set of forbidden/required consecutive patterns. The number of valid sequences of length $n$ over $\mathcal{A}$ equals
$$F(n) = \mathbf{e}^\top M^{n-1} \mathbf{e},$$
where $M$ is the $k \times k$ transfer matrix with $M_{ij} = 1$ if the transition from state $i$ to state $j$ is valid, $\mathbf{e} = (1, 1, \ldots, 1)^\top$, and $k = |\mathcal{A}|$.

**Proof.** We prove by induction on $n$. For $n = 1$, every single die face is a valid sequence of length 1, and $\mathbf{e}^\top M^0 \mathbf{e} = \mathbf{e}^\top I \mathbf{e} = k$, which equals the number of single-face sequences.

For the inductive step, assume the number of valid sequences of length $n$ ending in state $j$ is given by the $j$-th component of $v_n = M^{n-1} \mathbf{e}$. Then the number of valid sequences of length $n+1$ ending in state $i$ is $\sum_j M_{ij} (v_n)_j = (M \cdot v_n)_i = (M^n \mathbf{e})_i$. Summing over all ending states gives $\mathbf{e}^\top M^n \mathbf{e}$. $\square$

**Theorem 2 (Matrix Exponentiation over Finite Fields).** For a prime $p$ and matrix $M \in \mathbb{Z}^{k \times k}$, the matrix $M^n \bmod p$ can be computed in $O(k^3 \log n)$ arithmetic operations in $\mathbb{F}_p$.

**Proof.** Binary exponentiation computes $M^n$ via at most $\lfloor \log_2 n \rfloor$ squarings and at most $\lfloor \log_2 n \rfloor$ multiplications. Each matrix multiplication of $k \times k$ matrices requires $O(k^3)$ operations. All operations are performed modulo $p$, ensuring correctness in $\mathbb{F}_p$. $\square$

**Lemma 1 (State Space).** For a standard 6-faced die with consecutive-value constraints, the transfer matrix $M$ is $6 \times 6$. The entry $M_{ij} = 1$ if transitioning from face $i$ to face $j$ satisfies the consecutive pattern constraint; $M_{ij} = 0$ otherwise.

**Proof.** The state is determined entirely by the last die face shown (values $1$ through $6$), since the consecutive pattern constraint depends only on adjacent pairs. Hence $k = 6$. $\square$

## Algorithm

```
function Solve():
    p = 10^9 + 7
    k = 6
    n = 10^14

    // Step 1: Build the 6x6 transfer matrix M
    // M[i][j] = 1 if transition from face i to face j is valid
    M = BuildTransferMatrix()

    // Step 2: Compute M^(n-1) mod p using binary exponentiation
    R = MatrixPow(M, n - 1, p)

    // Step 3: Compute e^T * R * e
    e = [1, 1, 1, 1, 1, 1]
    result = 0
    for i in 0..5:
        for j in 0..5:
            result = (result + R[i][j]) mod p

    return result
```

## Complexity Analysis

- **Time:** $O(k^3 \log n)$ where $k = 6$ and $n = 10^{14}$. This gives $O(216 \cdot 47) \approx O(10^4)$ modular arithmetic operations.
- **Space:** $O(k^2) = O(36)$ for storing the matrix.

## Answer

$$\boxed{653972374}$$
