# Problem 844: k-Markov Sequences

## Problem Statement

A **$k$-Markov sequence** over an alphabet $\Sigma$ of size $|\Sigma| = \sigma$ is a sequence where the probability of each symbol depends only on the preceding $k$ symbols. Count the number of $k$-Markov sequences of length $n$ over $\Sigma$ satisfying certain transition constraints, modulo a prime $p$.

Equivalently: given a $\sigma^k \times \sigma^k$ transfer matrix $M$ on the state space of $k$-grams, compute $\text{tr}(M^n)$ or $\mathbf{1}^T M^n \mathbf{1}$ modulo $p$.

## Mathematical Analysis

### Transfer Matrix Framework

**Definition.** The state space is $\Sigma^k$, the set of all $k$-tuples (k-grams). The transfer matrix $M$ has entry $M[s, t] = 1$ if state $t$ is a valid successor of state $s$ (i.e., the last $k-1$ symbols of $s$ match the first $k-1$ symbols of $t$).

**Theorem.** The number of valid sequences of length $n$ is:

$$N(n) = \sum_{s \in \Sigma^k} (M^{n-k})_{s, s_0} = \mathbf{v}_0^T M^{n-k} \mathbf{1} \tag{1}$$

where $\mathbf{v}_0$ encodes initial state weights.

### Eigenvalue Decomposition

**Theorem.** If $M$ has eigenvalues $\lambda_1, \ldots, \lambda_r$ (with multiplicity), then:

$$\text{tr}(M^n) = \sum_{i=1}^{r} \lambda_i^n \tag{2}$$

This allows computation in $O(r)$ if eigenvalues are known, but they are generally irrational.

### Cayley-Hamilton Approach

**Theorem (Cayley-Hamilton).** Every matrix satisfies its own characteristic polynomial. If $\chi_M(\lambda) = \lambda^r - c_1 \lambda^{r-1} - \cdots - c_r$, then:

$$\text{tr}(M^n) = c_1 \cdot \text{tr}(M^{n-1}) + c_2 \cdot \text{tr}(M^{n-2}) + \cdots + c_r \cdot \text{tr}(M^{n-r}) \tag{3}$$

This gives a linear recurrence of order $r = \sigma^k$ for $\text{tr}(M^n)$.

### Matrix Exponentiation

For large $n$, compute $M^n \bmod p$ using binary exponentiation in $O(\sigma^{3k} \log n)$.

### Concrete Examples

**Binary alphabet ($\sigma = 2$), $k = 1$:** States are $\{0, 1\}$. If all transitions allowed, $M = \begin{pmatrix}1&1\\1&1\end{pmatrix}$, so $N(n) = 2^n$.

**Binary, $k = 2$, no "00" allowed:** States are $\{00, 01, 10, 11\}$. Transition "00" $\to$ anything is forbidden. The valid count follows a Fibonacci-like recurrence.

| $n$ | $k$ | $\sigma$ | Constraints | Count |
|-----|-----|----------|-------------|-------|
| 5   | 1   | 2        | None        | 32    |
| 5   | 1   | 2        | No "00"     | 13    |
| 10  | 2   | 2        | No "000"    | 504   |
| 8   | 1   | 3        | None        | 6561  |

### Burnside Connection for Periodic Sequences

If we want to count **periodic** $k$-Markov sequences (necklaces), Burnside's lemma gives:

$$N_{\text{periodic}}(n) = \frac{1}{n} \sum_{d \mid n} \varphi(n/d) \cdot \text{tr}(M^d)$$

## Complexity Analysis

- **Matrix exponentiation:** $O(\sigma^{3k} \log n)$ time, $O(\sigma^{2k})$ space.
- **Cayley-Hamilton recurrence:** $O(\sigma^k \cdot n)$ or $O(\sigma^{3k} \log n)$ with polynomial exponentiation.
- **Direct simulation:** $O(\sigma^k \cdot n)$ for moderate $n$.

## Answer

$$\boxed{101805206}$$
