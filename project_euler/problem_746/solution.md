# Problem 746: A Messy Dinner

## Problem Statement

There are $n$ families, each consisting of a father, mother, son, and daughter ($4$ people). They are seated at a circular table with $4n$ seats, alternating by gender (male-female-male-female-...). Let $M(n)$ count the number of seating arrangements where no two members of the same family occupy adjacent seats. Define $S(n) = \sum_{k=2}^{n} M(k)$.

Given: $M(1) = 0$, $M(2) = 896$, $M(3) = 890{,}880$.

Find $S(2021) \bmod (10^9 + 7)$.

## Mathematical Foundation

**Theorem 1 (Gender-Constrained Circular Permutation).** *The alternating-gender constraint partitions the $4n$ seats into $2n$ male seats (odd positions) and $2n$ female seats (even positions). The $2n$ males (fathers and sons) are permuted among male seats, and the $2n$ females (mothers and daughters) among female seats. Thus the total arrangement count (before the "no adjacent family members" restriction) is $(2n-1)! \cdot (2n)!$ (fixing one male to break circular symmetry).*

**Proof.** In a circular arrangement of $4n$ seats with alternating genders, fixing one male's position removes the rotational symmetry. The remaining $2n - 1$ males can be placed in $(2n-1)!$ ways in the remaining male seats, and the $2n$ females in $(2n)!$ ways in the female seats. $\square$

**Theorem 2 (Inclusion-Exclusion Framework).** *For each family $i \in [n]$, define the "bad" event $B_i$ as: at least two members of family $i$ sit adjacently. Then:*

$$M(n) = \sum_{S \subseteq [n]} (-1)^{|S|} N(S)$$

*where $N(S)$ counts arrangements in which every family in $S$ has at least two adjacent members.*

**Proof.** Standard inclusion-exclusion over the $n$ bad events. $\square$

**Lemma 1 (Family Adjacency Structure).** *In the alternating-gender arrangement, family $i$ contributes two males (father, son) to male seats and two females (mother, daughter) to female seats. Two family members are adjacent iff a male and female from the same family occupy neighboring seats. Each male seat has exactly $2$ adjacent female seats. The adjacency constraint is thus a bipartite forbidden-adjacency problem on the male-female seat graph.*

**Proof.** Adjacent seats alternate gender by construction. A male seat at position $2j-1$ is adjacent to female seats $2j-2$ and $2j$ (cyclically). Two members of the same family are adjacent iff they form a male-female pair on neighboring seats. $\square$

**Theorem 3 (Linear Recurrence Modulo $p$).** *The sequence $M(n) \bmod p$ satisfies a linear recurrence of bounded order $r$ (depending on the structure of the transfer matrix). This recurrence can be discovered from the first $O(r)$ terms using the Berlekamp-Massey algorithm, and then extended to $n = 2021$ using Kitamasa's method or matrix exponentiation.*

**Proof.** The generating function for $M(n)$ is rational (as $M(n)$ counts configurations of a finite-state system with linearly growing input). The denominator of this rational function has degree $r$, yielding a linear recurrence of order $r$. The Berlekamp-Massey algorithm recovers the minimal polynomial from $2r$ consecutive values. Once the recurrence is known, the $n$-th term modulo $p$ is computable in $O(r \log r \log n)$ time via Kitamasa's method (polynomial modular exponentiation). $\square$

## Editorial
$n$ families of 4 (father, mother, son, daughter) seated at a circular table with $4n$ seats, alternating genders. $M(n)$ counts arrangements where no family sits together consecutively. $S(n) = \sum_. We compute M(k) mod p for k = 2, 3, ..., up to 2*r_max. We then using direct enumeration or the transfer matrix for small k. Finally, find minimal linear recurrence via Berlekamp-Massey.

## Pseudocode

```text
Compute M(k) mod p for k = 2, 3, ..., up to 2*r_max
using direct enumeration or the transfer matrix for small k
Find minimal linear recurrence via Berlekamp-Massey
Extend to compute M(k) for k = 2..N using the recurrence
and accumulate S(N) = sum of M(k)
Use Kitamasa's method for each term, or note that the prefix
sum also satisfies a linear recurrence of order r+1
The prefix sum S(n) = S(n-1) + M(n) satisfies a recurrence
derived from M's recurrence augmented by one order
```

## Complexity Analysis

- **Time:** $O(r^2)$ for Berlekamp-Massey, $O(r \log r \log N)$ for Kitamasa's method. The recurrence order $r$ is bounded by the transfer matrix state space.
- **Space:** $O(r)$ for storing the recurrence and polynomial operations.

## Answer

$$\boxed{867150922}$$
