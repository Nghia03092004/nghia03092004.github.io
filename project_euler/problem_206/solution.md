# Problem 206: Concealed Square

## Problem Statement

Find the unique positive integer whose square has the form $1\_2\_3\_4\_5\_6\_7\_8\_9\_0$, where each underscore represents a single digit.

## Mathematical Foundation

**Theorem (Divisibility by 10).** *If $n^2$ has the form $1\_2\_3\_4\_5\_6\_7\_8\_9\_0$, then $10 \mid n$.*

**Proof.** The last digit of $n^2$ is 0. Since $n^2 \equiv 0 \pmod{10}$, we must have $10 \mid n$ (for if $\gcd(n, 5) = 1$ and $\gcd(n, 2) = 1$, then $n^2 \equiv 1$ or other nonzero residues mod 10). More precisely, $n^2 \equiv 0 \pmod{10}$ requires $5 \mid n$ and $2 \mid n$, hence $10 \mid n$. $\square$

**Theorem (Residue Constraint mod 100).** *Write $n = 10m$. Then $n^2$ has the prescribed form only if $m \equiv 3 \pmod{10}$ or $m \equiv 7 \pmod{10}$, i.e., $n \equiv 30 \pmod{100}$ or $n \equiv 70 \pmod{100}$.*

**Proof.** With $n = 10m$, we have $n^2 = 100m^2$. The pattern requires the digit at position 16 (0-indexed from the left in the 19-digit number) to be 9, which means the tens digit of $n^2$ must fit $\_9\_0$. Equivalently, the units digit of $m^2$ must be 9. Since $k^2 \equiv 9 \pmod{10}$ iff $k \equiv 3$ or $k \equiv 7 \pmod{10}$, we get $m \equiv 3$ or $7 \pmod{10}$. $\square$

**Lemma (Search Bounds).** *The value of $n$ satisfies $1010101010 \leq n \leq 1389199190$.*

**Proof.** The 19-digit square $n^2$ lies in the interval $[1020304050607080900,\; 1929394959697989990]$. Taking square roots:
$$\lceil\sqrt{1020304050607080900}\rceil = 1010101010, \qquad \lfloor\sqrt{1929394959697989990}\rfloor = 1389199189.$$
Since $n$ must be divisible by 10, the upper bound rounds to $1389199190$. $\square$

**Theorem (Uniqueness).** *There exists exactly one $n$ in the search range satisfying the pattern constraint.*

**Proof.** The search is exhaustive over approximately $7.58 \times 10^6$ candidates (values of $n$ in $[1010101010, 1389199190]$ with $n \equiv 30$ or $70 \pmod{100}$). Computational verification confirms exactly one solution: $n = 1389019170$, with $n^2 = 1929374254627488900$, which matches the pattern $1\_2\_3\_4\_5\_6\_7\_8\_9\_0$. $\square$

## Editorial
We lo = 1010101030  (first multiple of 10 in range with m ending in 3). Candidates are generated from the derived formulas, filtered by the required conditions, and processed in order until the desired value is obtained.

## Pseudocode

```text
Input: pattern 1_2_3_4_5_6_7_8_9_0
Output: the unique n such that n^2 matches the pattern
lo = 1010101030  (first multiple of 10 in range with m ending in 3)
For n = lo to hi, stepping by 100:
```

## Complexity Analysis

- **Time:** $O((n_{\max} - n_{\min})/50) \approx O(7.58 \times 10^6)$ candidate evaluations. Each evaluation involves one multiplication and at most 10 digit extractions, all $O(1)$.
- **Space:** $O(1)$.

## Answer

$$\boxed{1389019170}$$
