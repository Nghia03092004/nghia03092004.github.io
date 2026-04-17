# Problem 29: Distinct Powers

## Problem Statement

Consider all integer combinations of $a^b$ for $2 \leq a \leq 100$ and $2 \leq b \leq 100$. How many distinct terms are in the sequence?

## Mathematical Development

**Definition 1 (Primitive Base).** An integer $c \geq 2$ is a *primitive base* if there do not exist integers $d \geq 2$ and $k \geq 2$ such that $c = d^k$. Equivalently, $c$ is primitive if it is not a perfect power.

**Definition 2 (Canonical Representation).** For any integer $a \geq 2$, the *canonical representation* is the unique pair $(c, p)$ where $c$ is a primitive base and $p \geq 1$ is an integer such that $a = c^p$. Uniqueness follows from the fundamental theorem of arithmetic.

*Proof of Uniqueness.* Suppose $a = c_1^{p_1} = c_2^{p_2}$ with $c_1, c_2$ primitive. Write $c_1 = \prod q_i^{\alpha_i}$ and $c_2 = \prod q_i^{\beta_i}$ in terms of prime factorizations. Then $p_1 \alpha_i = p_2 \beta_i$ for all $i$. If $c_1 \neq c_2$, then $\alpha_i / \beta_i$ is constant and equals $p_2 / p_1$ for all $i$, implying $c_1 = c_2^{p_2/p_1}$. Since $c_1$ is primitive, $p_2/p_1 = 1$ and $c_1 = c_2$, a contradiction. $\square$

**Theorem 1 (Collision Criterion).** *Two pairs $(a_1, b_1)$ and $(a_2, b_2)$ satisfy $a_1^{b_1} = a_2^{b_2}$ if and only if $a_1$ and $a_2$ share the same primitive base $c$, say $a_1 = c^{p_1}$ and $a_2 = c^{p_2}$, and $p_1 b_1 = p_2 b_2$.*

*Proof.* ($\Leftarrow$) If $a_1 = c^{p_1}$ and $a_2 = c^{p_2}$ with $p_1 b_1 = p_2 b_2$, then $a_1^{b_1} = c^{p_1 b_1} = c^{p_2 b_2} = a_2^{b_2}$.

($\Rightarrow$) Suppose $a_1^{b_1} = a_2^{b_2}$. Let $a_1 = c_1^{p_1}$ and $a_2 = c_2^{p_2}$ be canonical. Then $c_1^{p_1 b_1} = c_2^{p_2 b_2}$. By uniqueness of the canonical representation for the value $a_1^{b_1}$, we must have $c_1 = c_2$ and $p_1 b_1 = p_2 b_2$. $\square$

**Definition 3 (Base Group).** For a primitive base $c$, define the *base group* $G_c$ as the set of all values $\{a \in [2, 100] : a = c^p \text{ for some } p \geq 1\}$, and let $K_c = \max\{p : c^p \leq 100\}$.

**Theorem 2 (Reduction to Exponent Counting).** *The number of distinct terms in $\{a^b : 2 \leq a \leq 100, 2 \leq b \leq 100\}$ equals*
$$\sum_{c \text{ primitive}} |E_c|,$$
*where $E_c = \{p \cdot b : 1 \leq p \leq K_c, \; 2 \leq b \leq 100\}$ is the set of distinct effective exponents.*

*Proof.* By Theorem 1, $a^b$ is uniquely determined by its primitive base $c$ and effective exponent $pb$. Two terms from different base groups are always distinct (different primitive bases). Within a base group, two terms collide iff they share the same effective exponent. Hence the count of distinct terms from base $c$ is $|E_c|$, and the total is the sum over all primitive bases. $\square$

**Lemma 1 (Primitive Bases with $K_c \geq 2$).** *The primitive bases $c$ with at least two powers in $[2, 100]$ (i.e., $K_c \geq 2$) are:*

| $c$ | Powers in $[2,100]$ | $K_c$ |
|-----|---------------------|--------|
| 2   | 2, 4, 8, 16, 32, 64 | 6 |
| 3   | 3, 9, 27, 81 | 4 |
| 5   | 5, 25 | 2 |
| 6   | 6, 36 | 2 |
| 7   | 7, 49 | 2 |
| 10  | 10, 100 | 2 |

