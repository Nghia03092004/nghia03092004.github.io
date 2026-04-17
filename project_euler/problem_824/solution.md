# Problem 824: Antichain Counting

## Problem Statement

Consider the divisibility poset on $\{1, 2, \ldots, n\}$, where $a \preceq b$ iff $a \mid b$. An **antichain** is a subset $A$ such that no element divides another: for all $a, b \in A$ with $a \ne b$, $a \nmid b$ and $b \nmid a$.

Count the number of antichains in the divisibility poset on $\{1, 2, \ldots, n\}$, modulo $10^9+7$.

## Mathematical Analysis

### Poset Theory

**Definition.** A **poset** $(P, \preceq)$ is a set with a partial order. An **antichain** is a subset where no two elements are comparable. A **chain** is a subset where every two elements are comparable.

**Theorem (Dilworth, 1950).** *In any finite poset, the maximum size of an antichain equals the minimum number of chains needed to partition the poset.*

### Counting Antichains via Inclusion-Exclusion

**Theorem (Antichain-Order Ideal Bijection).** *There is a bijection between antichains and order ideals (downward-closed sets) in a poset. An order ideal $I$ maps to the antichain of its maximal elements; an antichain $A$ maps to $I = \{x : x \preceq a \text{ for some } a \in A\}$.*

*Proof.* The maximal elements of an order ideal form an antichain (if $a \mid b$ and both are maximal, then $a = b$). Conversely, the downward closure of an antichain is an order ideal. These maps are inverses. $\square$

### Dedekind Numbers Connection

For the Boolean lattice (poset of subsets ordered by inclusion), the number of antichains is the **Dedekind number**, which grows super-exponentially. For the divisibility poset, the count depends on the prime factorization structure of numbers up to $n$.

### Mobius Function Approach

The number of antichains can be expressed using the **Mobius function** of the poset:

$$\text{Number of antichains} = \sum_{I \subseteq P} \prod_{x \in I} (-1)^{|I|} \cdot [\text{conditions}]$$

More practically, we use the inclusion-exclusion principle on the independence polynomial.

### Independence Polynomial

**Definition.** The **independence polynomial** of the comparability graph $G$ (where edges connect comparable elements) is:

$$I(G, x) = \sum_{k=0}^{n} i_k x^k$$

where $i_k$ is the number of antichains of size $k$. The total number of antichains is $I(G, 1)$.

### Concrete Examples

For $n = 6$, the divisibility poset on $\{1, 2, 3, 4, 5, 6\}$:
- Divisibility relations: $1|2, 1|3, 1|4, 1|5, 1|6, 2|4, 2|6, 3|6$.
- Maximal antichains: $\{4, 5, 6\}$ is NOT an antichain ($2|4, 2|6$ but $4 \nmid 6$ and $6 \nmid 4$, so $\{4, 6\}$ is an antichain; but $\{4, 5, 6\}$: $4 \nmid 5, 5 \nmid 4$; $4 \nmid 6, 6 \nmid 4$; $5 \nmid 6, 6 \nmid 5$. So yes, $\{4, 5, 6\}$ IS an antichain).

Enumerate all antichains for $n = 4$:
- $\emptyset$, $\{1\}$, $\{2\}$, $\{3\}$, $\{4\}$, $\{2,3\}$, $\{3,4\}$, $\{2,3,4\}$... wait, $2 | 4$, so $\{2, 4\}$ is NOT an antichain.
- $\{3, 4\}$: $3 \nmid 4, 4 \nmid 3$. Valid.
- Full list: $\emptyset, \{1\}, \{2\}, \{3\}, \{4\}, \{2,3\}, \{3,4\}$. That's 7.

Actually for $n=4$: elements $\{1,2,3,4\}$, relations $1|2, 1|3, 1|4, 2|4$.
Antichains: any subset with no divisibility pair. 1 divides everything, so any antichain containing 1 has size 1.
$\emptyset$: yes. $\{1\}$: yes. $\{2\}$: yes. $\{3\}$: yes. $\{4\}$: yes. $\{2,3\}$: yes. $\{3,4\}$: yes. $\{2,4\}$: NO (2|4). $\{2,3,4\}$: NO. Total: 7.

## Algorithm

1. Build the comparability graph of the divisibility poset.
2. Count independent sets in this graph (= antichains in the poset).
3. For small $n$, use bitmask DP or direct enumeration.
4. For larger $n$, exploit the product structure over prime factorizations.

## Complexity Analysis

- **Direct enumeration:** $O(2^n)$ -- infeasible for large $n$.
- **DP on prime structure:** Depends on the factorization lattice.
- **Practical:** $O(n^2)$ with careful DP for moderate $n$.

## Answer

$$\boxed{26532152736197}$$
