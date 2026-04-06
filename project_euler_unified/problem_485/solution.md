# Problem 485: Maximum Number of Divisors

## Problem Statement

Let $d(i)$ be the number of divisors of $i$. Define

$$M(n, k) = \max\bigl(d(n), d(n+1), \ldots, d(n+k-1)\bigr).$$

Given that $\sum_{n=1}^{100} M(n, 10) = 432$, find $\sum_{n=1}^{10^8} M(n, 10^5)$.

## Mathematical Foundation

**Theorem 1 (Divisor function via sieve).** For any positive integer $n$,

$$d(n) = \sum_{j \mid n} 1.$$

The values $d(1), d(2), \ldots, d(L)$ can be computed simultaneously by iterating: for each $j = 1, 2, \ldots, L$, increment $d[j], d[2j], d[3j], \ldots, d[\lfloor L/j \rfloor \cdot j]$ by 1.

**Proof.** By definition, $d(n)$ counts the number of positive divisors of $n$. The sieve procedure adds 1 to $d[n]$ exactly once for each divisor $j$ of $n$ (since $j \mid n$ iff $n$ appears in the arithmetic progression $j, 2j, 3j, \ldots$). The total work is $\sum_{j=1}^L \lfloor L/j \rfloor = O(L \ln L)$. $\square$

**Theorem 2 (Sliding window maximum via monotone deque).** Given an array $a[1 \ldots L]$ and window size $k$, the sequence of window maxima $M_i = \max(a[i], \ldots, a[i+k-1])$ for $i = 1, \ldots, L - k + 1$ can be computed in $O(L)$ amortized time using a monotone deque.

**Proof.** Maintain a deque $D$ of indices, invariant: $D$ is non-empty, indices are in increasing order, and $a[D[\text{front}]] \ge a[D[\text{front}+1]] \ge \cdots \ge a[D[\text{back}]]$ (weakly decreasing in $a$-values).

For each new index $i$:
1. While $D$ is non-empty and $a[D.\text{back}] \le a[i]$: pop from back.
2. Push $i$ to back.
3. While $D.\text{front} < i - k + 1$: pop from front (out of window).
4. The window maximum is $a[D.\text{front}]$.

*Correctness:* After step 1, all indices remaining in $D$ have $a$-values strictly greater than $a[i]$, and $i$ is appended. The front is always the index of the maximum value in the current window because: (a) all indices in $D$ are within the window (step 3 ensures this), and (b) any index $j < i$ with $a[j] \le a[i]$ was removed in step 1 (either when $i$ was processed or earlier), so the front has the largest $a$-value.

*Amortized $O(1)$:* Each index is pushed onto $D$ exactly once and popped at most once from the back and at most once from the front. Over $L$ iterations, the total number of push and pop operations is at most $3L$. $\square$

**Lemma 1 (Divisor count upper bound).** For $n \le 10^8 + 10^5$, we have $d(n) \le 768$. More precisely, the maximum value of $d(n)$ for $n \le 10^8$ is achieved at highly composite numbers.

**Proof.** By the theory of highly composite numbers (Ramanujan, 1915), the divisor function grows sub-polynomially. For $n \le 10^8$, an exhaustive sieve confirms $d(n) \le 768$. $\square$

## Algorithm

```
function SOLVE(N, k):
    L = N + k - 1

    // Step 1: Divisor count sieve
    d[1..L] = array of zeros
    for j = 1 to L:
        for multiple = j, 2j, 3j, ..., up to L:
            d[multiple] += 1

    // Step 2: Sliding window maximum via monotone deque
    deque = empty double-ended queue of indices
    total = 0

    for i = 1 to L:
        // Maintain decreasing invariant
        while deque is not empty and d[deque.back] <= d[i]:
            deque.pop_back()
        deque.push_back(i)

        // Remove out-of-window front
        while deque.front < i - k + 1:
            deque.pop_front()

        // Once window is full (i >= k), record the maximum
        if i >= k:
            total += d[deque.front]

    return total
```

## Complexity Analysis

- **Time:** $O(L \log L)$ for the divisor sieve (where $L = N + k - 1 \approx 1.0001 \times 10^8$), plus $O(L)$ for the sliding window. Total: $O(L \log L)$.
- **Space:** $O(L)$ for the divisor array, plus $O(k)$ for the deque. Total: $O(L)$.

For $N = 10^8$ and $k = 10^5$, $L \approx 10^8$, so the sieve requires approximately $10^8 \times \ln(10^8) \approx 1.8 \times 10^9$ operations and $\sim 400$ MB of memory (using 32-bit integers for $d$).

## Answer

$$\boxed{51281274340}$$
