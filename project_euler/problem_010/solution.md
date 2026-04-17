# Problem 10: Summation of Primes

## Problem Statement

Compute $\displaystyle\sum_{\substack{p \text{ prime} \\ p < 2\,000\,000}} p$.

## Mathematical Development

### Sieve of Eratosthenes

**Definition 1.** The *Sieve of Eratosthenes* on $[0, N)$ is the following procedure:

1. Initialize a Boolean array $A[0 \ldots N-1]$ with every entry set to `true`.
2. Set $A[0] = A[1] = \mathrm{false}$.
3. For each integer $p$ from $2$ to $\lfloor \sqrt{N-1} \rfloor$, if $A[p] = \mathrm{true}$ then mark every multiple
   $$p^2, p^2 + p, p^2 + 2p, \ldots < N$$
   as composite by setting the corresponding array entries to `false`.

**Theorem 1 (Sieve correctness).** After the sieve terminates, $A[i] = \mathrm{true}$ if and only if $i$ is prime, for every integer $0 \le i < N$.

*Proof.* We prove both directions.

*Completeness.* Let $m \in [2,N-1]$ be composite. Then $m = ab$ for some integers $2 \le a \le b$. Hence

$$a \le \sqrt{m} \le \sqrt{N-1}.$$

Let $p$ be the smallest prime factor of $m$. Then $p \le a \le \sqrt{N-1}$. During iteration $p$, the sieve marks every multiple of $p$ starting at $p^2$. Since $m$ is a multiple of $p$ and $m = p(m/p) \ge p^2$, the entry $A[m]$ is marked `false`.

*Soundness.* Let $q \in [2,N-1]$ be prime. The only numbers marked during iteration $p$ are multiples of $p$. If $p < q$, then $p \nmid q$ because $q$ is prime. If $p = q$, then the marking starts at $q^2 > q$, so $q$ is not marked during its own iteration. Therefore no sieve step ever changes $A[q]$ from `true` to `false`.

The entries $A[0]$ and $A[1]$ are explicitly set to `false`, so the theorem follows. $\square$

**Theorem 2 (Summation correctness).** If

$$S = \sum_{\substack{0 \le i < N \\ A[i] = \mathrm{true}}} i,$$

then

$$S = \sum_{\substack{p < N \\ p \text{ prime}}} p.$$

*Proof.* By Theorem 1, the set of indices $i$ with $A[i] = \mathrm{true}$ is exactly the set of primes less than $N$. Summing over either description gives the same value. $\square$

## Algorithm

We use the Sieve of Eratosthenes to classify all integers below $N$ as prime or composite. After initializing the table, we traverse prime bases up to $\sqrt{N-1}$ and mark their multiples starting at $p^2$, then sum the indices that remain marked. This is sufficient because every composite below $N$ has a prime factor at most $\sqrt{N-1}$.

**Theorem 3 (Algorithm correctness).** `SumOfPrimesBelow(N)` returns the sum of all primes less than $N$.

*Proof.* The sieve phase constructs an array $A$ satisfying Theorem 1. The final loop sums exactly those indices $i$ for which $A[i] = \mathrm{true}$. By Theorem 2, that sum is precisely the sum of all primes less than $N$. $\square$

### Numerical Evaluation

**Proposition 1.** For $N = 2{,}000{,}000$, the algorithm returns

$$142{,}913{,}828{,}922.$$

*Proof.* Running the sieve on $[0,2{,}000{,}000)$ identifies exactly $148{,}933$ primes in this interval; the largest of them is $1{,}999{,}993$. Summing all marked prime indices yields

$$142{,}913{,}828{,}922.$$

By Theorem 3, this is exactly the required sum. $\square$

## Pseudocode

```text
Algorithm: Sum of Primes Below a Bound
Require: An integer N >= 2.
Ensure: S = ∑_{p < N, p prime} p.
1: Apply the Sieve of Eratosthenes on {2, 3, ..., N - 1}.
2: Compute S as the sum of all integers that remain marked prime after sieving.
3: Return S.
```

## Complexity Analysis

**Theorem 4 (Time complexity).** The algorithm runs in $O(N \log \log N)$ time.

*Proof.* The total number of marking operations satisfies

$$\sum_{\substack{p \le \sqrt{N} \\ p \text{ prime}}} \left\lfloor \frac{N-p^2}{p} \right\rfloor + 1
\le N \sum_{\substack{p \le N \\ p \text{ prime}}} \frac{1}{p}.$$

By Mertens' second theorem,

$$\sum_{p \le x} \frac{1}{p} = \ln \ln x + M + O\!\left(\frac{1}{\ln x}\right),$$

where $M$ is the Meissel-Mertens constant. Hence the sieve phase uses $O(N \log \log N)$ time. The final summation pass is linear, so the overall complexity remains $O(N \log \log N)$. $\square$

**Theorem 5 (Space complexity).** The algorithm uses $O(N)$ space.

*Proof.* The Boolean array $A$ has $N$ entries. The running sum and loop variables contribute only $O(1)$ additional space. For $N = 2{,}000{,}000$, a 64-bit signed integer is sufficient for the accumulator because

$$142{,}913{,}828{,}922 < 2^{63}.$$

Thus the total space usage is $O(N)$. $\square$

## Answer

$$\boxed{142913828922}$$
