# Problem 413: One-child Numbers

## Problem Statement

A $d$-digit positive number (no leading zeros) is a **one-child number** if exactly one of its substrings is divisible by $d$.

**Examples:**
- $5671$ is a 4-digit one-child number: among its substrings, only $56$ is divisible by $4$.
- $104$ is a 3-digit one-child number: only $0$ is divisible by $3$.
- $1132451$ is a 7-digit one-child number: only $245$ is divisible by $7$.

Let $F(N)$ be the number of one-child numbers less than $N$.

**Given:** $F(10) = 9$, $F(10^3) = 389$, $F(10^7) = 277674$.

**Find:** $F(10^{19})$.

## Mathematical Foundation

**Theorem 1 (Substring Divisibility via Suffix Remainders).** For a $d$-digit number with digits $a_1 a_2 \cdots a_d$, define the suffix remainder at position $j$ for start $k$ as $S_k(j) = V(k,j) \bmod d$ where $V(k,j) = \sum_{i=k}^{j} a_i \cdot 10^{j-i}$. When appending digit $a_{j+1}$, the suffix remainders update as
$$
S_k(j+1) = (S_k(j) \cdot 10 + a_{j+1}) \bmod d, \quad k \le j,
$$
and a new length-1 suffix begins: $S_{j+1}(j+1) = a_{j+1} \bmod d$.

**Proof.** We have $V(k,j+1) = V(k,j) \cdot 10 + a_{j+1}$, so $S_k(j+1) = V(k,j+1) \bmod d = (V(k,j) \cdot 10 + a_{j+1}) \bmod d = (S_k(j) \cdot 10 + a_{j+1}) \bmod d$. $\square$

**Lemma 1 (Coprime Case Reduction).** When $\gcd(10, d) = 1$, define $R(j) = V(1,j) \cdot 10^{d-j} \bmod d$. Then
$$
V(i,j) \equiv 0 \pmod{d} \iff R(j) \equiv R(i-1) \pmod{d}.
$$
Consequently, the number of substrings divisible by $d$ equals $\sum_{r=0}^{d-1} \binom{c_r}{2}$ where $c_r = |\{j \in \{0,1,\dots,d\} : R(j) \equiv r \pmod{d}\}|$.

**Proof.** Since $\gcd(10, d) = 1$, the factor $10^{d-j}$ is invertible modulo $d$. We have $V(i,j) = (R(j) - R(i-1)) \cdot (10^{d-j})^{-1} \cdot 10^{d-j} \bmod d$. More precisely, $V(i,j) \cdot 10^{d-j} \equiv R(j) - R(i-1) \pmod{d}$, and since $10^{d-j}$ is invertible, $V(i,j) \equiv 0$ iff $R(j) \equiv R(i-1)$. The collision count follows from standard combinatorial counting of equal pairs. $\square$

**Theorem 2 (Digit DP with Histogram).** The count of $d$-digit one-child numbers can be computed via a digit dynamic programming approach where the state at position $j$ consists of:
1. The histogram $h[r] = |\{k \le j : S_k(j) \equiv r \pmod{d}\}|$ for $r = 0, 1, \dots, d-1$.
2. The cumulative zero-hit count $Z$ (number of substrings found divisible by $d$ so far).

A number is one-child iff $Z = 1$ at position $d$. States with $Z \ge 2$ are pruned.

**Proof.** The histogram $h$ completely determines how many new divisible substrings arise when the next digit is appended: exactly $h'[0]$ new divisible substrings appear (where $h'$ is the transformed histogram after appending). The pruning is valid because $Z$ is monotonically non-decreasing, so once $Z \ge 2$ it remains $\ge 2$. $\square$

**Lemma 2 (Non-coprime Case via CRT).** When $d = 2^a \cdot 5^b \cdot m$ with $\gcd(m, 10) = 1$, divisibility by $d$ decomposes via the Chinese Remainder Theorem into simultaneous divisibility by $2^a$, $5^b$, and $m$. Divisibility of $V(i,j)$ by $2^a$ depends only on the last $a$ digits, and by $5^b$ on the last $b$ digits. The modular-$m$ component uses the coprime-case reduction.

**Proof.** By CRT, $V(i,j) \equiv 0 \pmod{d}$ iff $V(i,j) \equiv 0 \pmod{2^a}$, $V(i,j) \equiv 0 \pmod{5^b}$, and $V(i,j) \equiv 0 \pmod{m}$ simultaneously. Since $10^a \equiv 0 \pmod{2^a}$, the value $V(i,j) \bmod 2^a$ depends only on the last $\min(a, j-i+1)$ digits. Similarly for $5^b$. The $m$-component follows from Lemma~1 since $\gcd(10,m) = 1$. $\square$

## Editorial
A d-digit positive number is a "one-child number" if exactly one of its substrings is divisible by d. F(N) = count of one-child numbers less than N. Given: F(10)=9, F(10^3)=389, F(10^7)=277674. Find: F(10^19). Approach: Digit DP for each digit length d from 1 to 19. For each d, count d-digit numbers where exactly one substring is divisible by d. Key insight for gcd(d, 10) = 1: Define R(j) = (number formed by first j digits) * 10^(d-j) mod d. Then substring(i+1..j) is divisible by d iff R(j) == R(i) mod d. So we need exactly one pair (i,j) with 0<=i<j<=d where R(i)==R(j). This means among R(0), R(1), ..., R(d) (d+1 values mod d), exactly one residue appears exactly twice, rest appear at most once. For d with gcd(d,10) > 1, we handle separately using decomposition. We use dynamic programming over the digit positions. We then state: (histogram_mod_d, zero_hit_count). Finally, transform histogram: h'[(r*10 + digit) mod d] += h[r].

## Pseudocode

```text
DP over digit positions
State: (histogram_mod_d, zero_hit_count)
Transform histogram: h'[(r*10 + digit) mod d] += h[r]
Add new length-1 suffix: hist'[digit mod d] += 1
Count new zero-hits
```

## Complexity Analysis

- **Time:** For each digit length $d$, the DP processes $O(|\mathcal{S}| \cdot 10)$ transitions per position, where $|\mathcal{S}|$ is the number of active states. With the $Z \ge 2$ pruning, $|\mathcal{S}|$ is dramatically reduced from the theoretical maximum. For the coprime case with $d$ up to $19$, the state space involves residue histograms compressible to $O(d \cdot 2^d)$ states. Total time is feasible for $d \le 19$.
- **Space:** $O(|\mathcal{S}|)$ per digit length, stored in hash maps.

## Answer

$$\boxed{3079418648040719}$$
