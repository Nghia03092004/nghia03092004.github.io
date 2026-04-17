# Problem 22: Names Scores

## Problem Statement

Given a text file of over five thousand first names, sort the names into lexicographic (alphabetical) order. Define the *alphabetical value* of a name as the sum of the positional values of its letters (A = 1, B = 2, ..., Z = 26). The *name score* of the $j$-th name in sorted order is $j$ times its alphabetical value. Compute the sum of all name scores.

## Mathematical Development

### Definitions

**Definition 1 (Letter Value).** For an uppercase Latin letter $c$, define
$$\operatorname{val}(c) = \operatorname{ord}(c) - 64,$$
where $\operatorname{ord}$ returns the ASCII codepoint. This yields $\operatorname{val}(\texttt{A}) = 1, \operatorname{val}(\texttt{B}) = 2, \ldots, \operatorname{val}(\texttt{Z}) = 26$.

**Definition 2 (Alphabetical Value).** For a name $w = c_1 c_2 \cdots c_k$ over the alphabet $\{\texttt{A}, \ldots, \texttt{Z}\}$, the *alphabetical value* is
$$\alpha(w) = \sum_{i=1}^{k} \operatorname{val}(c_i).$$

**Definition 3 (Name Score).** Let $w_1 \prec w_2 \prec \cdots \prec w_n$ be the names in lexicographic order. The *name score* of $w_j$ is $S(w_j) = j \cdot \alpha(w_j)$.

### Theorems and Proofs

**Theorem 1 (Well-Definedness).** *The total score $T = \sum_{j=1}^{n} j \cdot \alpha(w_j)$ is uniquely determined by the input set of names.*

**Proof.** The lexicographic order $\preceq$ on $\Sigma^*$ (finite strings over a totally ordered alphabet $\Sigma$) is a total order: for strings $u, v$, compare character-by-character at the first differing position, with shorter strings preceding longer ones if one is a prefix of the other. Since the input names are distinct (an assumption from the problem data), the sorted sequence $w_1 \prec w_2 \prec \cdots \prec w_n$ is unique. Each $\alpha(w_j)$ depends only on the string $w_j$, and the index $j$ depends only on the sorted order. Therefore $T$ is uniquely determined. $\square$

**Lemma 1 (Bounds on $\alpha$).** *For any name $w$ of length $k$ with characters in $\{\texttt{A}, \ldots, \texttt{Z}\}$,*
$$k \leq \alpha(w) \leq 26k.$$

**Proof.** Each character contributes at least $\operatorname{val}(\texttt{A}) = 1$ and at most $\operatorname{val}(\texttt{Z}) = 26$ to the sum. The bounds follow by summing over all $k$ characters. $\square$

**Lemma 2 (ASCII and Dictionary Order).** *Lexicographic comparison of ASCII-encoded uppercase Latin strings coincides with standard dictionary order.*

**Proof.** The ASCII codes of $\texttt{A}, \texttt{B}, \ldots, \texttt{Z}$ are $65, 66, \ldots, 90$, which is a strictly increasing sequence. Lexicographic order is defined by the order on individual characters, so ASCII byte comparison produces the same total order as alphabetical comparison. $\square$

## Algorithm

We read the quoted names, sort them lexicographically, and then evaluate each name score in sorted order. For position $j$, we compute the alphabetical value of the name by summing its letter values and multiply by $j$ before adding to the running total. This is sufficient because the problem definition depends only on the sorted order and those per-name letter sums.

## Pseudocode

```text
Algorithm: Total Name Scores
Require: A finite list of uppercase names.
Ensure: The total of the positional name scores after lexicographic sorting.
1: Sort the names into lexicographic order.
2: For each position j, compute α_j as the sum of alphabetical values of the letters in the j-th name and update T ← T + j · α_j.
3: Return T.
```

## Complexity Analysis

**Proposition.** *The algorithm runs in $O(nL \log n)$ time and $O(nL)$ space, where $n$ is the number of names and $L$ is the maximum name length.*

**Proof.** Sorting $n$ strings by comparison-based sort requires $O(n \log n)$ comparisons, each costing $O(L)$ character inspections in the worst case, for a total of $O(nL \log n)$. The subsequent linear scan computes $\alpha(w_j)$ for each name in $O(L)$ time, contributing $O(nL)$ total, which is dominated by the sorting step. Storage for all $n$ names of length at most $L$ requires $O(nL)$ space. $\square$

## Answer

$$\boxed{871198282}$$
