# Problem 200: Find the 200th Prime-proof Sqube Containing "200"

## Problem Statement

A **sqube** is a number of the form $p^2 q^3$ or $p^3 q^2$ where $p$ and $q$ are distinct primes.

A number is **prime-proof** if no single digit substitution (replacing exactly one digit with a different digit) produces a prime number.

Find the 200th smallest sqube that:
1. Contains the substring "200"
2. Is prime-proof

## Mathematical Foundation

**Theorem 1.** *(Sqube Characterization.) A positive integer $n$ is a sqube if and only if $n = p^2 q^3$ for some pair of distinct primes $p, q$. Note that $p^3 q^2 = q^2 p^3$ is also a sqube (with roles swapped), so every sqube has exactly the form $a^2 b^3$ for distinct primes $a, b$.*

**Proof.** By definition. The factorization $n = p^2 q^3$ determines $p$ and $q$ uniquely (up to the swap $p^2 q^3 \leftrightarrow q^3 p^2$, which is the same number). The form $p^3 q^2$ is simply $q^2 p^3$ with $a = q, b = p$. $\square$

**Theorem 2.** *(Deterministic Primality via Miller-Rabin.) For $n < 3.2 \times 10^{14}$, the Miller-Rabin test with witnesses $\{2, 3, 5, 7, 11, 13\}$ is deterministic (no pseudoprimes exist below this bound for this witness set).*

**Proof.** This is a computational result. Jaeschke (1993) proved that the witnesses $\{2, 3, 5, 7, 11, 13\}$ suffice for $n < 3.2 \times 10^{14}$. Since our squbes and their digit-substituted variants are below $10^{13}$, this witness set provides a rigorous primality test. $\square$

**Lemma 1.** *(Prime-Proof Condition.) A number $n$ with decimal digits $d_1 d_2 \cdots d_m$ is prime-proof if and only if for every position $i \in \{1, \ldots, m\}$ and every replacement digit $d' \in \{0, 1, \ldots, 9\} \setminus \{d_i\}$ (with $d' \neq 0$ when $i = 1$), the resulting number $n'$ is composite.*

**Proof.** Direct from the definition: "replacing exactly one digit with a different digit" means choosing one position and one alternative digit. The leading-digit constraint $d' \neq 0$ when $i = 1$ ensures $n'$ has the same number of digits (no leading zeros). $\square$

**Lemma 2.** *(Digit Substitution Count.) For a number with $m$ digits, the number of single-digit substitutions is at most $9m - 1$ (position 1 has 8 alternatives if $d_1 \neq 0$, all other positions have 9 alternatives each; minus adjustments for the leading digit).*

**Proof.** Position 1: $\{1, \ldots, 9\} \setminus \{d_1\}$ gives 8 choices. Positions $2, \ldots, m$: $\{0, \ldots, 9\} \setminus \{d_i\}$ gives 9 choices each. Total: $8 + 9(m-1) = 9m - 1$. $\square$

## Editorial
A sqube is p^2*q^3 or p^3*q^2 for distinct primes p, q. Prime-proof means no single digit change produces a prime. We generate all squbes below B. We then note: p^3 * q^2 is generated as q^2 * p^3 with roles swapped. Finally, filter squbes containing "200".

## Pseudocode

```text
Generate all squbes below B
Note: p^3 * q^2 is generated as q^2 * p^3 with roles swapped
Filter squbes containing "200"
Check prime-proof condition
for s in candidates
for i from 0 to m-1
for d from 0 to 9
```

## Complexity Analysis

- **Time:** Sqube generation: iterate over primes $q \leq B^{1/3}$ and for each, primes $p \leq \sqrt{B/q^3}$. The number of squbes below $B$ is $O(B^{1/2}/\log B + B^{1/3}/\log B)$ by the prime counting function. Filtering for "200": $O(S \log S)$ for sorting $S$ candidates. Prime-proof check: $O(m)$ substitutions per candidate, each requiring a Miller-Rabin test in $O(\log^2 n)$. The dominant cost is the sorting and prime-proof checking.
- **Space:** $O(S)$ to store all squbes containing "200", where $S$ is their count.

## Answer

$$\boxed{229161792008}$$
