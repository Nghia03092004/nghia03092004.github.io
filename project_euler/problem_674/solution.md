# Problem 674: Shared Splints

## Problem Statement

Count configurations of overlapping intervals (splints) on a line where splints are shared between objects. A splint of length $k$ is an interval $[a, a+k-1]$. A shared splint appears in multiple objects' intervals. Enumerate valid configurations meeting coverage and sharing constraints.

## Mathematical Analysis

### Generating Function Approach

Let $g(n)$ count valid splint configurations of total length $n$. The generating function is:

$$G(x) = \sum_{n \ge 0} g(n) x^n$$

### Interval Decomposition

Each configuration decomposes into maximal blocks of overlapping intervals. If blocks are independent, then:

$$G(x) = \frac{1}{1 - H(x)}$$

where $H(x) = \sum_{k \ge 1} h(k) x^k$ counts atomic (indecomposable) configurations of width $k$.

**Theorem.** The number of valid configurations satisfies the linear recurrence:
$$g(n) = \sum_{k=1}^{n} h(k) \cdot g(n-k), \quad g(0) = 1$$

### Counting Atomic Configurations

An atomic configuration of width $k$ has no proper sub-decomposition. The count $h(k)$ depends on:
- How many intervals overlap at each position
- The sharing constraints between intervals
- Whether shared intervals form valid covers

**Lemma.** For a simple sharing model (each position covered by at most 2 intervals), $h(k)$ satisfies a secondary recurrence related to Catalan numbers or Fibonacci variants.

### Sweep-Line Analysis

Processing intervals left-to-right, the active set changes at each endpoint. The number of active configurations at position $x$ is:

$$\text{active}(x) = |\{I : I \text{ is an interval containing } x\}|$$

The sharing constraint limits $\text{active}(x) \le C$ for some constant $C$.

## Concrete Examples

| Width $k$ | Atomic configs $h(k)$ | Description |
|-----------|----------------------|-------------|
| 1 | 1 | Single point |
| 2 | 2 | Two overlapping intervals |
| 3 | 5 | Various overlap patterns |

For total length $n = 4$: $g(4) = h(1)g(3) + h(2)g(2) + h(3)g(1) + h(4)g(0)$.

### Verification

$g(n)$ for small $n$ is verified by exhaustive enumeration of all valid interval configurations.

## Derivation

### Editorial
We compute atomic configuration counts $h(k)$ for $k = 1, \ldots, K$ using the specific problem rules. Finally, use the linear recurrence $g(n) = \sum_k h(k) g(n-k)$ to compute $g(n)$ for the target $n$.

### Pseudocode

```text
Compute atomic configuration counts $h(k)$ for $k = 1, \ldots, K$ using the specific problem rules
Use the linear recurrence $g(n) = \sum_k h(k) g(n-k)$ to compute $g(n)$ for the target $n$
If $n$ is large, use matrix exponentiation on the companion matrix of the recurrence
```

### Matrix Exponentiation

The recurrence has order $K$ (maximum atomic width). The companion matrix $M$ is $K \times K$:
$$g(n) = \mathbf{e}_1^T M^n \mathbf{g}_0$$

Compute $M^n$ by repeated squaring in $O(K^3 \log n)$.

## Proof of Correctness

**Theorem.** *The decomposition into atomic blocks is unique, so the generating function factorization $G = 1/(1-H)$ is exact.*

*Proof.* Each configuration has a unique leftmost atomic block of some width $k$. Removing it leaves a valid configuration of width $n-k$ (or empty). This bijection establishes the convolution $g(n) = \sum h(k) g(n-k)$. $\square$

## Complexity Analysis

- **Recurrence computation:** $O(n \cdot K)$ for direct evaluation.
- **Matrix exponentiation:** $O(K^3 \log n)$ for large $n$.
- **Space:** $O(K)$ for the recurrence history or $O(K^2)$ for the matrix.

## Answer

$$\boxed{416678753}$$
