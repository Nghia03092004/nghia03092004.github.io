# Problem 43: Sub-string Divisibility

## Problem Statement

Let $d_1 d_2 d_3 \ldots d_{10}$ be a $0$-to-$9$ pandigital number (a permutation of the digits $\{0,1,\ldots,9\}$). We require the following sub-string divisibility conditions:

| Substring | Divisor |
|-----------|---------|
| $d_2 d_3 d_4$ | 2 |
| $d_3 d_4 d_5$ | 3 |
| $d_4 d_5 d_6$ | 5 |
| $d_5 d_6 d_7$ | 7 |
| $d_6 d_7 d_8$ | 11 |
| $d_7 d_8 d_9$ | 13 |
| $d_8 d_9 d_{10}$ | 17 |

Find the sum of all $0$-to-$9$ pandigital numbers satisfying these conditions.

## Mathematical Development

### Definitions

**Definition 1.** For a sequence of digits $d_i, d_j, d_k$, the *three-digit substring value* is $\overline{d_i d_j d_k} = 100 d_i + 10 d_j + d_k$.

**Definition 2.** Let $p_1 = 2, p_2 = 3, p_3 = 5, p_4 = 7, p_5 = 11, p_6 = 13, p_7 = 17$ denote the first seven primes. The *substring divisibility property* requires $p_i \mid \overline{d_{i+1}\, d_{i+2}\, d_{i+3}}$ for each $i \in \{1, \ldots, 7\}$.

### Theoretical Development

**Lemma 1 (Parity of $d_4$).** *The digit $d_4$ must be even, i.e., $d_4 \in \{0, 2, 4, 6, 8\}$.*

**Proof.** The condition $2 \mid \overline{d_2 d_3 d_4}$ requires $2 \mid (100 d_2 + 10 d_3 + d_4)$. Since $100 d_2$ and $10 d_3$ are both even, we need $2 \mid d_4$. $\square$

**Lemma 2 (Constraint on $d_6$).** *We have $d_6 \in \{0, 5\}$.*

**Proof.** The condition $5 \mid \overline{d_4 d_5 d_6}$ requires $5 \mid (100 d_4 + 10 d_5 + d_6)$. Since $100 d_4 + 10 d_5 \equiv 0 \pmod{5}$, we need $5 \mid d_6$, so $d_6 \in \{0, 5\}$. $\square$

**Lemma 3 (Divisibility by 3).** *The condition $3 \mid \overline{d_3 d_4 d_5}$ is equivalent to $d_3 + d_4 + d_5 \equiv 0 \pmod{3}$.*

**Proof.** Since $100 \equiv 1 \pmod{3}$ and $10 \equiv 1 \pmod{3}$, we have $\overline{d_3 d_4 d_5} = 100 d_3 + 10 d_4 + d_5 \equiv d_3 + d_4 + d_5 \pmod{3}$. $\square$

**Lemma 4 (Overlapping constraint propagation).** *Each consecutive pair of divisibility conditions shares two digits. Specifically, $\overline{d_{i+1}\, d_{i+2}\, d_{i+3}}$ and $\overline{d_{i+2}\, d_{i+3}\, d_{i+4}}$ share digits $d_{i+2}$ and $d_{i+3}$. This overlap enables efficient right-to-left construction: once $d_{i+2}$ and $d_{i+3}$ are fixed, only the new digit $d_{i+1}$ (or $d_{i+4}$) must be determined.*

**Proof.** Immediate from the definition of overlapping substrings. $\square$

**Theorem 1 (Exhaustive enumeration).** *There are exactly six $0$-to-$9$ pandigital numbers satisfying the substring divisibility property:*

$$1406357289, \quad 1430952867, \quad 1460357289, \quad 4106357289, \quad 4130952867, \quad 4160357289.$$

**Proof.** We construct all solutions by building digit sequences from right to left, exploiting the cascading constraints.

