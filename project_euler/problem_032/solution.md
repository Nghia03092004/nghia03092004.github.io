# Problem 32: Pandigital Products

## Problem Statement

We say that an $n$-digit number is pandigital if it uses all the digits 1 to $n$ exactly once. Find the sum of all products whose multiplicand/multiplier/product identity can be written as a 1 through 9 pandigital.

## Mathematical Development

**Definition 1.** A triple $(a, b, p) \in \mathbb{Z}_{>0}^3$ with $p = a \cdot b$ is a *1-to-9 pandigital identity* if the concatenation of the decimal representations of $a$, $b$, and $p$ is a permutation of the string $123456789$. Let $d(x)$ denote the number of decimal digits of $x$.

**Theorem 1 (Digit-count constraint).** *If $(a, b, p)$ is a 1-to-9 pandigital identity with $a \le b$, then $d(p) = 4$ and $(d(a), d(b)) \in \{(1, 4),\, (2, 3)\}$.*

**Proof.** The pandigital condition requires $d(a) + d(b) + d(p) = 9$ with no digit 0 and no repeated digits. Let $\alpha = d(a)$, $\beta = d(b)$, $\gamma = d(p)$.

Since $10^{\alpha - 1} \le a < 10^\alpha$ and $10^{\beta - 1} \le b < 10^\beta$, the product satisfies
$$10^{\alpha + \beta - 2} \le p < 10^{\alpha + \beta},$$
whence $\gamma \in \{\alpha + \beta - 1,\, \alpha + \beta\}$.

*Case $\gamma = \alpha + \beta$:* Then $\alpha + \beta + (\alpha + \beta) = 9$, giving $\alpha + \beta = 9/2 \notin \mathbb{Z}$. Contradiction.

*Case $\gamma = \alpha + \beta - 1$:* Then $\alpha + \beta + (\alpha + \beta - 1) = 9$, giving $\alpha + \beta = 5$ and $\gamma = 4$.

Subject to $1 \le \alpha \le \beta$ and $\alpha + \beta = 5$, the admissible pairs are $(\alpha, \beta) \in \{(1, 4),\, (2, 3)\}$. $\square$

**Lemma 1 (Search bounds).** *For case $(1, 4)$: $a \in [1, 9]$, $b \in [1234, 9876]$. For case $(2, 3)$: $a \in [12, 98]$, $b \in [123, 987]$. In both cases, $p$ must satisfy $1000 \le p \le 9876$.*

**Proof.** Since digits are drawn from $\{1, \ldots, 9\}$ without repetition, the smallest $k$-digit number is $\underbrace{12\ldots k}$ and the largest is formed from the $k$ largest digits in decreasing order. The constraint $d(p) = 4$ with digits in $\{1, \ldots, 9\}$ forces $1000 \le p \le 9876$. The bounds on $a$ and $b$ follow analogously. $\square$

**Proposition 1 (Deduplication).** *The problem requires the sum of distinct products. If $(a_1, b_1, p)$ and $(a_2, b_2, p)$ are both pandigital identities with $a_1 \ne a_2$, the product $p$ is counted once.*

**Proof.** The problem statement specifies "sum of all products," not "sum over all identities." $\square$

**Theorem 2 (Exhaustive enumeration).** *The complete set of 1-to-9 pandigital products is $\{4396, 5346, 5796, 6952, 7254, 7632, 7852\}$.*

**Proof.** Enumerate all pairs $(a, b)$ in the two cases of Theorem 1. For each pair, compute $p = ab$ and test whether $d(p) = 4$ and the concatenation of digits of $a$, $b$, $p$ is a permutation of $\{1, \ldots, 9\}$. The test is a constant-time operation (sort 9 characters and compare). The search examines at most $9 \times 8643 + 87 \times 865 < 1.6 \times 10^5$ pairs, each verified or rejected in $O(1)$, yielding exactly the seven products listed. $\square$

## Algorithm

```
PANDIGITAL-PRODUCTS():
    products <- empty set
    // Case (1,4)
    for a <- 1 to 9:
        for b <- 1234 to 9876:
            p <- a * b
            if d(p) != 4: continue
            if IS-PANDIGITAL-9(a, b, p):
                products.add(p)
    // Case (2,3)
    for a <- 12 to 98:
        for b <- 123 to 987:
            p <- a * b
            if d(p) != 4: continue
            if IS-PANDIGITAL-9(a, b, p):
                products.add(p)
    return SUM(products)

IS-PANDIGITAL-9(a, b, p):
    s <- concatenate digits of a, b, p
    return |s| = 9 and sorted(s) = "123456789"
```

## Complexity Analysis

**Proposition 2.** *The algorithm runs in $O(N)$ time and $O(1)$ space, where $N \approx 1.6 \times 10^5$ is the number of candidate pairs.*

**Proof.** The outer loops iterate over a fixed number of pairs determined by Lemma 1. The pandigital test for each pair examines exactly 9 digits (constant work). The set of products has at most $|P| = 7$ elements, requiring $O(1)$ auxiliary space. $\square$

## Answer

$$\boxed{45228}$$
