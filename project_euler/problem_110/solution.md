# Problem 110: Diophantine Reciprocals II

## Problem Statement

Find the least value of $n$ such that the number of distinct solutions of

$$\frac{1}{x} + \frac{1}{y} = \frac{1}{n}$$

exceeds four million (4,000,000).

## Mathematical Foundation

**Theorem 1.** *(Algebraic Transformation -- same as Problem 108)* The equation $\frac{1}{x} + \frac{1}{y} = \frac{1}{n}$ with positive integers $x, y$ is equivalent to $(x-n)(y-n) = n^2$, and the number of solutions with $x \leq y$ is $f(n) = \lceil d(n^2)/2 \rceil$.

**Proof.** See Problem 108, Theorems 1 and 2. $\square$

**Theorem 2.** *(Divisor Count of a Square)* If $n = p_1^{a_1} p_2^{a_2} \cdots p_k^{a_k}$, then $d(n^2) = \prod_{i=1}^{k}(2a_i + 1)$.

**Proof.** See Problem 108, Theorem 3. $\square$

**Theorem 3.** *(Required Bound)* We need $f(n) > 4{,}000{,}000$, i.e., $\lceil d(n^2)/2 \rceil > 4{,}000{,}000$.

Since $d(n^2) = \prod(2a_i + 1)$ is always odd, $\lceil d(n^2)/2 \rceil = (d(n^2) + 1)/2$. The condition $(d(n^2) + 1)/2 > 4{,}000{,}000$ gives $d(n^2) > 7{,}999{,}999$, so $d(n^2) \geq 8{,}000{,}001$ (since $d(n^2)$ is odd).

**Proof.** $d(n^2)$ is a product of odd numbers $(2a_i + 1)$, hence odd. For odd $m$, $\lceil m/2 \rceil = (m+1)/2$. The inequality $(m+1)/2 > 4{,}000{,}000$ gives $m > 7{,}999{,}999$, and since $m$ is odd, $m \geq 8{,}000{,}001$. $\square$

**Theorem 4.** *(Optimization as a Discrete Minimization Problem)* Minimize $n = \prod p_i^{a_i}$ (equivalently, minimize $\log n = \sum a_i \log p_i$) subject to $\prod(2a_i + 1) \geq 8{,}000{,}001$ and $a_1 \geq a_2 \geq \cdots \geq a_k \geq 1$, where $p_i$ is the $i$-th prime.

**Proof.** The constraint $a_1 \geq a_2 \geq \cdots \geq a_k$ is necessary for optimality by the same argument as Problem 108 Lemma 1: swapping a larger exponent to a smaller prime strictly decreases $n$ while preserving $d(n^2)$. $\square$

**Lemma 1.** *(Search Space Bounds)* The number of primes $k$ satisfies $k \leq \lfloor \log_3(8{,}000{,}001) \rfloor = 14$, and the maximum exponent satisfies $a_1 \leq \lfloor (8{,}000{,}001 - 1)/2 \rfloor$ but in practice $a_1 \leq 14$ suffices (since $3^{14} = 4{,}782{,}969 < 8{,}000{,}001 < 3^{15}$, using more primes with exponent 1 is more efficient than increasing one exponent).

**Proof.** With $k$ primes all having exponent $\geq 1$, $d(n^2) \geq 3^k$. For $3^k \geq 8{,}000{,}001$, we need $k \geq 15$. However, some primes may have exponent $> 1$, so $k$ can be smaller. The bound $k \leq 14$ follows from $\log_3(8{,}000{,}001) \approx 14.5$. For the maximum exponent: if $a_1 = m$ and all other exponents are 1, then $d(n^2) = (2m+1) \cdot 3^{k-1}$, and it suffices to have $m$ small with $k$ large. The practical bound $a_1 \leq 14$ is sufficient. $\square$

**Theorem 5.** *(Solution)* The minimum $n$ is:

$$n = 9350130049860600 = 2^3 \cdot 3^3 \cdot 5^2 \cdot 7^2 \cdot 11 \cdot 13 \cdot 17 \cdot 19 \cdot 23 \cdot 29 \cdot 31 \cdot 37$$

with exponent sequence $(3, 3, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1)$ giving:

$$d(n^2) = 7 \cdot 7 \cdot 5 \cdot 5 \cdot 3^8 = 49 \cdot 25 \cdot 6561 = 8{,}037{,}225$$

and $f(n) = \lceil 8{,}037{,}225/2 \rceil = 4{,}018{,}613 > 4{,}000{,}000$.

**Proof.** The exponent sequence $(3, 3, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1)$ is non-increasing with 12 primes. The divisor product is $7^2 \cdot 5^2 \cdot 3^8 = 8{,}037{,}225 \geq 8{,}000{,}001$. Assigning exponents to the first 12 primes in decreasing order: $2^3 \cdot 3^3 \cdot 5^2 \cdot 7^2 \cdot 11 \cdot 13 \cdot 17 \cdot 19 \cdot 23 \cdot 29 \cdot 31 \cdot 37 = 9{,}350{,}130{,}049{,}860{,}600$.

To verify minimality: any alternative exponent sequence with $\prod(2a_i+1) \geq 8{,}000{,}001$ yields $n \geq 9{,}350{,}130{,}049{,}860{,}600$. This is confirmed by exhaustive search over all feasible non-increasing exponent sequences (bounded by $k \leq 15$ primes and $a_1 \leq 14$), computing $n$ for each and retaining the minimum. $\square$

## Editorial
This is the same divisor-count minimization as Problem 108, but the target is now so large that comparing candidate integers directly during the search becomes awkward. The structure of the optimal solution is unchanged: we still assign a non-increasing exponent sequence to successive primes, because moving a larger exponent onto a smaller prime always makes $n$ smaller.

The implementation therefore performs the same recursive search over exponent patterns, but it compares candidates in log-space. Tracking $\log n = \sum a_i \log p_i$ preserves the ordering of candidate values while avoiding overflow during recursion. Once the best exponent pattern has been identified, the actual integer $n$ is reconstructed from those stored exponents.

## Pseudocode

```text
Store the primes and their logarithms, and begin with no exponents chosen.
Recursively assign an exponent to each successive prime, never exceeding the previous exponent.

At each recursive call:
    If the current divisor product has reached 8,000,001, compare the current log(n) with the best one seen so far and keep the better exponent pattern.
    Otherwise try the next exponent values in order, update both the divisor product and log(n), and abandon any branch that is already no better than the current best.
    After returning from a branch, remove the exponent and continue with the next choice.

After the search finishes, rebuild n from the stored best exponents and return it.
```

## Complexity Analysis

- **Time**: The recursive search explores non-increasing exponent sequences with aggressive pruning ($n < \text{best\_n}$ and $\prod(2a_i+1) \geq 8{,}000{,}001$). The number of feasible sequences is bounded by the number of partitions of $\lceil\log_3(8{,}000{,}001)\rceil \approx 15$ into at most 15 parts, weighted by decreasing exponent constraints. In practice, the search visits on the order of $10^4$ nodes and completes in milliseconds.
- **Space**: $O(k)$ for the recursion stack, where $k \leq 15$.

## Answer

$$\boxed{9350130049860600}$$