*Step 1.* Enumerate all values of $(d_8, d_9, d_{10})$ such that $\overline{d_8 d_9 d_{10}}$ is a multiple of $17$ with pairwise distinct digits from $\{0, \ldots, 9\}$. There are $\lfloor 999/17 \rfloor = 58$ multiples of $17$ in $[0, 999]$; after filtering for distinct digits, at most $53$ candidates remain.

*Step 2.* For each valid triple, extend to $d_7$: choose an unused digit $d_7 \in \{0,\ldots,9\} \setminus \{d_8, d_9, d_{10}\}$ such that $13 \mid \overline{d_7 d_8 d_9}$.

*Step 3.* Extend to $d_6$: choose unused $d_6$ with $11 \mid \overline{d_6 d_7 d_8}$. By Lemma 2, $d_6 \in \{0, 5\}$, drastically reducing candidates.

*Step 4.* Extend to $d_5$: choose unused $d_5$ with $7 \mid \overline{d_5 d_6 d_7}$.

*Step 5.* Extend to $d_4$: choose unused $d_4$ with $5 \mid \overline{d_4 d_5 d_6}$. By Lemma 1, $d_4$ must be even.

*Step 6.* Extend to $d_3$: choose unused $d_3$ with $3 \mid \overline{d_3 d_4 d_5}$ (equivalently, $d_3 + d_4 + d_5 \equiv 0 \pmod{3}$ by Lemma 3).

*Step 7.* Extend to $d_2$: choose unused $d_2$ with $2 \mid \overline{d_2 d_3 d_4}$. Since $d_4$ is even (already guaranteed by construction), any unused digit works for $d_2$.

*Step 8.* Assign the sole remaining digit to $d_1$.

This exhaustive search with constraint propagation yields exactly six solutions. Their sum is:
$$\sum = 1406357289 + 1430952867 + 1460357289 + 4106357289 + 4130952867 + 4160357289 = 16695334890.$$

The construction is complete in the sense that every digit assignment is tested at each step, and all constraints are verified. No valid pandigital number can be missed. $\square$

## Editorial

We construct admissible pandigital numbers from right to left, using the overlap between consecutive three-digit divisibility conditions. The search is seeded with distinct-digit multiples of $17$ for the block $d_8d_9d_{10}$, and each subsequent stage prepends one unused digit that makes the new leading three-digit block divisible by $13,11,7,5,3,$ and $2$, respectively. Once all divisibility constraints have been enforced, the unique remaining digit becomes $d_1$, and the resulting pandigital numbers are summed.

## Pseudocode

```text
Algorithm: Pandigital Numbers with Substring Divisibility
Require: The decimal digits 0 through 9.
Ensure: The sum of all 0-to-9 pandigital numbers satisfying the prescribed divisibility conditions.
1: Initialize the search with all distinct-digit three-digit blocks divisible by 17, interpreted as suffixes d_8d_9d_10.
2: For each divisor in the sequence 13, 11, 7, 5, 3, 2 do:
3:     Extend every current suffix by prepending a digit d so that the new leading three-digit block is divisible by the current divisor and all digits remain distinct.
4: After the last extension, prepend the unique missing digit to each 9-digit tail to obtain a full pandigital number.
5: Return the sum of all numbers constructed in this way.
```

## Complexity Analysis

**Proposition (Time complexity).** *The algorithm runs in $O(C)$ time where $C$ is the total number of partial candidates explored across all steps. Empirically, $C = O(10^2)$ due to the cascading divisibility constraints.*

**Proof.** At each of the 7 extension steps, for each surviving candidate, we try at most 10 digit choices and perform an $O(1)$ divisibility check. The key observation is that the constraints are highly restrictive: divisibility by 17 (Step 1) admits at most 53 triples, and each subsequent step eliminates most extensions. The total number of candidates explored is bounded by a small constant (empirically, on the order of 100). This is a dramatic improvement over the naive $O(10!) \approx 3.6 \times 10^6$ brute-force enumeration. $\square$

**Proposition (Space complexity).** *$O(C)$ for storing the candidate list, where $C$ is bounded by the number of surviving partial assignments.*

## Answer

$$\boxed{16695334890}$$
