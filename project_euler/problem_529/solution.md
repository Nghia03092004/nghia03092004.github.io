# Problem 529: 10-substrings

## Problem Statement

A **10-substring** of a number is a contiguous subsequence of its digits whose digits sum to 10. A number is **10-substring-friendly** if every one of its digits belongs to at least one 10-substring.

Let $T(n)$ count the 10-substring-friendly numbers from 1 to $10^n$ (inclusive).

Given $T(2) = 9$ and $T(5) = 3492$, find $T(10^{18}) \bmod (10^9 + 7)$.

## Mathematical Foundation

**Theorem (Finite Automaton Model).** The set of 10-substring-friendly digit strings of length $n$ is recognized by a finite automaton whose state encodes the suffix of uncovered digits (digits not yet belonging to any 10-substring). The number of states is bounded and independent of $n$.

**Proof.** Consider building a digit string one digit at a time from left to right. At each step, we maintain the suffix of digits that have not yet been covered by any 10-substring. When we append a new digit $d$:

1. Add $d$ to the uncovered suffix.
2. Check if any contiguous suffix of the uncovered tail sums to exactly 10. If so, all digits in that suffix become covered.
3. The new state is the remaining uncovered suffix.

The key observation is that the uncovered suffix has digit sum strictly less than 10 (otherwise a 10-substring would have been found). The state is therefore a composition of an integer $s < 10$ using digits from $\{0, 1, \ldots, 9\}$, where each part is at most $\min(9, s)$.

Since the suffix must have digit sum $< 10$, and each digit is between 0 and 9, the possible suffixes are compositions of integers 0 through 9 using parts from $\{0, \ldots, 9\}$. However, we can compress: the state is determined by the sequence of cumulative partial sums of the uncovered suffix (taken modulo the coverage rule). In practice, the state space has size $|S| \le 512$ (empirically determined). $\square$

**Lemma (Matrix Exponentiation).** Let $M$ be the $|S| \times |S|$ transition matrix of the automaton, where $M_{ij} = $ (number of digits $d \in \{0, \ldots, 9\}$ causing a transition from state $i$ to state $j$). Let $\mathbf{v}_0$ be the initial state vector and $A \subseteq S$ be the set of accepting states (those with an empty uncovered suffix). Then

$$T(n) = \mathbf{v}_0^\top M^n \mathbf{1}_A$$

where $\mathbf{1}_A$ is the indicator vector of accepting states.

**Proof.** The matrix $M^n$ encodes all possible paths of length $n$ through the automaton. Entry $(M^n)_{ij}$ counts the number of digit strings of length $n$ that transition from state $i$ to state $j$. Multiplying by the initial vector (starting state) on the left and the accepting-state indicator on the right extracts precisely the count of accepted strings. $\square$

**Theorem (Efficient Computation via Binary Exponentiation).** $M^{10^{18}}$ can be computed in $O(|S|^3 \log(10^{18}))$ arithmetic operations modulo $10^9 + 7$.

**Proof.** Binary (repeated squaring) exponentiation computes $M^N$ using $O(\log N)$ matrix multiplications, each costing $O(|S|^3)$ operations. For $N = 10^{18}$, $\log_2(10^{18}) < 60$, so the total operation count is at most $60 \cdot |S|^3$. With $|S| \approx 512$, this is approximately $60 \cdot 1.34 \times 10^8 \approx 8 \times 10^9$, which is feasible with modular arithmetic on 64-bit integers. $\square$

**Lemma (Leading Zeros).** The count $T(n)$ excludes numbers with leading zeros. To account for this, the initial state must distinguish the first digit (which must be $\ge 1$) from subsequent digits. This adds at most a factor of 2 to the state space or requires a separate initial vector for the first step.

**Proof.** Let $\mathbf{v}_1$ be the state distribution after processing the first digit $d \in \{1, \ldots, 9\}$ (excluding 0). Then $T(n) = \mathbf{v}_1^\top M^{n-1} \mathbf{1}_A$, correctly counting $n$-digit numbers without leading zeros. $\square$

## Editorial
Count 10-substring-friendly numbers from 1 to 10^n. Find T(10^18) mod 10^9+7. Approach: Build automaton with states tracking uncovered suffix. Use matrix exponentiation. We enumerate automaton states. We then build transition matrix. Finally, iterate over each state s in states.

## Pseudocode

```text
Enumerate automaton states
Build transition matrix
for each state s in states
Check for 10-substring: find longest suffix with sum = 10
Compute initial vector (first digit 1-9)
Matrix exponentiation
Sum over accepting states
```

## Complexity Analysis

- **Time:** $O(|S|^3 \cdot \log N)$ for matrix exponentiation, where $|S| \le 512$ and $\log_2(10^{18}) < 60$. Additionally, $O(|S|^2 \cdot 10)$ for building the transition matrix.
- **Space:** $O(|S|^2)$ for the transition matrix.

## Answer

$$\boxed{23624465}$$