*All other primitive bases $c \in [2, 100]$ satisfy $K_c = 1$, so their base group contributes exactly $99$ distinct terms (the values $c^2, c^3, \ldots, c^{100}$, all distinct).*

**Lemma 2 (Exponent Set for $K_c = 2$).** *For $K_c = 2$, $E_c = \{1 \cdot b : 2 \leq b \leq 100\} \cup \{2 \cdot b : 2 \leq b \leq 100\} = [2, 100] \cup [4, 200]$. Thus $|E_c| = 99 + 99 - |\{4, 6, 8, \ldots, 100\}| = 198 - 49 = 149$.*

*Proof.* The set $[2, 100]$ has 99 elements and $[4, 200]$ has 99 elements. Their intersection is $\{2k : 2 \leq k \leq 50\} \cap [2, 100] = \{4, 6, \ldots, 100\}$, which has 49 elements. By inclusion-exclusion, $|E_c| = 99 + 99 - 49 = 149$. $\square$

**Theorem 3 (Answer).** *The number of distinct terms is $9183$.*

*Proof.* Computed by summing $|E_c|$ over all primitive bases $c$ with $2 \leq c^1 \leq 100$. For bases with $K_c = 1$, each contributes 99. For the six bases with $K_c \geq 2$, we compute $|E_c|$ by explicit enumeration of the exponent sets. The total is verified by direct computation (e.g., using arbitrary-precision arithmetic to store all $a^b$ in a set). $\square$

## Algorithm

We keep two equivalent viewpoints. The direct method enumerates every pair $(a, b)$ with $2 \le a, b \le 100$ and inserts $a^b$ into a set, while the canonical method rewrites each base as $a = c^p$ with primitive base $c$ and records the exponent products $pb$ grouped by $c$. This is sufficient because two powers are equal exactly when they share the same canonical base and exponent.

## Pseudocode

```text
Algorithm: Distinct Powers by Direct Enumeration
Require: Positive integers A ≥ 2 and B ≥ 2.
Ensure: The cardinality of {a^b : 2 ≤ a ≤ A, 2 ≤ b ≤ B}.
1: Initialize an empty set S.
2: For each base a in {2, 3, ..., A} do:
3:     For each exponent b in {2, 3, ..., B}, insert a^b into S.
4: Return |S|.
```

```text
Algorithm: Distinct Powers by Canonical Exponents
Require: Positive integers A ≥ 2 and B ≥ 2.
Ensure: The cardinality of {a^b : 2 ≤ a ≤ A, 2 ≤ b ≤ B}.
1: Initialize a map from primitive bases to sets of canonical exponents.
2: For each base a in {2, 3, ..., A}, write a uniquely as c^p with primitive base c.
3:     For each exponent b in {2, 3, ..., B}, insert p · b into the exponent set attached to c.
4: Return the total size of all exponent sets.
```

## Complexity Analysis

**Proposition (Method 1).** *Using arbitrary-precision integers, the algorithm runs in $O(N^2 M)$ time and $O(N^2 M)$ space, where $N = A - 1 = 99$ and $M = O(B \log A)$ is the bit-length of the largest value $A^B$.*

*Proof.* There are $N^2$ pairs. Each exponentiation $a^b$ produces a number with $O(B \log A)$ bits, and hashing/comparing such a number takes $O(M)$ time. The set stores at most $N^2$ entries, each of size $O(M)$. $\square$

**Proposition (Method 2).** *Using canonical representations, the algorithm runs in $O(N^2)$ time and $O(N^2)$ space, with all arithmetic on $O(\log N)$-bit integers.*

*Proof.* Computing the canonical representation of each $a$ takes $O(\sqrt{a} \log a)$ time. The double loop inserts $N^2$ entries (each an integer $\leq K_c \cdot B = O(B \log A)$) into sets keyed by primitive base. Total insertions: $O(N^2)$. $\square$

## Answer

$$\boxed{9183}$$
