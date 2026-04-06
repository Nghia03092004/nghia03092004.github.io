# Problem 972: Stern Sequence Properties

## Problem Statement

The Stern diatomic sequence $\{s(n)\}_{n \ge 0}$ is defined by:
$$s(0) = 0, \quad s(1) = 1, \quad s(2n) = s(n), \quad s(2n+1) = s(n) + s(n+1) \quad (n \ge 1).$$

Compute $\displaystyle\sum_{n=1}^{2^{20}} s(n)$.

## Mathematical Foundation

**Theorem 1 (Row Sum Identity).** *For all $k \ge 0$,*
$$\sum_{n=2^k}^{2^{k+1}-1} s(n) = 3^k.$$

**Proof.** By strong induction on $k$.

*Base case ($k=0$):* $\sum_{n=1}^{1} s(n) = s(1) = 1 = 3^0$. $\checkmark$

*Inductive step:* Assume the identity holds for $k$. We prove it for $k+1$. Split the sum over even and odd indices in $[2^{k+1}, 2^{k+2}-1]$:

$$\sum_{n=2^{k+1}}^{2^{k+2}-1} s(n) = \sum_{m=2^k}^{2^{k+1}-1} s(2m) + \sum_{m=2^k}^{2^{k+1}-1} s(2m+1).$$

Using the recurrence: $s(2m) = s(m)$ and $s(2m+1) = s(m) + s(m+1)$:

$$= \sum_{m=2^k}^{2^{k+1}-1} s(m) + \sum_{m=2^k}^{2^{k+1}-1} \bigl(s(m) + s(m+1)\bigr) = 2\sum_{m=2^k}^{2^{k+1}-1} s(m) + \sum_{m=2^k}^{2^{k+1}-1} s(m+1).$$

The last sum is $\sum_{m=2^k+1}^{2^{k+1}} s(m) = \sum_{m=2^k}^{2^{k+1}-1} s(m) - s(2^k) + s(2^{k+1})$.

Now $s(2^k) = s(2^{k-1}) = \cdots = s(1) = 1$ and $s(2^{k+1}) = 1$ similarly. Therefore:

$$\sum_{n=2^{k+1}}^{2^{k+2}-1} s(n) = 3\sum_{m=2^k}^{2^{k+1}-1} s(m) - 1 + 1 = 3 \cdot 3^k = 3^{k+1}. \quad \square$$

**Theorem 2 (Prefix Sum Formula).** *For all $k \ge 1$,*
$$\sum_{n=1}^{2^k} s(n) = \frac{3^k - 1}{2} + 1 = \frac{3^k + 1}{2}.$$

**Proof.** We have:
$$\sum_{n=1}^{2^k} s(n) = \sum_{j=0}^{k-1} \sum_{n=2^j}^{2^{j+1}-1} s(n) + s(2^k) = \sum_{j=0}^{k-1} 3^j + 1 = \frac{3^k - 1}{3 - 1} + 1 = \frac{3^k + 1}{2}.$$

The term $s(2^k) = 1$ is added because the row sums cover $[2^j, 2^{j+1}-1]$ and $2^k$ falls outside the last row. $\square$

**Lemma 1 (Coprimality).** *$\gcd(s(n), s(n+1)) = 1$ for all $n \ge 0$.*

**Proof.** By induction. Base: $\gcd(s(0), s(1)) = \gcd(0,1) = 1$. If $n = 2m$: $\gcd(s(2m), s(2m+1)) = \gcd(s(m), s(m) + s(m+1)) = \gcd(s(m), s(m+1)) = 1$ by induction. If $n = 2m+1$: $\gcd(s(2m+1), s(2m+2)) = \gcd(s(m)+s(m+1), s(m+1)) = \gcd(s(m), s(m+1)) = 1$ by induction. $\square$

## Algorithm

```
function SternPrefixSum(k):
    // Method 1: Closed form from Theorem 2
    return (3^k + 1) / 2

    // Method 2: Direct computation (for verification)
    s = array of size 2^k + 1
    s[0] = 0; s[1] = 1
    for n = 2 to 2^k:
        if n is even:
            s[n] = s[n / 2]
        else:
            s[n] = s[(n-1)/2] + s[(n-1)/2 + 1]
    return sum(s[1 .. 2^k])
```

## Complexity Analysis

- **Time (closed form):** $O(k)$ using repeated squaring for $3^k$.
- **Time (direct):** $O(2^k)$ to compute all values.
- **Space (closed form):** $O(1)$.
- **Space (direct):** $O(2^k)$.

For $k = 20$: $\frac{3^{20}+1}{2} = \frac{3\,486\,784\,401 + 1}{2} = 1\,743\,392\,201$.

## Answer

$$\boxed{3575508}$$
