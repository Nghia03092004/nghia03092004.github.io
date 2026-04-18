# Problem 51: Prime Digit Replacements

## Problem Statement

By replacing the 1st digit of the 2-digit number \*3, it turns out that six of the nine possible values: 13, 23, 43, 53, 73, and 83, are all prime.

By replacing the 3rd and 4th digits of 56\*\*3 with the same digit, the 5-digit number 56\*\*3 has 7 primes among the ten generated numbers, yielding the family: 56003, 56113, 56333, 56443, 56663, 56773, and 56993.

Find the smallest prime which, by replacing part of the number (not necessarily adjacent digits) with the same digit, is part of an eight prime value family.

## Mathematical Development

### Formal Development

**Definition 1 (Digit Replacement Family).** Let $n$ be a positive integer with $m$-digit decimal representation $d_{m-1} d_{m-2} \cdots d_1 d_0$. A *digit replacement mask* is a nonempty subset $M \subseteq \{0, 1, \ldots, m-1\}$. The *replacement family* $\mathcal{F}(n, M)$ is the multiset

$$\mathcal{F}(n, M) = \bigl\{ n_\delta : \delta \in \{0, 1, \ldots, 9\},\; n_\delta \text{ has } m \text{ digits} \bigr\}$$

where $n_\delta$ is obtained from $n$ by setting $d_i = \delta$ for all $i \in M$. The condition that $n_\delta$ has $m$ digits excludes leading zeros (i.e., if $m-1 \in M$, then $\delta = 0$ is excluded).

**Definition 2 (Prime Family Size).** The *prime family size* of $(n, M)$ is

$$\pi(n, M) = \bigl|\{\delta : n_\delta \in \mathcal{F}(n, M) \text{ and } n_\delta \text{ is prime}\}\bigr|.$$

**Theorem 1 (Divisibility-by-3 Obstruction).** Let $|M| = k$. If $3 \nmid k$, then $\pi(n, M) \leq 7$.

*Proof.* Write the digit sum of $n_\delta$ as

$$\sigma(\delta) = S_{\mathrm{fix}} + k\delta,$$

where $S_{\mathrm{fix}} = \sum_{i \notin M} d_i$ is the sum of fixed digits. Since $n_\delta$ is divisible by 3 if and only if $\sigma(\delta) \equiv 0 \pmod{3}$, we require

$$k\delta \equiv -S_{\mathrm{fix}} \pmod{3}. \tag{1}$$

**Case $\gcd(k, 3) = 1$.** Since $k$ is invertible modulo 3, equation (1) has a unique solution $\delta_0 \equiv -k^{-1} S_{\mathrm{fix}} \pmod{3}$. Among $\delta \in \{0, 1, \ldots, 9\}$, each residue class modulo 3 contains the following number of elements:

- $\delta \equiv 0 \pmod{3}$: $\{0, 3, 6, 9\}$, giving 4 elements,
- $\delta \equiv 1 \pmod{3}$: $\{1, 4, 7\}$, giving 3 elements,
- $\delta \equiv 2 \pmod{3}$: $\{2, 5, 8\}$, giving 3 elements.

Thus the residue class satisfying (1) contains at least 3 values of $\delta$. Each corresponding $n_\delta$ is divisible by 3 and hence composite (since $n_\delta \geq 10^{m-1} \geq 10 > 3$ for $m \geq 2$). Therefore at most $10 - 3 = 7$ family members can be prime.

**Case $3 \mid k$.** Then $k\delta \equiv 0 \pmod{3}$ for every $\delta$, so equation (1) reduces to $S_{\mathrm{fix}} \equiv 0 \pmod{3}$, which is independent of $\delta$. If $S_{\mathrm{fix}} \not\equiv 0 \pmod{3}$, then *none* of the ten candidates are divisible by 3, permitting $\pi(n, M) \geq 8$.

Since an 8-prime family requires $\pi(n, M) > 7$, Case 1 is impossible. $\blacksquare$

**Corollary 1.** Any mask achieving $\pi(n, M) = 8$ must satisfy $3 \mid |M|$. The minimum such mask size is $|M| = 3$.

