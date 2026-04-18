# Problem 255: Rounded Square Roots

## Problem Statement

Define the "rounded square root" algorithm for a positive integer $n$:

1. Let $d$ be the number of digits of $n$.
2. If $d$ is odd, set $x_0 = 2 \times 10^{(d-1)/2}$. If $d$ is even, set $x_0 = 7 \times 10^{(d-2)/2}$.
3. Iterate: $x_{k+1} = \left\lfloor \dfrac{x_k + \lceil n / x_k \rceil}{2} \right\rfloor$.
4. Stop when $x_{k+1} = x_k$; return $x_k$.

Find the average number of iterations for all $n$ from $10^{13}$ to $10^{14} - 1$, to 10 decimal places.

## Mathematical Foundation

**Lemma 1 (Fixed Range).** *All integers in $[10^{13}, 10^{14})$ have exactly $d = 14$ digits (even), so the initial guess is $x_0 = 7 \times 10^6$.*

**Proof.** An integer $n$ has $d$ digits if and only if $10^{d-1} \le n < 10^d$. For $n \in [10^{13}, 10^{14})$, we have $d = 14$. Since $d$ is even, $x_0 = 7 \times 10^{(14-2)/2} = 7{,}000{,}000$. $\square$

**Theorem 1 (Convergence).** *The iteration $x_{k+1} = \lfloor (x_k + \lceil n/x_k \rceil)/2 \rfloor$ is an integer variant of Newton's method for $\sqrt{n}$. For any $n \ge 1$ and any initial $x_0 \ge 1$, the sequence $(x_k)$ converges to a fixed point $x^*$ satisfying $x^*(x^*-1) < n \le x^*(x^*+1)$, i.e., $x^*$ is the nearest integer to $\sqrt{n}$ (rounded up on half-integers).*

**Proof.** Newton's method for $f(x) = x^2 - n$ gives the iteration $x \mapsto (x + n/x)/2$. The integer variant uses ceiling division and floor of the average. One can verify:

1. If $x_k > x^*$, then $\lceil n/x_k \rceil \le x_k$, and $x_{k+1} \le x_k$ (the sequence is non-increasing once above the fixed point).
2. If $x_k \le x^*$, then $\lceil n/x_k \rceil \ge x_k$, and $x_{k+1} \ge x_k$.
3. The sequence is eventually monotone and bounded, hence convergent.

The fixed-point condition $x^* = \lfloor (x^* + \lceil n/x^* \rceil)/2 \rfloor$ characterizes $x^*$ as the rounded square root. $\square$

**Theorem 2 (Grouping by Quotient).** *For fixed $x_k$, the values of $n \in [L, R]$ producing a given ceiling quotient $q = \lceil n/x_k \rceil$ form the interval $n \in [(q-1)x_k + 1, \; qx_k]$. The next iterate is then $x_{k+1} = \lfloor (x_k + q)/2 \rfloor$, which is constant over this interval.*

**Proof.** By definition of ceiling division, $\lceil n/x_k \rceil = q$ iff $(q-1)x_k < n \le qx_k$, i.e., $n \in [(q-1)x_k + 1, qx_k]$. Since $x_{k+1}$ depends on $n$ only through $q$, all $n$ in this interval share the same $x_{k+1}$. $\square$

## Editorial
Average number of iterations of the rounded square root algorithm for n from 10^13 to 10^14 - 1. All numbers have 14 digits (even), so x0 = 7 * 10^6 = 7000000. Iteration: x_{k+1} = floor((x_k + ceil(n/x_k)) / 2) Stop when x_{k+1} == x_k. Interval-based approach: track (x_current, n_lo, n_hi) and split by ceil(n/x) values at each step. We use a map: x_value -> count_of_n_values. We then initially, all n in [N_lo, N_hi] start with x = x0. Finally, converged: these n values took `iteration` iterations.

## Pseudocode

```text
Use a map: x_value -> count_of_n_values
Initially, all n in [N_lo, N_hi] start with x = x0
For each distinct q = ceil(n/x), compute x_next
Converged: these n values took `iteration` iterations
else
Continue iterating: group by x_next
```

## Complexity Analysis

- **Time:** At each iteration, the number of groups is $O(\sqrt{N})$ (since the number of distinct ceiling quotients for a given $x$ over $[N_{\text{lo}}, N_{\text{hi}}]$ is $O(\sqrt{N})$ by the divisor-bound argument). With $O(\log N)$ iterations, the total is $O(\sqrt{N} \log N)$. For $N = 10^{14}$: approximately $10^7 \times 47 \approx 5 \times 10^8$.
- **Space:** $O(\sqrt{N})$ for storing the groups at each iteration level.

## Answer

$$\boxed{4.4474011180}$$
