# Problem 59: XOR Decryption

## Problem Statement

A text file has been encrypted by XOR-ing each byte with a repeating key of three lowercase ASCII characters. Given the ciphertext (as a sequence of ASCII codes), decrypt the message and find the sum of the ASCII values in the original plaintext.

## Mathematical Development

**Definition 1 (XOR Cipher).** Let $\mathbf{p} = (p_0, p_1, \ldots, p_{N-1})$ be the plaintext byte sequence and $\mathbf{k} = (k_0, k_1, k_2)$ be the key, where each $k_j \in [97, 122]$ (lowercase ASCII). The ciphertext is

$$c_i = p_i \oplus k_{i \bmod 3}, \qquad i = 0, 1, \ldots, N-1,$$

where $\oplus$ denotes bitwise exclusive-or.

**Theorem 1 (XOR is an Involution).** For any bytes $p, k \in \{0, 1, \ldots, 255\}$:

$$(p \oplus k) \oplus k = p.$$

*Proof.* We verify bitwise. For each bit position, XOR satisfies $x \oplus x = 0$ and $x \oplus 0 = x$ (since XOR is addition in $\mathbb{F}_2$). By associativity:

$$(p \oplus k) \oplus k = p \oplus (k \oplus k) = p \oplus 0 = p. \qquad \square$$

**Corollary 1 (Decryption).** The decryption operation is identical to encryption:

$$p_i = c_i \oplus k_{i \bmod 3}.$$

*Proof.* Immediate from Theorem 1. $\square$

**Definition 2 (Residue Subsequences).** For each $j \in \{0, 1, 2\}$, define the $j$-th residue subsequence of the ciphertext:

$$G_j = (c_i)_{i \equiv j \pmod{3}} = (c_j, c_{j+3}, c_{j+6}, \ldots).$$

Each element of $G_j$ was encrypted with the same key byte $k_j$.

**Theorem 2 (Key Recovery via Frequency Analysis).** If the plaintext is English prose, the most frequent byte in $G_j$ is $32 \oplus k_j$ (where 32 is the ASCII code for the space character). Consequently,

$$k_j = \operatorname{mode}(G_j) \oplus 32.$$

*Proof.* In English text, the space character (ASCII 32) is the most frequent byte, typically constituting 15--20% of all characters. For each position $i \equiv j \pmod{3}$ where $p_i = 32$, the corresponding ciphertext byte is $c_i = 32 \oplus k_j$. Since space is the most frequent plaintext character, $32 \oplus k_j$ is the most frequent value in $G_j$.

By Theorem 1, $k_j = (32 \oplus k_j) \oplus 32 = \operatorname{mode}(G_j) \oplus 32$.

This holds provided no other plaintext character exceeds the frequency of space in the residue class, which is a standard assumption for sufficiently long English text. $\square$

**Lemma 1 (Key Space Cardinality).** The key consists of 3 lowercase letters, giving $|\mathcal{K}| = 26^3 = 17{,}576$ possible keys. This is small enough for brute-force enumeration, but frequency analysis provides a direct $O(N)$ solution.

*Proof.* Each key byte ranges over 26 values (ASCII 97 through 122). Independence of the three bytes gives $26^3 = 17{,}576$. $\square$

**Theorem 3 (Validation).** A decryption with key $\mathbf{k}$ is correct if and only if:

(i) All decrypted bytes lie in the printable ASCII range $[32, 126]$.

(ii) The decrypted text is coherent English prose.

*Proof.* Condition (i) is necessary since the plaintext is stated to be readable text. Condition (ii) is necessary by the problem statement. Among $17{,}576$ candidate keys, the probability that an incorrect key produces both printable ASCII and coherent English is negligible (the space of coherent English texts is a vanishingly small subset of all byte sequences), so the correct key is uniquely determined. $\square$

**Proposition 1 (XOR Algebraic Structure).** The set $\{0, 1, \ldots, 255\}$ under XOR forms an abelian group isomorphic to $(\mathbb{F}_2)^8$. Encryption with a fixed key $k$ is a group automorphism (specifically, translation by $k$). This structure ensures that frequency distributions in the plaintext are perfectly preserved (up to relabeling) in the ciphertext.

*Proof.* XOR is componentwise addition in $\mathbb{F}_2^8$. Translation by $k$ is a bijection on $\mathbb{F}_2^8$ that preserves the frequency histogram: if byte $b$ appears $f(b)$ times in the plaintext, then $b \oplus k$ appears $f(b)$ times in the ciphertext. $\square$

## Algorithm

We exploit the period-3 structure of the repeating XOR key by splitting the ciphertext into three residue classes. In each class, the most common ciphertext byte is interpreted as the encryption of a space character, which directly recovers one key byte by XOR with ASCII 32. Once the three-byte key is known, the entire ciphertext is decrypted in one pass and the plaintext byte values are summed.

## Pseudocode

```text
Algorithm: Recovering a Three-byte XOR Key by Frequency Analysis
Require: A ciphertext encrypted by a repeating three-byte lowercase key.
Ensure: The sum of the ASCII values in the decrypted plaintext.
1: Partition the ciphertext into the three residue classes of positions modulo 3.
2: For each class j, identify its most frequent ciphertext byte m_j and set the key byte k_j ← m_j XOR 32.
3: Form the repeating key (k_0, k_1, k_2) and decrypt each ciphertext byte c_i by p_i ← c_i XOR k_(i mod 3).
4: Compute S ← ∑ p_i over the decrypted plaintext bytes.
5: Return S.
```

## Complexity Analysis

**Theorem 4 (Time Complexity).** The frequency-analysis approach runs in $O(N)$ time, where $N$ is the ciphertext length.

*Proof.* Step 1 scans each of the $N$ ciphertext bytes exactly once, building three frequency tables of size at most 256. Finding the mode of each table costs $O(256) = O(1)$. Step 2 decrypts in a single $O(N)$ pass. Step 3 validates in $O(N)$. Step 4 sums in $O(N)$. Total: $O(N)$. $\square$

**Space:** $O(N)$ for storing the ciphertext and plaintext arrays. The frequency tables require $O(256) = O(1)$ auxiliary space.

*Remark.* A brute-force approach testing all $26^3 = 17{,}576$ keys would cost $O(|\mathcal{K}| \cdot N) = O(17{,}576 \cdot N)$, which is also efficient for the given input size.

## Answer

$$\boxed{129448}$$
