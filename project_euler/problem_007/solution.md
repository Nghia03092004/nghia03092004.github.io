# Problem 7: 10001st Prime

## Problem Statement

Determine the 10001st prime number, where primes are indexed as $p_1 = 2, p_2 = 3, p_3 = 5, \ldots$

## Mathematical Development

### Definitions

**Definition 1.** A natural number $n \ge 2$ is *prime* if its only positive divisors are 1 and $n$. A natural number $n \ge 2$ that is not prime is *composite*.

**Definition 2.** The *prime-counting function* $\pi(x)$ denotes the number of primes $p$ satisfying $p \le x$. We write $p_n$ for the $n$-th smallest prime.

### Upper Bound on $p_n$

**Theorem 1 (Prime Number Theorem).** As $x \to \infty$,
$$\pi(x) \sim \frac{x}{\ln x}.$$
Equivalently, $p_n \sim n \ln n$ as $n \to \infty$.

**Theorem 2 (Rosser, 1941).** For every integer $n \ge 6$,
$$p_n < n(\ln n + \ln \ln n).$$

*Reference.* J. B. Rosser, "Explicit bounds for some functions of prime numbers," *Amer. J. Math.* **63** (1941), 211--232. The proof relies on explicit bounds for the Chebyshev function $\theta(x) = \sum_{p \le x} \ln p$ and is taken here as established.

**Proposition 1 (Sieve bound for $n = 10001$).** The 10001st prime satisfies $p_{10001} < 114320$.

*Proof.* Since $10001 \ge 6$, Theorem 2 applies. We compute:
- $\ln(10001) = 9.21044\ldots$
- $\ln(\ln(10001)) = \ln(9.21044\ldots) = 2.22069\ldots$
- $10001 \times (9.21044 + 2.22069) = 10001 \times 11.43113\ldots = 114319.4\ldots$

Hence $p_{10001} < 114320$. We set the sieve limit $N = 115000$ to provide a safety margin. $\square$

### Correctness of the Sieve of Eratosthenes

**Theorem 3 (Sieve of Eratosthenes).** Let $N \ge 2$ be an integer. Define the sieve procedure:
1. Initialize a Boolean array $A[0 \ldots N]$ with $A[i] = \mathrm{true}$ for all $i$.
2. Set $A[0] = A[1] = \mathrm{false}$.
3. For each integer $p$ from $2$ to $\lfloor \sqrt{N} \rfloor$: if $A[p] = \mathrm{true}$, set $A[kp] = \mathrm{false}$ for every integer $k \ge p$ with $kp \le N$.

Then for every integer $i \in [2, N]$, $A[i] = \mathrm{true}$ if and only if $i$ is prime.

*Proof.* We establish both directions.

**Claim A (every composite is marked).** Let $m \in [2, N]$ be composite. Write $m = ab$ with $2 \le a \le b$. Then $a \le \sqrt{m} \le \sqrt{N}$. Let $p$ be the smallest prime factor of $m$; then $p \le a \le \sqrt{N}$. At iteration $p$ of the sieve, all multiples of $p$ starting from $p^2$ are marked false. Since $m$ is a multiple of $p$ and $m \ge pa \ge p^2$, the entry $A[m]$ is set to false.

**Claim B (no prime is marked).** Let $q \in [2, N]$ be prime. In step 3, $A[q]$ could only be set to false during the iteration for some prime $p \le \lfloor \sqrt{N} \rfloor$ with $p \ne q$. But marking begins at $p^2$, and the values marked are $p^2, p^2 + p, p^2 + 2p, \ldots$, all of which are multiples of $p$. Since $q$ is prime and $q \ne p$, we have $p \nmid q$, so $q$ is never marked. $\square$

**Corollary 1.** Running the Sieve of Eratosthenes up to $N = 115000$ and extracting the $n$-th entry equal to true (counting from $i = 2$) yields $p_n$ for any $n$ with $p_n \le N$. By Proposition 1, this includes $n = 10001$.

## Editorial

The algorithm first chooses a proven upper bound that is guaranteed to contain the $n$-th prime. It then runs the Sieve of Eratosthenes on $[2, N]$, marks every composite starting from $p^2$, and performs a final increasing scan through the sieve until the $n$-th marked prime is reached. This is sufficient because the bound contains $p_n$ and the sieve leaves exactly the primes unmarked.

**Theorem 5 (Algorithm correctness).** `NthPrime(n)` returns the $n$-th prime number for every $n \ge 1$.

*Proof.* If $n < 6$, then the algorithm uses the explicit bound $N = 20$, and the primes up to 20 are

$$2, 3, 5, 7, 11, 13, 17, 19,$$

so the $n$-th prime certainly lies in $[2,20]$.

If $n \ge 6$, the algorithm sets

$$N = \left\lceil n(\ln n + \ln \ln n)\right\rceil + 100.$$

By Theorem 2, $p_n < n(\ln n + \ln \ln n) \le N$, so again $p_n \le N$.

By Theorem 3, after the sieve phase the entries marked `true` are exactly the primes in $[2,N]$. The final scan visits these integers in strictly increasing order and increments `count` once for each prime encountered. Therefore when `count == n`, the current value is exactly the $n$-th prime. Hence the returned value is $p_n$. $\square$

### Numerical Evaluation

**Proposition 2.** The algorithm returns

$$p_{10001} = 104743.$$

*Proof.* By Proposition 1, it is enough to sieve up to $N = 115000$. Running the algorithm on this interval shows:

- there are exactly $10000$ primes strictly less than $104743$;
- there are exactly $10001$ primes less than or equal to $104743$.

Therefore $104743$ is the $10001$-st prime. $\square$

## Pseudocode

```text
Algorithm: nth Prime by Sieve
Require: An integer n ≥ 1.
Ensure: The n-th prime number.
1: Choose an upper bound U that is guaranteed to contain the n-th prime.
2: Build a sieve on {2, 3, ..., U}, marking composites from p^2 onward for each prime base p.
3: Scan the surviving primes in increasing order, maintaining a counter c.
4: When c = n, return the current prime.
```

## Complexity Analysis

**Theorem 4 (Sieve complexity).** The Sieve of Eratosthenes on $[0, N]$ runs in $O(N \log \log N)$ time and $O(N)$ space.

*Proof.* **Time.** The total number of marking operations is
$$\sum_{\substack{p \le \sqrt{N} \\ p \text{ prime}}} \left\lfloor \frac{N}{p} \right\rfloor \le \sum_{\substack{p \le N \\ p \text{ prime}}} \frac{N}{p} = N \sum_{\substack{p \le N \\ p \text{ prime}}} \frac{1}{p}.$$
By Mertens' second theorem (1874), $\sum_{p \le x} 1/p = \ln \ln x + M + O(1/\ln x)$, where $M \approx 0.2615$ is the Meissel--Mertens constant. Hence the sum is $O(N \log \log N)$. The subsequent linear scan to locate $p_n$ costs $O(N)$, which is dominated.

**Space.** The Boolean array has $N + 1$ entries, giving $O(N)$ space. $\square$

**Corollary 2.** With $N = O(n \log n)$ by Theorem 2, the overall time complexity for finding $p_n$ is $O(n \log n \cdot \log \log(n \log n))$ and space complexity is $O(n \log n)$.

## Answer

$$\boxed{104743}$$
