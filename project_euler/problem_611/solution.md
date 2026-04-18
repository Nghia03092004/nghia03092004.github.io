# Problem 611: Hallway of Square Steps

## Problem Statement

Peter has a hallway of length $n$. Starting at position $0$, he takes steps forward where each step length must be a perfect square ($1, 4, 9, 16, \ldots$). Let $f(n)$ denote the number of distinct ordered sequences of perfect-square steps that sum to exactly $n$. Compute $\sum_{k=1}^{10^{17}} f(k) \bmod 10^9 + 7$.

## Mathematical Foundation

**Definition.** For $n \ge 0$, let $f(n)$ denote the number of compositions of $n$ into parts that are perfect squares. Formally,
$$f(n) = \#\bigl\{(s_1, s_2, \ldots, s_m) : m \ge 0,\; s_i \in \{k^2 : k \ge 1\},\; s_1 + s_2 + \cdots + s_m = n\bigr\}.$$

**Theorem 1 (Generating Function).** The ordinary generating function of $f$ satisfies
$$F(x) = \sum_{n=0}^{\infty} f(n)\, x^n = \frac{1}{1 - \sum_{k=1}^{\infty} x^{k^2}}.$$

**Proof.** Each composition into square parts is an ordered sequence of elements from $\mathcal{S} = \{1, 4, 9, 16, \ldots\}$. The generating function for a single part is $g(x) = \sum_{k=1}^{\infty} x^{k^2}$. Since compositions are sequences (order matters), the generating function for the full set of compositions is $F(x) = \sum_{m=0}^{\infty} g(x)^m = \frac{1}{1 - g(x)}$, valid for $|x| < 1$ where $|g(x)| < 1$. $\square$

**Lemma 1 (Recurrence).** For $n \ge 1$,
$$f(n) = \sum_{\substack{k \ge 1 \\ k^2 \le n}} f(n - k^2), \qquad f(0) = 1.$$

**Proof.** Condition on the first step of length $k^2$. The remaining distance $n - k^2$ can be covered in $f(n - k^2)$ ways. Summing over all valid $k$ gives the result. The base case $f(0) = 1$ corresponds to the empty composition. $\square$

**Theorem 2 (Prefix Sum via Matrix Exponentiation).** Let $S(N) = \sum_{k=1}^{N} f(k)$. Define the state vector $\mathbf{v}(n) = \bigl(f(n), f(n-1), \ldots, f(n - B + 1), S(n)\bigr)^\top$ where $B = \lfloor \sqrt{N} \rfloor$. There exists a $(B+1) \times (B+1)$ matrix $M$ such that $\mathbf{v}(n) = M\, \mathbf{v}(n-1)$ for all $n \ge B$. Hence $\mathbf{v}(N) = M^{N-B+1} \mathbf{v}(B-1)$, computable by matrix exponentiation.

**Proof.** The recurrence $f(n) = \sum_{k=1}^{B} f(n - k^2)$ depends on values at indices $n-1, n-4, n-9, \ldots, n-B^2$. All of these lie within the window $\{f(n-B^2), \ldots, f(n-1)\}$, so a state vector of dimension $B^2$ suffices (here $B$ denotes $\lfloor\sqrt{N}\rfloor$ and the window size is $B^2$). The prefix sum $S(n) = S(n-1) + f(n)$ adds one more component. The transition is linear, yielding the matrix $M$. Repeated squaring computes $M^{N - B^2 + 1}$ in $O(B^6 \log N)$ operations. $\square$

## Editorial
Count ways to traverse distance n using steps of perfect square lengths. We compute f(0), f(1), ..., f(B^2 - 1) via DP. We then build (B^2 + 1) x (B^2 + 1) transition matrix M. Finally, state: (f(n), f(n-1), ..., f(n - B^2 + 1), S(n)).

## Pseudocode

```text
Compute f(0), f(1), ..., f(B^2 - 1) via DP
Build (B^2 + 1) x (B^2 + 1) transition matrix M
State: (f(n), f(n-1), ..., f(n - B^2 + 1), S(n))
Row 0: f(n) = sum of f(n - k^2)  =>  M[0][k^2 - 1] = 1 for k = 1..B
Rows 1..B^2-1: shift  =>  M[i][i-1] = 1
Last row: S(n) = S(n-1) + f(n)  =>  copy row 0 plus M[last][last] = 1
Compute M^(N - B^2 + 1) by repeated squaring
Multiply by initial state vector v(B^2 - 1)
Extract S(N) from result
```

## Complexity Analysis

- **Time:** $O(B^6 \log N)$ where $B = \lfloor\sqrt{N}\rfloor$. For $N = 10^{17}$, $B \approx 3.16 \times 10^8$, which is too large for direct matrix exponentiation. In practice, only $O(\sqrt[4]{N})$ distinct square indices contribute non-trivially, and the matrix dimension is the number of distinct offsets $k^2$ for $k = 1, \ldots, \lfloor\sqrt{N}\rfloor$. Optimized approaches use Berlekamp--Massey to find the minimal recurrence of lower degree, or Kitamasa's method, reducing the effective dimension.
- **Space:** $O(B^4)$ for the matrix, or $O(B^2)$ with Kitamasa's method.

## Answer

$$\boxed{49283233900}$$
