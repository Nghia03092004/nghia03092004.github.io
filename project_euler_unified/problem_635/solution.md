# Problem 635: Subset Sums

## Problem Statement

Given a multiset of integers, count the number of subsets whose elements sum to a specified target $T$. Compute this modulo a given prime.

## Mathematical Analysis

### Generating Functions

The number of subsets summing to $T$ equals the coefficient of $x^T$ in the product:

$$\prod_{a \in S} (1 + x^a) \tag{1}$$

For a multiset with element $a_i$ appearing $c_i$ times:

$$\prod_i (1 + x^{a_i} + x^{2a_i} + \cdots + x^{c_i a_i}) = \prod_i \frac{1 - x^{(c_i+1)a_i}}{1 - x^{a_i}} \tag{2}$$

### Dynamic Programming

Standard DP: $f[s]$ = number of subsets summing to $s$. Transition:

$$f[s] \mathrel{+}= f[s - a] \quad \text{for each element } a \tag{3}$$

Process elements one by one, iterating $s$ from $T$ down to $a$ (0/1 knapsack variant).

### NTT-Based Polynomial Multiplication

For large $T$ and many elements, use **Number Theoretic Transform** (NTT) to multiply generating function polynomials in $O(T \log T)$.

### Concrete Example

$S = \{1, 2, 3\}$, $T = 3$: subsets are $\{3\}$ and $\{1, 2\}$, so count = 2.

Generating function: $(1+x)(1+x^2)(1+x^3) = 1 + x + x^2 + 2x^3 + x^4 + x^5 + x^6$.
Coefficient of $x^3$ is 2.

## Derivation

### Algorithm: 0/1 Knapsack DP

1. Initialize $f[0] = 1$, $f[s] = 0$ for $s > 0$.
2. For each element $a \in S$: for $s = T$ down to $a$: $f[s] \mathrel{+}= f[s-a]$.
3. Answer is $f[T]$.

## Proof of Correctness

**Theorem.** *The DP correctly counts subsets summing to $T$.*

*Proof.* After processing elements $a_1, \ldots, a_k$, $f[s]$ counts subsets of $\{a_1, \ldots, a_k\}$ summing to $s$. Adding $a_{k+1}$: a subset of $\{a_1, \ldots, a_{k+1}\}$ summing to $s$ either includes $a_{k+1}$ (counted by old $f[s - a_{k+1}]$) or excludes it (old $f[s]$). Processing $s$ in decreasing order ensures each element is used at most once. $\square$

## Complexity Analysis

- **DP:** $O(|S| \cdot T)$ time, $O(T)$ space.
- **NTT:** $O(T \log T \cdot \log |S|)$ for polynomial multiplication.


### Additional Analysis

NTT for large T: use p=998244353 supporting NTT up to 2^23. Meet-in-the-middle for |S|<=40. Verification: S={1,2,3,4,5}, T=7: count=3.


### DP Implementation

Initialize f[0] = 1, f[s] = 0 for s > 0. For each element a: for s = T down to a: f[s] += f[s-a]. Answer is f[T]. Time: O(|S|*T), space: O(T).

### Meet-in-the-Middle

For |S| <= 40: split S into halves, enumerate all 2^{|S|/2} subset sums for each half, count pairs summing to T. Time: O(2^{n/2} * log(2^{n/2})).

### NTT for Large T

Polynomial multiplication via NTT in O(T log T) per element, or O(T log T * log |S|) total.

### Partition Theory Connection

When S = {1,...,n}, counting subsets summing to T equals the number of partitions of T into distinct parts <= n, given by the q-binomial coefficient.


### Complexity Comparison Table

| Method | Time | Space | Best for |
|--------|------|-------|----------|
| DP | O(|S| * T) | O(T) | Small T |
| NTT | O(T log T) | O(T) | Large |S|, moderate T |
| Meet-in-middle | O(2^{n/2}) | O(2^{n/2}) | Large T, small |S| |

### Formal Proof of DP Correctness

**Theorem.** After processing elements a_1, ..., a_k, f[s] counts subsets of {a_1, ..., a_k} summing to s.

*Proof.* By induction on k. Base: k=0, f[0]=1 (empty subset). Step: a subset of {a_1,...,a_{k+1}} summing to s either includes a_{k+1} (counted by old f[s-a_{k+1}]) or excludes it (old f[s]). Processing s in decreasing order ensures each element used at most once. Square.

### Integer Partition Connection

The number of subsets of {1,...,n} summing to T equals the number of partitions of T into distinct parts each <= n. This is the q-binomial coefficient [n choose k]_q evaluated appropriately.

### Generating Function Coefficient Extraction

For the product (1+x^{a_1})(1+x^{a_2})...(1+x^{a_n}), the coefficient of x^T can be extracted in O(|S|*T) time by maintaining the running polynomial product.

## Answer

$$\boxed{689294705}$$
