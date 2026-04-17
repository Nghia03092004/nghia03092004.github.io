# Problem 441: The Inverse Summation of Coprime Couples

## Problem Statement

For an integer $M \geq 2$, define

$$R(M) = \sum_{\substack{1 \leq p < q \leq M \\ p + q \geq M \\ \gcd(p,q) = 1}} \frac{1}{pq}.$$

Let $S(N) = \sum_{M=2}^{N} R(M)$. Given that $S(2) = 1/2$, $S(10) \approx 6.9147$, and $S(100) \approx 58.2962$, find $S(10^7)$ rounded to 4 decimal places.

## Solution

### Theorem 1 (Contribution Counting)

**Statement.** For a coprime pair $(p,q)$ with $1 \leq p < q$, define the multiplicity

$$c(p,q; N) := \bigl|\{M \in \mathbb{Z} : 2 \leq M \leq N,\; q \leq M,\; p + q \geq M\}\bigr|.$$

Then

$$S(N) = \sum_{\substack{1 \leq p < q \\ \gcd(p,q) = 1}} \frac{1}{pq}\, c(p,q; N), \qquad c(p,q; N) = \max\bigl(0,\, \min(N, p+q) - q + 1\bigr).$$

**Proof.** The pair $(p,q)$ with $p < q$ contributes the term $\frac{1}{pq}$ to $R(M)$ precisely when the three conditions $1 \leq p < q \leq M$ and $p + q \geq M$ and $\gcd(p,q) = 1$ all hold. The first two conditions reduce to $q \leq M \leq p + q$. Since $q \geq 2$ (as $p \geq 1$ and $p < q$), every such $M$ satisfies $M \geq 2$ automatically. Intersecting the interval $[q, p+q]$ with $[2, N]$ yields $M \in [\,q,\, \min(N, p+q)\,]$. The number of integers in this interval is $\max(0, \min(N, p+q) - q + 1)$. Exchanging the order of summation -- summing first over pairs $(p,q)$, then over $M$ -- gives the stated formula. $\square$

### Lemma 1 (Full--Partial Decomposition)

**Statement.** For $N \geq 2$, every coprime pair $(p,q)$ with $p < q \leq N$ falls into exactly one of two cases:

1. **Full pairs** ($p + q \leq N$): The multiplicity is $c(p,q;N) = p + 1$.
2. **Partial pairs** ($p + q > N$): The multiplicity is $c(p,q;N) = N - q + 1$.

Consequently,

$$S(N) = \sum_{\substack{p < q \leq N \\ p + q \leq N \\ \gcd(p,q)=1}} \frac{p+1}{pq} \;+\; \sum_{\substack{p < q \leq N \\ p + q > N \\ \gcd(p,q)=1}} \frac{N - q + 1}{pq}.$$

**Proof.** When $p + q \leq N$, we have $\min(N, p+q) = p+q$, so $c = p + q - q + 1 = p + 1$. When $p + q > N$ but $q \leq N$, we have $\min(N, p+q) = N$, so $c = N - q + 1$. Pairs with $q > N$ do not contribute. $\square$

### Theorem 2 (Mobius Inversion)

**Statement.** Let $f : \mathbb{Z}_{>0}^2 \to \mathbb{R}$ be an arithmetic function of two variables. Then

$$\sum_{\substack{p,q \geq 1 \\ \gcd(p,q)=1}} f(p,q) = \sum_{d=1}^{\infty} \mu(d) \sum_{a,b \geq 1} f(da, db),$$

where $\mu$ is the Mobius function, provided the right-hand side converges absolutely.

**Proof.** The classical identity $[\gcd(p,q) = 1] = \sum_{d \mid \gcd(p,q)} \mu(d)$ (a consequence of Mobius inversion on the poset of divisors of $\gcd(p,q)$) allows us to write

$$\sum_{\substack{p,q \geq 1 \\ \gcd(p,q)=1}} f(p,q) = \sum_{p,q \geq 1} f(p,q) \sum_{d \mid \gcd(p,q)} \mu(d).$$

Interchanging summation (justified by absolute convergence) and substituting $p = da$, $q = db$ yields the result. $\square$

### Lemma 2 (Harmonic Reduction)

**Statement.** After applying Theorem 2 with the substitution $p = da'$, $q = dq'$, the inner sums factor through the harmonic numbers $H_k = \sum_{i=1}^{k} \frac{1}{i}$. Specifically, for fixed $d$ with $\mu(d) \neq 0$ and fixed $q'$, define $Q = \lfloor N/d \rfloor$ and $\beta = Q - q'$. Then:

- **Full-pair contribution** (summing over $p' = 1, \ldots, \min(q'-1, \beta)$):

$$\sum_{p'=1}^{P_f} \frac{dp' + 1}{d^2 p' q'} = \frac{P_f}{d\, q'} + \frac{H_{P_f}}{d^2\, q'}, \qquad P_f = \min(q'-1, \beta).$$

- **Partial-pair contribution** (summing over $p' = \max(1, \beta+1), \ldots, q'-1$):

$$\sum_{p'=P_s}^{q'-1} \frac{N - dq' + 1}{d^2 p' q'} = \frac{(N - dq' + 1)(H_{q'-1} - H_{P_s - 1})}{d^2\, q'}.$$

**Proof.** Under the substitution $p = dp'$, $q = dq'$, the term $\frac{1}{pq} = \frac{1}{d^2 p' q'}$. For full pairs, $dp' + dq' \leq N$ iff $p' \leq Q - q' = \beta$, and the multiplicity is $dp' + 1$. The sum $\sum_{p'=1}^{P_f} \frac{dp' + 1}{d^2 p' q'} = \frac{1}{dq'} \sum_{p'=1}^{P_f} 1 + \frac{1}{d^2 q'} \sum_{p'=1}^{P_f} \frac{1}{p'} = \frac{P_f}{dq'} + \frac{H_{P_f}}{d^2 q'}$. The partial-pair formula follows similarly, noting that $N - dq' + 1$ is constant with respect to $p'$. $\square$

## Algorithm

1. **Sieve.** Compute $\mu(d)$ for $d = 1, \ldots, N$ via a linear sieve.
2. **Harmonics.** Precompute $H_k = H_{k-1} + 1/k$ for $k = 0, 1, \ldots, N$ in floating point.
3. **Main loop.** For each $d$ with $\mu(d) \neq 0$, set $Q = \lfloor N/d \rfloor$. For each $q' = 2, \ldots, Q$:
   - Compute $P_f = \min(q'-1,\, Q - q')$. If $P_f \geq 1$, accumulate the full-pair contribution.
   - Set $P_s = \max(1,\, Q - q' + 1)$. If $P_s \leq q' - 1$, accumulate the partial-pair contribution via harmonic differences.
4. **Aggregate.** Multiply each $d$-contribution by $\mu(d)$ and sum.

## Complexity Analysis

- **Time:** $O(N \log N)$. The outer loop iterates over squarefree $d \leq N$. For each $d$, the inner loop runs $\lfloor N/d \rfloor - 1$ iterations. The total work is $\sum_{d=1}^{N} N/d = O(N \ln N)$.
- **Space:** $O(N)$ for the Mobius function and harmonic number arrays.

## Answer

$$\boxed{5000088.8395}$$
