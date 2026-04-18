# Problem 50: Consecutive Prime Sum

## Problem Statement

Which prime, below one million, can be written as the sum of the most consecutive primes?

## Mathematical Development

### Formal Development

**Definition 1 (Consecutive prime sum).** Let $p_1 = 2, p_2 = 3, p_3 = 5, \ldots$ denote the primes in ascending order. A *consecutive prime sum* of length $L$ starting at index $i$ is:
$$S(i, L) = \sum_{k=i}^{i+L-1} p_k.$$

We seek the prime $q < 10^6$ such that $q = S(i, L)$ for some $i$ and $L$, with $L$ maximized.

**Theorem 1 (Prefix sum formulation).** *Define $P_0 = 0$ and $P_n = \sum_{k=1}^{n} p_k$ for $n \geq 1$. Then $S(i, L) = P_{i+L-1} - P_{i-1}$, and each evaluation is $O(1)$ after $O(\pi(N))$ preprocessing.*

*Proof.* By telescoping:
$$S(i, L) = \sum_{k=i}^{i+L-1} p_k = \left(\sum_{k=1}^{i+L-1} p_k\right) - \left(\sum_{k=1}^{i-1} p_k\right) = P_{i+L-1} - P_{i-1}.$$

The prefix sums $P_0, P_1, \ldots, P_{\pi(N)}$ are computed in a single pass in $O(\pi(N))$ time. $\square$

**Lemma 1 (Maximum feasible length).** *Define $L_{\max} = \max\{L : P_L < N\}$. No consecutive prime sum of length exceeding $L_{\max}$ can be less than $N$.*

*Proof.* The sum $S(1, L) = P_L$ uses the $L$ smallest primes, so $S(1, L) \leq S(i, L)$ for all $i \geq 1$ (since $p_j$ is non-decreasing in $j$, replacing $p_k$ by $p_{k + (i-1)}$ for $k = 1, \ldots, L$ can only increase the sum). Therefore, if $P_L \geq N$, then $S(i, L) \geq P_L \geq N$ for all $i$, so no chain of length $L$ sums to a prime below $N$. $\square$

**Theorem 2 (Optimal search strategy).** *The following algorithm correctly finds the prime below $N$ that is the longest consecutive prime sum:*
1. *Compute $L_{\max}$ as in Lemma 1.*
2. *For $L = L_{\max}, L_{\max} - 1, \ldots, 1$: for each starting index $i \geq 1$ with $S(i, L) < N$, test whether $S(i, L)$ is prime.*
3. *Return the first prime found.*

*Proof of correctness.* The search proceeds in order of decreasing $L$. For a given $L$, if any $S(i, L)$ is prime (and $< N$), it is optimal among all chains of that exact length (we only need one). Since we try the largest $L$ first, the first prime found has maximal length. By Lemma 1, no chain longer than $L_{\max}$ exists, so the search space is complete.

*Proof of termination.* For $L = 1$, every starting prime $p_i < N$ is itself a valid consecutive prime sum of length 1, so the algorithm always terminates. $\square$

**Theorem 3.** *The prime below $10^6$ expressible as the longest consecutive prime sum is $997651$. It equals the sum of $543$ consecutive primes starting from $p_4 = 7$:*
$$997651 = \sum_{k=4}^{546} p_k = 7 + 11 + 13 + \cdots + 3931.$$

*Proof.* We apply the Sieve of Eratosthenes to generate all primes below $10^6$ (there are $\pi(10^6) = 78{,}498$ such primes). Prefix sums are computed per Theorem 1. The search of Theorem 2 finds:
- $L_{\max} = 546$ (since $P_{546} < 10^6$ and $P_{547} \geq 10^6$).
- For $L = 546, 545, 544$: no starting index yields a prime sum below $10^6$.
- For $L = 543$: starting at $i = 4$ (i.e., $p_4 = 7$), $S(4, 543) = P_{546} - P_3 = 997651$.

**Primality verification.** $997651$ is confirmed prime by the sieve (alternatively, $\lfloor\sqrt{997651}\rfloor = 998$, and trial division by all primes up to $998$ yields no factor).

**Optimality.** No chain of length $544, 545,$ or $546$ yields a prime below $10^6$, and $543$ is the first length for which a prime is found. $\square$

## Editorial

We sieve all primes below the limit and build prefix sums of the prime sequence, which makes every consecutive prime sum a constant-time difference of two prefixes. Candidate chain lengths are then examined in decreasing order; for each length, the starting index is advanced until the corresponding sum reaches the limit, and primality is checked by table lookup. Because the search proceeds from longer chains to shorter ones, the first prime found is the optimal answer.

## Pseudocode

```text
Algorithm: Prime Below the Limit Expressible as the Longest Consecutive Prime Sum
Require: A prime bound L.
Ensure: The prime below L that can be written as the sum of the longest run of consecutive primes.
1: Sieve all primes below L and form the prime sequence p_0, p_1, ... together with prefix sums P_m.
2: For candidate lengths ell, examined from longest to shortest, do:
3:     For each admissible start index s, compute X ← P_(s + ell) - P_s.
4:     If X < L and X is prime, return X.
```

## Complexity Analysis

**Proposition (Time complexity).** *The algorithm runs in $O(N \log \log N + \pi(N)^2)$ worst-case time, but with early termination, the practical complexity is much smaller.*

*Proof.* The sieve costs $O(N \log \log N)$. Prefix sum computation is $O(\pi(N))$. The nested search loop iterates over lengths from $L_{\max}$ downward. For each length $L$, the inner loop runs at most $\pi(N) - L + 1$ iterations, each costing $O(1)$ (sieve lookup). In the worst case, this gives $\sum_{L=1}^{L_{\max}} (\pi(N) - L + 1) = O(\pi(N)^2)$.

However, with early termination (stopping as soon as the first prime is found for any length $L$, and breaking when $L$ drops to `best_len`), the practical runtime is dominated by the sieve. For $N = 10^6$, the search terminates after examining only a few lengths near $L_{\max} = 546$. $\square$

**Proposition (Space complexity).** *$O(N)$ for the sieve, plus $O(\pi(N))$ for the prime list and prefix sums. Total: $O(N)$.*

## Answer

$$\boxed{997651}$$
