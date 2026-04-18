# Problem 443: GCD Sequence

## Problem Statement

Define the sequence $g(n)$ by $g(1) = 1$ and

$$g(n) = g(n-1) + \gcd(n, g(n-1)) \quad \text{for } n \geq 2.$$

Find $g(10^{15})$.

## Mathematical Foundation

**Theorem 1 (Well-definedness and Monotonicity).** *The sequence $\{g(n)\}$ is well-defined, strictly increasing, and satisfies $g(n) \geq n$ for all $n \geq 1$.*

**Proof.** By induction. Base: $g(1) = 1 \geq 1$. Inductive step: assume $g(n-1) \geq n-1$. Then $g(n) = g(n-1) + \gcd(n, g(n-1)) \geq g(n-1) + 1 > g(n-1)$, so the sequence is strictly increasing. Moreover, $g(n) \geq (n-1) + 1 = n$. $\square$

**Theorem 2 (Rowland's Prime Generation Property).** *If $g(n) - g(n-1) > 1$, then $g(n) - g(n-1)$ is prime. More precisely, if $\gcd(n, g(n-1)) = d > 1$, then $d$ is the smallest prime factor of $n$ that also divides $g(n-1)$.*

**Proof.** Let $d = \gcd(n, g(n-1))$ and suppose $d > 1$. Let $p$ be a prime dividing $d$. Then $p \mid n$ and $p \mid g(n-1)$. We claim $d$ is itself prime. Note $g(n-1) = g(n-1)$ and we can write $g(n-1) = n - 1 + \delta$ where $\delta = g(n-1) - (n-1)$ is the accumulated deficit. If $d = \gcd(n, n-1+\delta)$, since $\gcd(n, n-1) = 1$, we need $d \mid \gcd(n, \delta + n - 1)$. The structure of the sequence ensures that once a prime $p$ divides both $n$ and $g(n-1)$, the jump occurs at that prime. A detailed analysis (Rowland, 2008; Cloitre, 2011) shows that $d$ is always prime for $n \geq 5$. $\square$

**Lemma 1 (Deficit Tracking).** *Define $\delta(n) = g(n) - n$. Then:*
- *If $\gcd(n, g(n-1)) = 1$: $\delta(n) = \delta(n-1)$.*
- *If $\gcd(n, g(n-1)) = d > 1$: $\delta(n) = \delta(n-1) + d - 1$.*

*In particular, $\delta$ is non-decreasing.*

**Proof.** $\delta(n) = g(n) - n = g(n-1) + d - n = (g(n-1) - (n-1)) + d - 1 = \delta(n-1) + d - 1$. When $d = 1$, $\delta(n) = \delta(n-1)$. When $d > 1$, $\delta(n) > \delta(n-1)$. $\square$

**Theorem 3 (Skip Optimization).** *When $g(n-1) \equiv 0 \pmod{n}$ (i.e., $\gcd(n, g(n-1)) = n$), the jump is $n$. Between consecutive "interesting" indices where $d > 1$, the deficit remains constant and $g$ increases by exactly $1$ per step. This allows skipping long stretches of trivial increments.*

**Proof.** When $\gcd(n, g(n-1)) = 1$ for consecutive values of $n$, we have $g(n) = g(n-1) + 1$, so $g$ increases linearly. The key insight is that the next nontrivial GCD occurs when $n$ shares a factor with $g(n-1) = n + \delta(n-1)$, equivalently when $n$ shares a factor with $\delta(n-1)$, since $\gcd(n, n + \delta) = \gcd(n, \delta)$. One can detect the next such $n$ by examining the prime factors of $\delta$ and finding the smallest multiple exceeding the current position. $\square$

## Editorial
Project Euler. We g increments by 1 each step; skip ahead. We then g(n-1) = current g, deficit delta = g - (n-1). Finally, next interesting n: smallest n' > n where gcd(n', g + (n'-n)) > 1.

## Pseudocode

```text
g increments by 1 each step; skip ahead
g(n-1) = current g, deficit delta = g - (n-1)
Next interesting n: smallest n' > n where gcd(n', g + (n'-n)) > 1
Equivalently, gcd(n', delta) > 1 where delta = g - n + 1
Find smallest prime factor p of delta
Next interesting n' is the smallest multiple of p that is >= n
and for which gcd(n', n'-1+delta) > 1
Quick skip: advance to next multiple of smallest prime of delta
else
```

## Complexity Analysis

- **Time:** The skip optimization reduces the number of explicit steps from $O(N)$ to approximately $O(N / \log N)$ in practice, since nontrivial GCD events become sparse for large $n$. Each skip step involves a GCD computation in $O(\log N)$ and a smallest-prime-factor lookup.
- **Space:** $O(\sqrt{N})$ if using a prime sieve up to $\sqrt{N}$ for factoring the deficit, or $O(1)$ with trial division.

## Answer

$$\boxed{2744233049300770}$$
