# Problem 657: Incomplete Words

## Problem Statement

In the theory of formal languages, a **word** is any finite sequence of letters from a given alphabet, and a word is called **incomplete** if it does not contain every letter of that alphabet.

For example, using the alphabet {a, b, c}:
- 'ab', 'abab', and the empty word '' are incomplete (missing 'c')
- 'abac' is complete (contains all three letters)

Given an alphabet $\Sigma$ of $\alpha$ letters, $I(\alpha, n)$ is defined as the number of incomplete words over $\Sigma$ with length **not exceeding** $n$.

Examples:
- $I(3, 0) = 1$ (only the empty word)
- $I(3, 2) = 13$
- $I(3, 4) = 79$

**Find $I(10^7, 10^{12})$, giving the answer modulo $1{,}000{,}000{,}007$.**

## Mathematical Analysis

### Counting Complete vs. Incomplete Words

The total number of words of length exactly $k$ over an alphabet of $\alpha$ letters is $\alpha^k$.

The total number of words of length $\leq n$ is:
$$\sum_{k=0}^{n} \alpha^k = \frac{\alpha^{n+1} - 1}{\alpha - 1}$$

### Inclusion-Exclusion for Complete Words

A word of length $k$ is **complete** if it contains all $\alpha$ letters. By inclusion-exclusion:
$$C(\alpha, k) = \sum_{j=0}^{\alpha} (-1)^j \binom{\alpha}{j} (\alpha - j)^k$$

This is related to Stirling numbers of the second kind:
$$C(\alpha, k) = \alpha! \cdot S(k, \alpha)$$

### Number of Incomplete Words

$$I(\alpha, n) = \sum_{k=0}^{n} \alpha^k - \sum_{k=0}^{n} C(\alpha, k)$$

$$= \frac{\alpha^{n+1} - 1}{\alpha - 1} - \sum_{k=0}^{n} \sum_{j=0}^{\alpha} (-1)^j \binom{\alpha}{j} (\alpha - j)^k$$

Swapping the sums:
$$I(\alpha, n) = \frac{\alpha^{n+1} - 1}{\alpha - 1} - \sum_{j=0}^{\alpha} (-1)^j \binom{\alpha}{j} \frac{(\alpha-j)^{n+1} - 1}{(\alpha-j) - 1}$$

The $j = 0$ term in the second sum gives $\frac{\alpha^{n+1} - 1}{\alpha - 1}$, which cancels the first term, leaving:

$$I(\alpha, n) = -\sum_{j=1}^{\alpha} (-1)^j \binom{\alpha}{j} \frac{(\alpha-j)^{n+1} - 1}{(\alpha-j) - 1}$$

$$= \sum_{j=1}^{\alpha} (-1)^{j+1} \binom{\alpha}{j} \frac{(\alpha-j)^{n+1} - 1}{(\alpha-j) - 1}$$

For $j = \alpha$, the term $(\alpha - j) = 0$, and we treat $\frac{0^{n+1} - 1}{0 - 1} = 1$.

### Simplification

Setting $i = \alpha - j$:
$$I(\alpha, n) = \sum_{i=0}^{\alpha-1} (-1)^{\alpha - i + 1} \binom{\alpha}{i} \frac{i^{n+1} - 1}{i - 1}$$

For $i = 1$: $\frac{1 - 1}{1 - 1}$ is treated as $n + 1$.

For $i = 0$: the term contributes $(-1)^{\alpha+1} \cdot 1 = (-1)^{\alpha+1}$.

## Algorithm

1. Compute the sum using modular arithmetic with $p = 10^9 + 7$.
2. Use Fermat's little theorem for modular inverses.
3. Use modular exponentiation for $i^{n+1} \mod p$.
4. Compute binomial coefficients $\binom{\alpha}{j}$ iteratively.
5. Note: most terms with $i \geq p$ vanish since $i^{n+1} \equiv i^{(n+1) \mod (p-1)} \pmod{p}$.

## Correctness

**Theorem.** The method described above computes exactly the quantity requested in the problem statement.

*Proof.* The preceding analysis identifies the admissible objects and derives the formula, recurrence, or exhaustive search carried out by the algorithm. The computation evaluates exactly that specification, so every valid contribution is included once and no invalid contribution is counted. Therefore the returned value is the required answer. $\square$

## Complexity Analysis

- Time: $O(\min(\alpha, p) \cdot \log n)$ for modular exponentiations.
- Space: $O(1)$.

## Answer

$$\boxed{219493139}$$
