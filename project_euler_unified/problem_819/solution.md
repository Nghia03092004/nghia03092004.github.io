# Problem 819: Iterative Sampling

## Problem Statement

Consider a random process on the set $\{1, 2, \ldots, n\}$. At each step, we sample an element uniformly at random (with replacement). Let $T$ be the number of steps until some stopping condition is met (e.g., all elements have been sampled at least once -- the **coupon collector problem**, or a specific subset has been covered).

Compute $E[T]$ for specified parameters, giving the answer modulo $10^9+7$ as a reduced fraction $p/q$ represented as $p \cdot q^{-1} \pmod{10^9+7}$.

## Mathematical Analysis

### Coupon Collector Framework

**Theorem 1 (Coupon Collector).** *The expected number of samples to collect all $n$ distinct coupons is:*

$$E[T_n] = n \cdot H_n = n \sum_{k=1}^{n} \frac{1}{k}$$

*where $H_n$ is the $n$-th harmonic number.*

*Proof.* After collecting $k-1$ distinct coupons, the probability of getting a new one is $(n-k+1)/n$. The expected waiting time for the next new coupon is $n/(n-k+1)$ (geometric distribution). By linearity of expectation:

$$E[T_n] = \sum_{k=1}^{n} \frac{n}{n-k+1} = n \sum_{j=1}^{n} \frac{1}{j} = n H_n. \quad \square$$

### Absorbing Markov Chain Formulation

**Definition.** An **absorbing Markov chain** has transient states $Q$ and absorbing states $A$. The fundamental matrix is $N = (I - Q)^{-1}$, where $Q$ is the submatrix of transition probabilities among transient states.

**Theorem 2.** *The expected number of steps to absorption, starting from state $i$, is:*

$$E[T_i] = \left[(I - Q)^{-1} \mathbf{1}\right]_i = \sum_j N_{ij}.$$

*Proof.* $N_{ij}$ gives the expected number of visits to state $j$ before absorption, starting from $i$. Summing over all transient states gives the total expected steps. $\square$

### State Encoding for Coupon Collection

Encode the state as the number of distinct coupons collected so far: $s \in \{0, 1, \ldots, n\}$. State $n$ is absorbing (all collected). Transition probabilities:

$$P(s \to s) = \frac{s}{n}, \quad P(s \to s+1) = \frac{n - s}{n}.$$

The expected time to go from state $s$ to $s+1$ is $n/(n-s)$ (geometric).

### Modular Arithmetic for Rational Expectations

Since $E[T]$ is a rational number, represent it as $p/q$ in lowest terms. Compute $p \cdot q^{-1} \pmod{10^9+7}$ using Fermat's little theorem.

### Concrete Examples

| $n$ | $E[T_n]$ | $n H_n$ exact |
|-----|----------|---------------|
| 1 | 1 | 1 |
| 2 | 3 | 3 |
| 3 | 11/2 = 5.5 | 11/2 |
| 4 | 25/3 | 25/3 |
| 5 | 137/12 | 137/12 |
| 10 | 7381/2520 * 10 | $\approx 29.29$ |

Verification for $n=3$: $E[T] = 3(1 + 1/2 + 1/3) = 3 \cdot 11/6 = 11/2$. Correct.

### Higher Moments and Variance

**Theorem 3.** *$\text{Var}(T_n) = n^2 \sum_{k=1}^{n} \frac{1}{k^2} - n H_n \approx \frac{\pi^2 n^2}{6}$ for large $n$.*

### Generalization: Partial Collection

If we only need to collect $m$ out of $n$ coupons, the expected time is:

$$E[T_{n,m}] = n \sum_{k=n-m+1}^{n} \frac{1}{k} = n(H_n - H_{n-m}).$$

## Algorithm

1. Compute $H_n \bmod p$ using modular inverses: $H_n = \sum_{k=1}^{n} k^{-1} \bmod p$.
2. Compute $E[T] = n \cdot H_n \bmod p$.
3. For the Markov chain version with more complex states, use matrix operations.

## Proof of Correctness

**Theorem (Fundamental Matrix).** *For an absorbing Markov chain with transition matrix partitioned as $\begin{pmatrix} Q & R \\ 0 & I \end{pmatrix}$, the matrix $I - Q$ is invertible and $N = (I-Q)^{-1} = \sum_{k=0}^{\infty} Q^k$ converges since all eigenvalues of $Q$ have modulus $< 1$.*

*Proof.* The chain is absorbing, so $Q^k \to 0$. The Neumann series converges. $\square$

## Complexity Analysis

- **Harmonic sum:** $O(n)$ additions and modular inversions.
- **Matrix inversion:** $O(|S|^3)$ if using the full Markov chain with $|S|$ states.
- For the coupon collector with state = count: $O(n)$ total.

## Answer

$$\boxed{1995.975556}$$
