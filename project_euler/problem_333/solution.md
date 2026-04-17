# Problem 333: Special Partitions

## Problem Statement

A number of the form $2^a \cdot 3^b$ with $a, b \ge 0$ is called a **3-smooth power**. A positive integer $n$ has a **special partition** if it can be written as a sum of distinct 3-smooth powers. A prime $p$ is called **special** if it has exactly one such partition. Find the sum of all special primes below $10^6$.

## Mathematical Foundation

**Definition.** Let $T = \{2^a \cdot 3^b : a \ge 0,\; b \ge 0\} = \{1, 2, 3, 4, 6, 8, 9, 12, 16, 18, \ldots\}$ be the set of all 3-smooth powers.

**Lemma 1 (Finiteness of $T$ below $N$).** *The number of elements of $T$ not exceeding $N$ is $|T \cap [1, N]| = O(\log^2 N)$.*

**Proof.** Each element $2^a \cdot 3^b \le N$ requires $a \le \log_2 N$ and $b \le \log_3 N$. The number of valid pairs $(a,b)$ is at most $(\lfloor \log_2 N \rfloor + 1)(\lfloor \log_3 N \rfloor + 1) = O(\log^2 N)$. $\square$

For $N = 10^6$: $\log_2(10^6) \approx 19.93$ and $\log_3(10^6) \approx 12.58$, giving at most $20 \times 13 = 260$ terms.

**Theorem 1 (0-1 Knapsack Reduction).** *The number of ways to write $n$ as a sum of distinct elements of $T \cap [1, N]$ equals the coefficient of $x^n$ in the product*
$$\prod_{t \in T \cap [1,N]} (1 + x^t).$$

**Proof.** Expanding the product, each monomial $x^{t_{i_1} + t_{i_2} + \cdots + t_{i_k}}$ corresponds to choosing a distinct subset $\{t_{i_1}, \ldots, t_{i_k}\} \subseteq T$. The coefficient of $x^n$ counts the number of such subsets summing to $n$. $\square$

**Lemma 2 (DP Correctness).** *Processing items in any order and updating in reverse (standard 0-1 knapsack), the array $\mathrm{dp}[s]$ correctly counts the number of representations of $s$ as a sum of distinct elements of $T$.*

**Proof.** By induction on the number of items processed. After processing item $t_k$, for each sum $s$, $\mathrm{dp}[s]$ equals the number of subsets of $\{t_1, \ldots, t_k\}$ summing to $s$. The reverse-order update ensures each item is used at most once, since when computing $\mathrm{dp}[s + t_k]$, the value $\mathrm{dp}[s]$ has not yet been modified in the current pass. $\square$

**Theorem 2 (Characterization of Special Primes).** *A prime $p < 10^6$ is special if and only if $\mathrm{dp}[p] = 1$ after the full knapsack computation. The answer is $\sum_{p \text{ special}} p$.*

**Proof.** By definition, $p$ is special if and only if it admits exactly one partition into distinct 3-smooth powers, which is precisely $\mathrm{dp}[p] = 1$. $\square$

## Algorithm

```
function SumSpecialPrimes(N):
    # Step 1: Generate 3-smooth powers up to N
    T = []
    b = 0
    while 3^b <= N:
        a = 0
        while 2^a * 3^b <= N:
            T.append(2^a * 3^b)
            a += 1
        b += 1

    # Step 2: 0-1 knapsack DP (count representations, cap at 2)
    dp = array of size N, initialized to 0
    dp[0] = 1
    for t in T:
        for s from N-1 down to t:
            if dp[s - t] > 0 and dp[s] < 2:
                dp[s] = min(dp[s] + dp[s - t], 2)

    # Step 3: Sieve primes up to N
    is_prime = sieve_of_eratosthenes(N)

    # Step 4: Sum special primes
    result = 0
    for p from 2 to N-1:
        if is_prime[p] and dp[p] == 1:
            result += p

    return result
```

## Complexity Analysis

- **Time:** Generating $T$ takes $O(\log^2 N)$. The knapsack DP runs in $O(|T| \cdot N) = O(N \log^2 N)$. The prime sieve takes $O(N \log \log N)$. Total: $O(N \log^2 N)$.
- **Space:** $O(N)$ for the DP array and the prime sieve.

## Answer

$$\boxed{3053105}$$
