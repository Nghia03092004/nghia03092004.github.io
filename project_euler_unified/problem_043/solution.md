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

## Definitions

**Definition 1.** For a sequence of digits $d_i, d_j, d_k$, the *three-digit substring value* is $\overline{d_i d_j d_k} = 100 d_i + 10 d_j + d_k$.

**Definition 2.** Let $p_1 = 2, p_2 = 3, p_3 = 5, p_4 = 7, p_5 = 11, p_6 = 13, p_7 = 17$ denote the first seven primes. The *substring divisibility property* requires $p_i \mid \overline{d_{i+1}\, d_{i+2}\, d_{i+3}}$ for each $i \in \{1, \ldots, 7\}$.

## Mathematical Development

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

## Algorithm

```
function FIND_SUBSTRING_DIVISIBLE_PANDIGITALS():
    primes <- [17, 13, 11, 7, 5, 3, 2]
    candidates <- {}

    // Step 1: seed with multiples of 17 having distinct digits
    for m <- 0 to 999 step 17:
        (d8, d9, d10) <- decimal digits of m (zero-padded)
        if d8, d9, d10 are pairwise distinct:
            candidates <- candidates + {([d8, d9, d10], {d8, d9, d10})}

    // Steps 2-7: extend leftward
    for i <- 1 to 6:
        p <- primes[i]
        new_candidates <- {}
        for (seq, used) in candidates:
            for d <- 0 to 9:
                if d not in used and p | (100*d + 10*seq[0] + seq[1]):
                    new_candidates <- new_candidates + {([d] + seq, used + {d})}
        candidates <- new_candidates

    // Step 8: assign remaining digit as d1
    total <- 0
    for (seq, used) in candidates:
        d1 <- the unique element of {0,...,9} \ used
        N <- integer formed by [d1] + seq
        total <- total + N
    return total
```

## Complexity Analysis

**Proposition (Time complexity).** *The algorithm runs in $O(C)$ time where $C$ is the total number of partial candidates explored across all steps. Empirically, $C = O(10^2)$ due to the cascading divisibility constraints.*

**Proof.** At each of the 7 extension steps, for each surviving candidate, we try at most 10 digit choices and perform an $O(1)$ divisibility check. The key observation is that the constraints are highly restrictive: divisibility by 17 (Step 1) admits at most 53 triples, and each subsequent step eliminates most extensions. The total number of candidates explored is bounded by a small constant (empirically, on the order of 100). This is a dramatic improvement over the naive $O(10!) \approx 3.6 \times 10^6$ brute-force enumeration. $\square$

**Proposition (Space complexity).** *$O(C)$ for storing the candidate list, where $C$ is bounded by the number of surviving partial assignments.*

## Answer

$$\boxed{16695334890}$$
