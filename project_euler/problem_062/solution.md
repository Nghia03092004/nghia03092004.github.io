# Problem 62: Cubic Permutations

## Problem Statement

The cube $41063625 = 345^3$ can be obtained by permuting the digits of $56623104 = 384^3$. Find the smallest cube for which exactly five permutations of its digits are also cubes.

## Mathematical Foundation

**Definition 1.** For a positive integer $m$, define the *canonical digit signature* $\sigma(m)$ as the string obtained by sorting the decimal digits of $m$ in non-decreasing order. For example, $\sigma(41063625) = 01234566$.

**Theorem 1 (Permutation equivalence).** Two positive integers $a$ and $b$ are digit permutations of each other if and only if $\sigma(a) = \sigma(b)$.

**Proof.** ($\Rightarrow$) If $b$ is a digit permutation of $a$, there exists a bijection $\pi$ on digit positions such that the $i$-th digit of $a$ equals the $\pi(i)$-th digit of $b$. Hence $a$ and $b$ share the same multiset of digits, and sorting both multisets yields the same string.

($\Leftarrow$) If $\sigma(a) = \sigma(b)$, the multisets of digits coincide. Since any two sequences with the same multiset are permutations of each other, $b$ is a digit permutation of $a$. $\square$

**Lemma 1 (Digit count preservation).** If $m^3$ and $n^3$ are digit permutations of each other (with $m, n \ge 1$), then $m^3$ and $n^3$ have the same number of digits.

**Proof.** Two numbers sharing the same digit multiset necessarily have the same total digit count, since neither has a leading zero (both being positive cubes $\ge 1$). $\square$

**Theorem 2 (Digit-count finality).** Let $D(k) = \lfloor \log_{10} k \rfloor + 1$ denote the number of decimal digits of $k > 0$. For a fixed digit count $d$, define the *equivalence classes*

$$\mathcal{C}_d = \bigl\{ [c] : c = n^3 \text{ with } D(n^3) = d \bigr\}$$

where $[c] = \{m^3 : \sigma(m^3) = \sigma(c),\; D(m^3) = d\}$. Then no cube with a different digit count can belong to any class in $\mathcal{C}_d$. Consequently, once all cubes with $d$ digits have been enumerated, the class sizes within $\mathcal{C}_d$ are final.

**Proof.** By Lemma 1, any cube that is a digit permutation of a $d$-digit cube must itself have $d$ digits. Therefore, $d'$-digit cubes with $d' \ne d$ cannot enter any class in $\mathcal{C}_d$. When all $n$ with $D(n^3) = d$ have been processed, no future cube can modify the classes. $\square$

**Lemma 2 (Range of $d$-digit cubes).** The cubes with exactly $d$ digits are $\{n^3 : n \in I_d\}$ where $I_d = [\lceil 10^{(d-1)/3} \rceil, \lfloor (10^d - 1)^{1/3} \rfloor]$. In particular, $|I_d| = \Theta(10^{d/3})$.

**Proof.** The condition $10^{d-1} \le n^3 \le 10^d - 1$ is equivalent to $10^{(d-1)/3} \le n \le (10^d - 1)^{1/3}$. The interval length is $(10^d - 1)^{1/3} - 10^{(d-1)/3} = \Theta(10^{d/3})$. $\square$

**Corollary.** The algorithm can process cubes in order of $n$ and finalize all classes of digit count $d$ as soon as the first $(d+1)$-digit cube is encountered.

## Editorial

We generate cubes in increasing order and group them by their canonical digit signatures, but only within a fixed digit length. This digit-length partition is important because once the number of digits increases, no later cube can belong to any group from the previous layer. At each such transition we inspect the completed groups from the previous digit length, and if one of them contains exactly five cubes, the smallest cube in that group is the answer. Otherwise we discard the old groups and continue with the next digit length.

## Pseudocode

```text
Initialize the grouping structure for the current digit length.
Start with no active digit length.

For each positive integer n:
    compute the cube c = n^3
    determine the number of digits of c

    if this is the first cube of a new digit length:
        inspect every group from the previous digit length
        if one of them contains exactly five cubes, return its smallest element
        reset the grouping structure for the new digit length

    place c into the group indexed by its sorted digit signature
```

## Complexity Analysis

**Time:** Let $K$ be the value of $n$ at termination. For each $n$, we compute $n^3$ in $O(1)$ (or $O(d)$ for arbitrary-precision integers), sort its $d$ digits in $O(d \log d)$, and perform a hash-map lookup in $O(d)$ expected time. The answer occurs at $n = 5027$ with $d = 12$ digits, giving total cost $O(K \cdot d \log d) \approx O(5027 \times 12 \times 4) = O(2.4 \times 10^5)$.

**Space:** $O(|I_d|)$ for the groups hash map within the current digit count, since completed digit counts are discarded.

## Answer

$$\boxed{127035954683}$$
