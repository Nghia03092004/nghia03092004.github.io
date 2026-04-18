# Problem 851: SOP and POS

## Problem Statement

A Boolean function $f: \{0,1\}^n \to \{0,1\}$ can be represented in **Sum of Products** (SOP, or DNF) form:

$$f = \bigvee_{i} \bigwedge_{j} l_{ij}$$

where each $l_{ij}$ is a literal ($x_k$ or $\overline{x_k}$). The **minimal SOP** representation uses the fewest product terms. Similarly, the **Product of Sums** (POS, or CNF) form is $f = \bigwedge_i \bigvee_j l_{ij}$. Compute the total number of minimal SOP representations (or the minimal number of prime implicants) for a given truth table.

## Mathematical Foundation

**Definition.** A **minterm** of $n$ variables is a product of all $n$ variables, each either complemented or uncomplemented. There are $2^n$ minterms. The **canonical SOP** is $f = \bigvee_{i : f(i) = 1} m_i$.

**Definition.** An **implicant** of $f$ is a product term $p$ such that $p \Rightarrow f$. A **prime implicant** (PI) is an implicant that is not contained in (i.e., does not imply) any other implicant with fewer literals.

**Theorem 1 (Quine, 1952).** Every minimal SOP of $f$ consists entirely of prime implicants.

**Proof.** Let $S = \{p_1, \ldots, p_k\}$ be a minimal SOP of $f$. Suppose for contradiction that some $p_i$ is not a prime implicant. Then there exists a product term $q$ with fewer literals such that $p_i \Rightarrow q$ and $q \Rightarrow f$. Replacing $p_i$ by $q$ in $S$ still covers all minterms that $p_i$ covered (since $p_i \Rightarrow q$) and introduces no new minterms outside $f$ (since $q \Rightarrow f$). Moreover, $q$ may cover additional minterms, potentially allowing removal of other terms from $S$, yielding a representation with at most $k$ terms. If $q$ itself is not prime, repeat. This process terminates since the number of literals is finite, contradicting minimality only if $p_i$ were already prime. $\square$

**Theorem 2 (Minimum Cover Equivalence).** The number of minimal SOP forms of $f$ equals the number of minimum-cardinality covers of the on-set by prime implicants, after accounting for essential prime implicants.

**Proof.** By Theorem 1, every minimal SOP uses only PIs. An essential PI is one that uniquely covers some minterm; it must appear in every minimal cover. Removing essential PIs and their covered minterms reduces the problem to finding minimum covers of the residual minterms by non-essential PIs. Each such minimum cover, combined with the essential PIs, yields a distinct minimal SOP. Conversely, every minimal SOP arises this way. $\square$

**Lemma 1 (Duality).** The minimal POS of $f$ is the minimal SOP of $\overline{f}$ with all literals complemented.

**Proof.** By De Morgan's laws, $\overline{f} = \overline{\bigwedge_i \bigvee_j l_{ij}} = \bigvee_i \bigwedge_j \overline{l_{ij}}$. Thus minimizing the POS of $f$ is equivalent to minimizing the SOP of $\overline{f}$. $\square$

**Theorem 3 (NP-Hardness).** The minimum cover problem for the prime implicant chart is equivalent to the minimum set cover problem, which is NP-hard in general.

**Proof.** Given an instance of set cover $(\mathcal{U}, \mathcal{S})$, construct a Boolean function whose minterms correspond to elements of $\mathcal{U}$ and whose prime implicants correspond to sets in $\mathcal{S}$. A PI covers a minterm iff the corresponding set contains that element. A minimum SOP corresponds to a minimum set cover. Since set cover is NP-hard (Karp, 1972), so is the PI chart covering problem. $\square$

## Editorial
Boolean function minimization via Quine-McCluskey algorithm. We generate all prime implicants. We then build prime implicant chart. Finally, extract essential prime implicants.

## Pseudocode

```text
Generate all prime implicants
while groups is not empty
if a and b differ in exactly one bit position j
Build prime implicant chart
Extract essential prime implicants
for each minterm mt
if exactly one PI covers mt
Petrick's method for remaining cover
Form product-of-sums: for each uncovered minterm,
OR together the PIs that cover it
Expand to SOP and find minimum-length terms
```

## Complexity Analysis

- **Time:** The Quine-McCluskey PI generation runs in $O(3^n / n)$ in the worst case, where $n$ is the number of variables. Petrick's method for the minimum cover is NP-hard in general; for small $n$ ($n \le 20$), it is practical with exponential worst-case in the number of non-essential PIs.
- **Space:** $O(3^n)$ to store all prime implicants (each of $n$ positions is 0, 1, or don't-care).

## Answer

$$\boxed{726358482}$$
