# Problem 42: Coded Triangle Numbers

## Problem Statement

The $n$-th triangle number is $T_n = \frac{n(n+1)}{2}$. By converting each letter in a word to its alphabetical position ($\text{A}=1, \text{B}=2, \ldots, \text{Z}=26$) and summing, we obtain a *word value*. A word is a *triangle word* if its word value is a triangle number. Using the provided word list, determine how many words are triangle words.

## Mathematical Development

### Definitions

**Definition 1.** The *alphabetical value* of a letter $c \in \{\text{A}, \ldots, \text{Z}\}$ is $\operatorname{pos}(c) = \operatorname{ord}(c) - \operatorname{ord}(\text{A}) + 1 \in \{1, 2, \ldots, 26\}$.

**Definition 2.** The *word value* of a word $w = c_1 c_2 \cdots c_L$ is $V(w) = \sum_{i=1}^{L} \operatorname{pos}(c_i)$.

**Definition 3.** A positive integer $v$ is a *triangular number* if $v = T_n = \frac{n(n+1)}{2}$ for some $n \in \mathbb{Z}^+$.

### Theoretical Development

**Theorem 1 (Triangular number characterization).** *A positive integer $v$ is triangular if and only if $8v + 1$ is a perfect square.*

**Proof.** ($\Rightarrow$) Suppose $v = \frac{n(n+1)}{2}$ for some $n \in \mathbb{Z}^+$. Then
$$8v + 1 = 4n(n+1) + 1 = 4n^2 + 4n + 1 = (2n+1)^2,$$
which is a perfect square.

($\Leftarrow$) Suppose $8v + 1 = m^2$ for some $m \in \mathbb{Z}^+$. Since $8v + 1 \equiv 1 \pmod{8}$ and even squares satisfy $m^2 \equiv 0$ or $4 \pmod{8}$, we conclude $m$ is odd. Write $m = 2n + 1$ for some $n \in \mathbb{Z}_{\geq 0}$. Then
$$8v + 1 = (2n+1)^2 = 4n^2 + 4n + 1 \implies v = \frac{n(n+1)}{2} = T_n.$$
Since $v \geq 1$, we have $n \geq 1$, confirming $v$ is triangular. $\square$

**Corollary 1 (Constant-time test).** *The triangular number test requires $O(1)$ arithmetic operations: compute $\Delta = 8v + 1$, take the integer square root $s = \lfloor \sqrt{\Delta} \rfloor$, and verify $s^2 = \Delta$.*

**Proof.** Immediate from Theorem 1. The integer square root and one multiplication-comparison suffice. $\square$

**Lemma 1 (Word value bounds).** *For a non-empty word of length $L$, the word value satisfies $L \leq V(w) \leq 26L$.*

**Proof.** Each letter contributes at least $\operatorname{pos}(\text{A}) = 1$ and at most $\operatorname{pos}(\text{Z}) = 26$. Summing over $L$ letters gives $L \leq V(w) \leq 26L$. $\square$

**Remark.** For the Project Euler word list, the maximum word length is $14$ (e.g., "SIMULTANEOUSLY"), giving $V(w) \leq 26 \cdot 14 = 364$. The largest triangle number not exceeding $364$ is $T_{26} = 351$, so we need only consider triangle numbers $T_1, T_2, \ldots, T_{26}$.

## Editorial

We read the word list, compute the value of each word by summing the alphabetical positions of its letters, and test the resulting integer with the triangular-number criterion from Theorem 1. Every word whose value satisfies that criterion contributes one to the running count, and the final count is the number of triangle words in the file.

## Pseudocode

```text
Algorithm: Counting Triangle Words
Require: A finite word list over the alphabet {A, B, ..., Z}.
Ensure: The number of words whose alphabetical values are triangular numbers.
1: Read the word list and initialize count ← 0.
2: For each word w in the list, compute its word value v ← sum of the letter positions in w.
3: Test whether 8v + 1 is a perfect square.
4: If the test succeeds, increment count.
5: Return count.
```

## Complexity Analysis

**Proposition (Time complexity).** *The algorithm runs in $O(W \cdot \bar{L})$ time, where $W$ is the number of words and $\bar{L}$ is the average word length.*

**Proof.** For each of the $W$ words, computing the word value requires summing $L_i$ letter values, where $L_i$ is the length of the $i$-th word. The triangular number test is $O(1)$ by Corollary 1. Total time:
$$\sum_{i=1}^{W} O(L_i) = O\!\left(\sum_{i=1}^{W} L_i\right) = O(W \cdot \bar{L}).$$
$\square$

**Proposition (Space complexity).** *The algorithm uses $O(W \cdot \bar{L})$ space for storing the word list, plus $O(1)$ auxiliary space.*

## Answer

$$\boxed{162}$$
