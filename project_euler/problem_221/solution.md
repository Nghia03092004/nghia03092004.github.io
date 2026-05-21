# Problem 221: Alexandrian Integers

## Problem Statement

We call a positive integer $A$ an **Alexandrian integer** if there exist integers $p, q, r$ such that

$$
A = pqr
\qquad\text{and}\qquad
\frac{1}{A} = \frac{1}{p} + \frac{1}{q} + \frac{1}{r}.
$$

Find the $150000$th Alexandrian integer.

## Mathematical Development

Multiplying the defining identity by $A = pqr$ gives

$$
pq + pr + qr = 1.
$$

Now fix $p$ and complete the square in the remaining two variables:

$$
(p + q)(p + r) = p^2 + pq + pr + qr = p^2 + 1.
$$

So every solution produces a factorization of $p^2 + 1$.

If $p, q, r$ were all positive, then $p + q > p$ and

$$
p + r = \frac{p^2 + 1}{p + q} < \frac{p^2 + 1}{p} = p + \frac{1}{p},
$$

which leaves no room for an integer strictly between $p$ and $p + 1/p$. Hence the positive branch is impossible. For an Alexandrian integer we may therefore take $p > 0$ and $q, r < 0$.

Write

$$
p + q = -d,
\qquad
p + r = -\frac{p^2 + 1}{d},
$$

where $d$ is a positive divisor of $p^2 + 1$. Then

$$
q = -d - p,
\qquad
r = -\frac{p^2 + 1}{d} - p,
$$

and therefore

$$
A = p(d + p)\left(\frac{p^2 + 1}{d} + p\right).
$$

Swapping $d$ with $(p^2 + 1)/d$ only swaps $q$ and $r$, so it is enough to take

$$
d \le \sqrt{p^2 + 1}.
$$

This parametrizes every Alexandrian integer.

## Editorial

Once the identity is rewritten as $(p+q)(p+r)=p^2+1$, the search becomes one-dimensional. For each fixed positive $p$, every divisor $d$ of $p^2+1$ produces one candidate Alexandrian integer

$$
A = p(d+p)\left(\frac{p^2+1}{d}+p\right).
$$

That is the whole problem: enumerate these candidates, remove duplicates, sort them, and take the $150000$th.

The only practical question is how to enumerate the divisors of $p^2+1$ efficiently. The C++ code simply scans divisors up to $\sqrt{p^2+1}$ for each $p$, which is fast enough. The Python code uses the same parametrization but evaluates $\sigma_2$-style factor information in blocks so that it does not need a large global table. Both implementations are doing the same mathematical search over divisors of $p^2+1$.

## Pseudocode

```text
Set the target index and a safe upper bound for p.
Create an empty set of Alexandrian integers.

For p from 1 up to the chosen bound:
    N = p^2 + 1

    For each divisor d of N with d <= sqrt(N):
        e = N / d
        A = p * (d + p) * (e + p)
        Insert A into the set

Sort the distinct values.
Return the element at position 150000.
```

## Complexity Analysis

- **Time:** If divisors are found by trial division, the work is
  $$
  \sum_{p \le P_{\max}} O(\sqrt{p^2+1}) = O(P_{\max}^2).
  $$
  The final sort costs $O(M \log M)$ for $M$ distinct candidates.
- **Space:** $O(M)$ for the set of generated Alexandrian integers.

## Answer

$$\boxed{1884161251122450}$$
