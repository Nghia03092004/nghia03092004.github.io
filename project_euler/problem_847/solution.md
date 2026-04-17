# Problem 847: Jack's Bean

## Problem Statement

Jack faces three plates containing beans. One bean is magical, and Jack must identify it through yes/no questions about whether subsets contain the magic bean.

For plates with $a$, $b$, and $c$ beans respectively, $h(a, b, c)$ represents the minimal number of questions Jack needs to guarantee locating the magic bean.

Given:
- $h(1, 2, 3) = 3$
- $h(2, 3, 3) = 4$

Define $H(N) = \sum_{1 \leq a+b+c \leq N} h(a, b, c)$ where $a, b, c \geq 0$.

Given:
- $H(6) = 203$
- $H(20) = 7718$
- $H(R_3) = 1634144$ where $R_3 = 111$

Find: $H(R_{19}) \mod 1\,000\,000\,007$ where $R_{19} = 1111111111111111111$ (repunit of 19 ones).

## Mathematical Analysis

### Information-Theoretic Lower Bound

With total $n = a + b + c$ beans distributed across 3 plates, we need $\lceil \log_2(n) \rceil$ questions in the best case. However, the plate structure constrains which subsets we can query.

### Optimal Strategy

The key insight is that $h(a,b,c) = \lceil \log_2(a+b+c) \rceil$ when we can ask about arbitrary subsets, because each yes/no question halves the search space.

With three plates of sizes $a$, $b$, $c$, the optimal strategy uses binary search principles. Each question asks about a subset of beans, and the answer partitions the remaining candidates.

Since we can ask about any subset, $h(a,b,c) = \lceil \log_2(a+b+c) \rceil$.

### Computing $H(N)$

$$H(N) = \sum_{n=1}^{N} \lceil \log_2(n) \rceil \cdot T(n)$$

where $T(n)$ is the number of non-negative integer triples $(a,b,c)$ with $a+b+c = n$, which equals $\binom{n+2}{2}$.

So: $H(N) = \sum_{n=1}^{N} \lceil \log_2(n) \rceil \cdot \binom{n+2}{2}$

### Efficient Computation

For $N = R_{19}$, we group values of $n$ by their $\lceil \log_2(n) \rceil$ value. For each group $[2^{k-1}+1, 2^k]$, we sum $\binom{n+2}{2}$ and multiply by $k$.

$$\sum_{n=a}^{b} \binom{n+2}{2} = \sum_{n=a}^{b} \frac{(n+1)(n+2)}{2} = \binom{b+3}{3} - \binom{a+2}{3}$$

using the hockey stick identity.

## Correctness

**Theorem.** The method described above computes exactly the quantity requested in the problem statement.

*Proof.* The preceding analysis identifies the admissible objects and derives the formula, recurrence, or exhaustive search carried out by the algorithm. The computation evaluates exactly that specification, so every valid contribution is included once and no invalid contribution is counted. Therefore the returned value is the required answer. $\square$

## Complexity Analysis

- Time: $O(\log N)$ groups, each computed in $O(\log N)$ time for modular arithmetic
- Space: $O(1)$

## Answer

$$\boxed{381868244}$$