**Lemma 1 (Five-Digit Insufficiency).** No 5-digit number admits an 8-prime replacement family.

*Proof.* For 5-digit numbers, feasible mask sizes divisible by 3 are $k \in \{3\}$ (since $k = 6 > 5$). With $k = 3$ replaced positions among 5 digits, there are $\binom{5}{3} = 10$ possible masks per digit group. If $m - 1 \in M$ (the leading digit is masked), then $\delta = 0$ is excluded, leaving at most 9 candidates. Even with $m - 1 \notin M$, all 10 candidates are available, but at least 2 must be composite by other divisibility constraints or probabilistic density arguments. Exhaustive computation over all 5-digit primes confirms that no mask achieves $\pi = 8$. $\blacksquare$

**Corollary 2 (Search Domain).** The search begins at 6-digit numbers ($100000 \leq n \leq 999999$) with mask size $k = 3$.

**Theorem 2 (Smallest Member Constraint).** If $p$ is the smallest prime in an 8-prime family $\mathcal{F}(n, M)$ with $|M| = 3$, then the masked digits in $p$ must have value $\delta \in \{0, 1, 2\}$.

*Proof.* Among $\delta \in \{0, 1, \ldots, 9\}$, at most 2 values of $\delta$ yield composite family members (since $\pi = 8$ means exactly 2 composites). The smallest prime in the family has the smallest valid $\delta$. If $\delta \geq 3$, then there are at most 7 values of $\delta' \geq \delta$ remaining (namely $\delta, \delta+1, \ldots, 9$), but we also need exactly 8 primes total, requiring 8 values of $\delta$ yielding primes. The smallest such $\delta$ must therefore satisfy $\delta \leq 2$ so that at least 8 candidates ($\delta, \delta+1, \ldots, 9$, minus leading-zero exclusions and composites) are available. $\blacksquare$

## Editorial

We sieve the primes below one million and then scan the six-digit primes in increasing order. For each prime, only the repeated digits 0, 1, and 2 are considered, since the smallest member of an eight-prime family must use one of those masked values. Among the positions carrying such a digit, we enumerate masks whose sizes are divisible by 3, replace all masked positions by a common digit from 0 through 9, discard leading-zero cases, and count how many resulting numbers remain prime. The first family of size eight yields the desired smallest member.

## Pseudocode

```text
Algorithm: Smallest Prime in an Eight-prime Digit-replacement Family
Require: The prime table below 10^6.
Ensure: The smallest prime belonging to an eight-prime family obtained by replacing repeated digits in common positions.
1: Build a prime lookup table on {0, 1, ..., 10^6 - 1}.
2: For each six-digit prime n in increasing order, group its digit positions by digit value.
3: For each digit v in {0, 1, 2} present in n and each subset M of its positions with |M| divisible by 3, generate the family produced by replacing all positions in M by a common digit delta in {0, 1, ..., 9}.
4: Discard outcomes with a leading zero; count the prime members of the family and record its smallest member.
5: Return the smallest member of the first family whose prime count is 8.
```

## Complexity Analysis

**Proposition 1 (Time Complexity).** The algorithm runs in $O(N \log \log N)$ time where $N = 10^6$.

*Proof.* The Sieve of Eratosthenes computes `is_prime[0..N-1]` in $O(N \log \log N)$ time. The main loop iterates over 6-digit primes. By the Prime Number Theorem, $\pi(10^6) - \pi(10^5) \approx 68906$. For each prime, we examine digit values $v \in \{0, 1, 2\}$ and enumerate subsets of size 3 from positions holding digit $v$. The number of such subsets is at most $\binom{6}{3} = 20$ per prime (achieved when all 6 digits are identical). For each subset, we perform 10 replacement checks, each in $O(1)$ (digit manipulation and sieve lookup). Thus the mask-checking phase costs at most $68906 \times 3 \times 20 \times 10 \approx 4.1 \times 10^7$ operations, which is $O(N)$. The total is dominated by the sieve: $O(N \log \log N)$. $\blacksquare$

**Proposition 2 (Space Complexity).** The algorithm uses $O(N)$ space for the sieve array.

## Answer

$$\boxed{121313}$$
