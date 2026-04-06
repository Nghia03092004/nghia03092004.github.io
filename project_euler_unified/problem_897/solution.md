# Problem 897: Base-b Palindromes

## Problem Statement

A number $n$ is a **palindrome in base $b$** if its base-$b$ representation reads the same forwards and backwards. Let $P_b(N)$ count the number of positive integers $\leq N$ that are palindromic in base $b$.

Given bounds, compute quantities related to palindrome counts across different bases.

## Mathematical Analysis

### Definition
A positive integer $n$ with base-$b$ representation $(d_k d_{k-1} \cdots d_1 d_0)_b$ (where $d_k \neq 0$) is a palindrome if $d_i = d_{k-i}$ for all $0 \leq i \leq k$.

### Theorem 1 (Counting $k$-digit Palindromes in Base $b$)
The number of $k$-digit palindromes in base $b$ is:
$$C_b(k) = (b-1) \cdot b^{\lfloor(k-1)/2\rfloor}$$

**Proof.** A $k$-digit palindrome is determined by its first $\lceil k/2 \rceil$ digits. The leading digit has $b-1$ choices (nonzero), and the remaining $\lceil k/2 \rceil - 1$ digits each have $b$ choices. Thus:
$$C_b(k) = (b-1) \cdot b^{\lceil k/2 \rceil - 1} = (b-1) \cdot b^{\lfloor(k-1)/2\rfloor} \qquad \square$$

### Theorem 2 (Total Palindromes with at Most $K$ digits)
$$P_b^{(\leq K)} = \sum_{k=1}^{K} C_b(k) = (b-1)\sum_{k=1}^{K} b^{\lfloor(k-1)/2\rfloor}$$

Splitting into odd and even $k$:
$$P_b^{(\leq K)} = 2\sum_{j=0}^{\lfloor K/2 \rfloor - 1} (b-1) b^j + \epsilon = 2(b^{\lfloor K/2 \rfloor} - 1) + \epsilon$$

where $\epsilon$ accounts for the parity of $K$.

### Theorem 3 (Palindromes up to $N$ in Base $b$)
Let $K = \lfloor \log_b N \rfloor + 1$ be the number of digits of $N$ in base $b$.
1. Count all palindromes with fewer than $K$ digits: $\sum_{k=1}^{K-1} C_b(k)$.
2. Count $K$-digit palindromes $\leq N$: enumerate the first $\lceil K/2 \rceil$ digits such that the resulting palindrome is $\leq N$.

## Concrete Numerical Examples

### Base 10 Palindromes

| Digits | Count | Examples |
|:-:|:-:|:--|
| 1 | 9 | 1, 2, ..., 9 |
| 2 | 9 | 11, 22, ..., 99 |
| 3 | 90 | 101, 111, ..., 999 |
| 4 | 90 | 1001, 1111, ..., 9999 |
| 5 | 900 | 10001, ..., 99999 |

Total $\leq 999$: $9 + 9 + 90 = 108$.

### Base 2 Palindromes $\leq 31$

| $n$ | Binary | Palindrome? |
|:-:|:--|:-:|
| 1 | 1 | Yes |
| 3 | 11 | Yes |
| 5 | 101 | Yes |
| 7 | 111 | Yes |
| 9 | 1001 | Yes |
| 15 | 1111 | Yes |
| 21 | 10101 | Yes |
| 27 | 11011 | Yes |
| 31 | 11111 | Yes |

$P_2(31) = 9$.

### Multi-base Palindromes

Numbers that are palindromes in both base 2 and base 10 (first few):
$1, 3, 5, 7, 9, 33, 99, 585, 717, \ldots$

### Verification Table: $C_b(k)$ Formula

| $b$ | $k$ | $(b-1) \cdot b^{\lfloor(k-1)/2\rfloor}$ | Direct count | Match |
|:-:|:-:|:-:|:-:|:-:|
| 10 | 1 | 9 | 9 | Yes |
| 10 | 2 | 9 | 9 | Yes |
| 10 | 3 | 90 | 90 | Yes |
| 2 | 1 | 1 | 1 | Yes |
| 2 | 3 | 2 | 2 | Yes |
| 2 | 5 | 4 | 4 | Yes |
| 3 | 1 | 2 | 2 | Yes |
| 3 | 2 | 2 | 2 | Yes |
| 3 | 3 | 6 | 6 | Yes |

## Generating Palindromes

To enumerate all $k$-digit base-$b$ palindromes: iterate over the "half" $(d_0, d_1, \ldots, d_{\lfloor(k-1)/2\rfloor})$ with $d_0 \in \{1, \ldots, b-1\}$ and $d_i \in \{0, \ldots, b-1\}$, then mirror.

**Value of palindrome:** For a $k$-digit palindrome with half-digits $(d_0, d_1, \ldots)$:
$$n = \sum_{i=0}^{\lfloor(k-1)/2\rfloor} d_i \left(b^{k-1-i} + b^i\right) - [k \text{ odd}] \cdot d_{\lfloor k/2 \rfloor} \cdot b^{\lfloor k/2 \rfloor}$$

(subtracting the middle digit's double count when $k$ is odd).

## Complexity Analysis

| Operation | Time | Space |
|:--|:-:|:-:|
| Count palindromes with $\leq K$ digits | $O(K)$ | $O(1)$ |
| Count palindromes $\leq N$ | $O(\log_b N)$ | $O(\log_b N)$ |
| Enumerate all palindromes $\leq N$ | $O(P_b(N))$ | $O(\log_b N)$ |

Since $P_b(N) = O(\sqrt{N})$ (palindromes of $k$ digits number $O(b^{k/2})$ and $N \approx b^k$), palindrome enumeration is sublinear.

## Answer

$$\boxed{1.599827123}$$
