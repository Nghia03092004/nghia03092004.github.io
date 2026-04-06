# Problem 615: The Millionth Number with at Least One Million Prime Factors

## Problem Statement

Define $D(n)$ as the number of ways to write $n$ as a product of powers of 2 alone — equivalently, $D(n) = v_2(n) + 1$ counts the choices $2^0, 2^1, \ldots, 2^{v_2(n)}$. More precisely (per the actual Project Euler problem), we define the *factorisation richness*: a number $n$ is **factor-rich** if the total count of prime factors with multiplicity satisfies $\Omega(n) \ge 10^6$. We seek the millionth smallest such number modulo $982451653$.

## Mathematical Analysis

### The $\Omega$ Function and Smooth Numbers

For a positive integer $n = p_1^{a_1} p_2^{a_2} \cdots p_k^{a_k}$, the number-of-prime-factors-with-multiplicity function is:

$$\Omega(n) = a_1 + a_2 + \cdots + a_k \tag{1}$$

The smallest number with $\Omega(n) \ge K$ is $2^K$, the next is $2^{K-1} \cdot 3$, and so on. All such numbers are **$K$-almost primes or higher** — products of at least $K$ primes (with repetition).

### Ordering by Logarithm

Since we seek the $M$-th smallest number with $\Omega(n) \ge K$, we use the key observation:

$$\log n = a_1 \log 2 + a_2 \log 3 + a_3 \log 5 + \cdots \tag{2}$$

subject to $a_1 + a_2 + a_3 + \cdots \ge K$ and each $a_i \ge 0$. Sorting by $\log n$ is equivalent to sorting by $n$.

### Priority Queue Approach

We enumerate tuples $(a_1, a_2, a_3, \ldots)$ in increasing order of $\sum a_i \log p_i$ with the constraint $\sum a_i \ge K$. This is done with a min-heap keyed by the log-value:

1. Start with $(K, 0, 0, \ldots)$ — the number $2^K$.
2. Extract the minimum; record it.
3. Generate successors: replace one factor of $p_i$ with a factor of $p_{i+1}$ (i.e., decrease $a_i$ by 1 and increase $a_{i+1}$ by 1), which increases the log-value since $\log p_{i+1} > \log p_i$.
4. Also add one extra factor of the current largest prime.

### Avoiding Duplicates

To avoid generating the same tuple twice, enforce a canonical ordering: successors of $(a_1, \ldots, a_k)$ are generated only by:
- Transferring from prime $p_i$ to $p_{i+1}$ for the *largest active* index.
- This ensures each composition is generated exactly once.

### Concrete Examples (small $K$)

For $K = 3$ (at least 3 prime factors with multiplicity), the sequence begins:

| Rank | $n$ | Factorization | $\Omega(n)$ |
|------|-----|---------------|-------------|
| 1 | 8 | $2^3$ | 3 |
| 2 | 12 | $2^2 \cdot 3$ | 3 |
| 3 | 16 | $2^4$ | 4 |
| 4 | 18 | $2 \cdot 3^2$ | 3 |
| 5 | 20 | $2^2 \cdot 5$ | 3 |
| 6 | 24 | $2^3 \cdot 3$ | 4 |
| 7 | 27 | $3^3$ | 3 |
| 8 | 28 | $2^2 \cdot 7$ | 3 |
| 9 | 30 | $2 \cdot 3 \cdot 5$ | 3 |
| 10 | 32 | $2^5$ | 5 |

## Derivation

### Algorithm 1: Heap-Based Enumeration

For the full problem with $K = 10^6$ and $M = 10^6$:

1. Represent each candidate as an exponent vector $(a_1, a_2, \ldots)$ where $p_i$ is the $i$-th prime.
2. Since $\sum a_i \ge 10^6$ and we want the million smallest, most candidates have $a_1$ close to $10^6$ with at most a few non-trivial higher prime factors.
3. The heap size stays manageable because replacing 2's with larger primes quickly inflates $\log n$.

### Algorithm 2: Binary Search on Log-Threshold

Alternatively, binary search on $L$: "how many numbers with $\Omega(n) \ge K$ satisfy $\log n \le L$?" If we can count this efficiently (via dynamic programming on the prime decomposition), we can pinpoint the $M$-th element.

### Modular Computation

The answer is $n \bmod 982451653$. Since $n = 2^{a_1} \cdot 3^{a_2} \cdots$, we compute:

$$n \bmod p = \prod_i \text{pow}(p_i, a_i, p) \bmod p$$

using fast modular exponentiation.

## Proof of Correctness

**Theorem.** *Every number with $\Omega(n) \ge K$ is uniquely represented by its prime exponent vector, and the heap enumeration visits them in increasing order of $n$.*

*Proof.* The fundamental theorem of arithmetic gives the unique representation. The heap key $\sum a_i \log p_i = \log n$ is a faithful order-preserving map. The successor generation covers all vectors with $\sum a_i \ge K$ reachable from the initial vector $(K, 0, 0, \ldots)$ by the transfer operations (decrease $a_i$, increase $a_{i+1}$ for adjacent primes), which form a connected graph on the set of valid vectors. $\square$

**Lemma.** *The number of candidates with $n \le 2^K \cdot (3/2)^M$ is at least $M$.*

*Proof.* Starting from $2^K$, each of the first $M$ transfers replaces at most one factor of 2 with a factor of 3, multiplying $n$ by at most $3/2$. So the $M$-th candidate satisfies $n \le 2^K \cdot (3/2)^M$. $\square$

## Complexity Analysis

- **Heap operations:** $O(M \log M)$ where $M = 10^6$.
- **Modular exponentiation:** $O(\log K)$ for the final answer.
- **Space:** $O(M)$ for the heap and visited set.

## Answer

$$\boxed{108424772}$$
