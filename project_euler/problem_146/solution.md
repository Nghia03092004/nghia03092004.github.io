# Problem 146: Investigating a Prime Pattern

## Problem Statement

The smallest positive integer $n$ for which the numbers $n^2+1$, $n^2+3$, $n^2+7$, $n^2+9$, $n^2+13$, and $n^2+27$ are consecutive primes is 10.

Find the sum of all integers $n$, $0 < n < 150{,}000{,}000$, such that $n^2+1$, $n^2+3$, $n^2+7$, $n^2+9$, $n^2+13$, and $n^2+27$ are consecutive primes.

## Mathematical Development

**Theorem 1 (Parity constraint).** *For $n^2 + 1$ to be an odd prime (greater than 2), $n$ must be even.*

**Proof.** If $n$ is odd, $n^2$ is odd, so $n^2 + 1$ is even. Since $n^2 + 1 > 2$ for $n > 1$, it would be composite. For $n = 1$: $n^2 + 1 = 2$ is prime but $n^2 + 3 = 4$ is not. Hence $n$ must be even. $\square$

**Theorem 2 (Mod 3 constraint).** *We must have $n \not\equiv 0 \pmod{3}$.*

**Proof.** If $3 \mid n$, then $n^2 \equiv 0 \pmod{3}$, so $n^2 + 3 \equiv 0 \pmod{3}$. Since $n^2 + 3 > 3$ for $n > 0$, it would be composite. $\square$

**Theorem 3 (Mod 5 constraint).** *We must have $n \equiv 0 \pmod{5}$.*

**Proof.** The quadratic residues mod 5 are $\{0, 1, 4\}$. We check each:
- $n^2 \equiv 0 \pmod{5}$: offsets give $\{1,3,2,4,3,2\} \pmod{5}$ -- none zero.
- $n^2 \equiv 1 \pmod{5}$: offsets give $\{2,4,3,0,4,3\} \pmod{5}$ -- $n^2 + 9 \equiv 0$, composite.
- $n^2 \equiv 4 \pmod{5}$: offsets give $\{0,2,1,3,2,1\} \pmod{5}$ -- $n^2 + 1 \equiv 0$, composite.

Only $n \equiv 0 \pmod{5}$ is viable. $\square$

**Lemma 1 (Combined congruence).** *From Theorems 1--3: $n \equiv 0 \pmod{2}$, $n \not\equiv 0 \pmod{3}$, $n \equiv 0 \pmod{5}$. Thus $n \equiv 10$ or $20 \pmod{30}$.*

**Proof.** By the Chinese Remainder Theorem, $n \equiv 0 \pmod{10}$ (from $2 \mid n$ and $5 \mid n$). Combined with $n \not\equiv 0 \pmod{3}$: $n \bmod 30 \in \{10, 20\}$. $\square$

**Theorem 4 (Mod 7 refinement).** *Further modular analysis with $p = 7$ restricts $n \bmod 7$ to specific residues.*

**Proof.** Quadratic residues mod 7 are $\{0, 1, 2, 4\}$. For each residue $r = n^2 \bmod 7$, check whether any offset $k \in \{1,3,7,9,13,27\}$ satisfies $r + k \equiv 0 \pmod{7}$:
- $r = 0$: $0 + 7 \equiv 0$ -- $n^2 + 7$ divisible by 7. Invalid (unless $n^2 + 7 = 7$, impossible for $n > 0$).
- $r = 1$: offsets mod 7 are $\{2,4,1,3,0,0\}$ -- $n^2+13 \equiv 0$ and $n^2+27 \equiv 0$. Invalid.
- $r = 2$: offsets mod 7 are $\{3,5,2,4,1,1\}$ -- none zero. Valid.
- $r = 4$: offsets mod 7 are $\{5,0,4,6,3,3\}$ -- $n^2+3 \equiv 0$. Invalid.

So $n^2 \equiv 2 \pmod{7}$, i.e., $n \equiv 3$ or $4 \pmod{7}$. $\square$

**Theorem 5 (Consecutive primes condition).** *The "consecutive primes" requirement means that for all $k \in \{5, 11, 15, 17, 19, 21, 23, 25\}$, $n^2 + k$ must be composite.*

**Proof.** "Consecutive primes" means no prime exists strictly between $n^2+1$ and $n^2+27$ other than the six listed values. The integers between 1 and 27 not in $\{1,3,7,9,13,27\}$ are $\{2,4,5,6,8,10,11,12,14,15,16,17,18,19,20,21,22,23,24,25,26\}$. Even offsets produce even $n^2 + k$ (since $n$ is even), which are automatically composite (greater than 2). The odd offsets to check are $\{5, 11, 15, 17, 19, 21, 23, 25\}$. $\square$

**Lemma 2 (Miller--Rabin sufficiency).** *Deterministic Miller--Rabin with witnesses $\{2, 3, 5, 7, 11, 13\}$ correctly determines primality for all integers up to $3.317 \times 10^{24}$, which exceeds our maximum value of $n^2 + 27 < 2.25 \times 10^{16}$.*

**Proof.** This follows from the result of Jaeschke (1993) and subsequent computational verification: the witnesses $\{2, 3, 5, 7, 11, 13\}$ form a sufficient set for numbers below $3.317 \times 10^{24}$. $\square$

## Editorial
The key to making the search feasible is to eliminate almost every integer before any expensive primality testing happens. The congruence arguments force $n$ into a tiny set of residue classes modulo $210=\operatorname{lcm}(2,3,5,7)$, so the program first precomputes exactly which residues survive those modular filters.

The main loop then scans candidates of the form $n=\text{base}+r$ using only those residues. For each candidate, it first checks that the six required values
$$n^2+1,\ n^2+3,\ n^2+7,\ n^2+9,\ n^2+13,\ n^2+27$$
are prime. Only after that does it verify the "consecutive primes" condition by testing the odd offsets in between that are supposed to be composite. Because the candidate set is already heavily reduced, deterministic Miller-Rabin primality tests are fast enough for the full bound.

## Pseudocode

```text
Set the search limit to 150000000.
Precompute all residues modulo 210 that satisfy the parity, mod 3, mod 5, and mod 7 filters.

Prepare the two offset lists:
    one list for values that must be prime,
    one list for odd intermediate values that must be composite.

Initialize the answer to zero.
For each multiple of 210 below the limit:
    For each admissible residue:
        Form the candidate $n$ and skip it if it falls outside the open interval $(0,150000000)$.
        Compute $n^2$ once.

        Test every required offset for primality.
        If any required value is composite, reject this candidate immediately.

        Then test every forbidden odd offset.
        If any forbidden value is prime, reject this candidate.

        Otherwise add $n$ to the running total.

Return the total.
```

## Complexity Analysis

- **Time:** The modular sieve reduces candidates to $O(B / 210 \times |\text{valid residues}|) \approx O(B / 50)$. Each candidate requires $O(14)$ Miller--Rabin tests, each costing $O(\log^2(n^2)) = O(\log^2 n)$ with modular exponentiation. Total: $O(B \log^2 B / 50)$.
- **Space:** $O(1)$ beyond the list of valid residues (which has $O(1)$ elements).

## Answer

$$\boxed{676333270}$$
