# Problem 315: Digital Root Clocks

## Problem Statement

Sam and Max have digital clocks with 7-segment displays. Starting from a number, they repeatedly compute the digit sum until a single digit remains (the digital root chain). Sam's clock turns off all segments then turns on the new display at each step. Max's clock only toggles differing segments between consecutive displays. For all primes $p$ with $10^7 < p < 2 \times 10^7$, compute:

$$\sum_{p} \bigl(\text{Sam}(p) - \text{Max}(p)\bigr)$$

## Mathematical Foundation

**Definition (Segment Encoding).** Each decimal digit is represented by a 7-bit mask over segments $\{a, b, c, d, e, f, g\}$:

| Digit | Segments | Count | Bitmask |
|-------|----------|-------|---------|
| 0 | a,b,c,d,e,f | 6 | `0x3F` |
| 1 | b,c | 2 | `0x06` |
| 2 | a,b,d,e,g | 5 | `0x5B` |
| 3 | a,b,c,d,g | 5 | `0x4F` |
| 4 | b,c,f,g | 4 | `0x66` |
| 5 | a,c,d,f,g | 5 | `0x6D` |
| 6 | a,c,d,e,f,g | 6 | `0x7D` |
| 7 | a,b,c,f | 4 | `0x27` |
| 8 | a,b,c,d,e,f,g | 7 | `0x7F` |
| 9 | a,b,c,d,f,g | 6 | `0x6F` |

Note: digit 7 uses 4 segments (including the top-left vertical $f$), per the Project Euler specification.

**Definition (Segment Count Functions).** For a number $n$ with digits $d_1 d_2 \cdots d_k$, define:
- $\text{seg}(n) = \sum_{i=1}^{k} |M(d_i)|$, the total number of active segments,
- $M(d)$ = bitmask of digit $d$.

**Lemma 1 (Sam's Cost).** *For the digital root chain $n_0, n_1, \ldots, n_K$ (where $n_0 = p$, $n_K$ is a single digit), Sam's total segment transitions are:*
$$\text{Sam}(p) = 2 \sum_{i=0}^{K} \text{seg}(n_i).$$

**Proof.** Sam turns on all segments of $n_i$ (cost $\text{seg}(n_i)$), then turns them all off (cost $\text{seg}(n_i)$), for each $i = 0, 1, \ldots, K$. Total: $2\sum_i \text{seg}(n_i)$. $\square$

**Lemma 2 (Max's Cost).** *Max's total segment transitions are:*
$$\text{Max}(p) = \text{seg}(n_0) + \sum_{i=0}^{K-1} \text{ham}(n_i, n_{i+1}) + \text{seg}(n_K),$$
*where $\text{ham}(n_i, n_{i+1})$ is the number of segments that differ between the right-aligned digit-by-digit bitmask representations of $n_i$ and $n_{i+1}$ (the Hamming distance of the composite bitmasks, padding shorter numbers with zero on the left).*

**Proof.** Max initially turns on $n_0$ from blank (cost $\text{seg}(n_0)$). At each transition $n_i \to n_{i+1}$, Max toggles only the differing segments (cost $\text{ham}(n_i, n_{i+1})$). Finally, Max turns off $n_K$ to blank (cost $\text{seg}(n_K)$). $\square$

**Theorem (Difference Identity).** *For any two bitmasks $A$ and $B$:*
$$|A| + |B| = |A \oplus B| + 2|A \wedge B|$$
*where $\oplus$ is XOR (symmetric difference) and $\wedge$ is AND (intersection). Consequently:*
$$\text{Sam}(p) - \text{Max}(p) = 2 \sum_{i=0}^{K-1} \text{common}(n_i, n_{i+1})$$
*where $\text{common}(n_i, n_{i+1})$ is the number of segments shared between consecutive displays (computed digit-by-digit, right-aligned).*

**Proof.** For bitmasks $A, B \subseteq \{0,1\}^7$: every bit is either in $A \cap B$, in $A \setminus B$, or in $B \setminus A$. The XOR counts bits in exactly one of $A, B$; the AND counts bits in both. So $|A| + |B| = |A \oplus B| + 2|A \wedge B|$.

Now expand the difference:
$$\text{Sam} - \text{Max} = 2\sum_{i=0}^{K} \text{seg}(n_i) - \text{seg}(n_0) - \sum_{i=0}^{K-1}\text{ham}(n_i, n_{i+1}) - \text{seg}(n_K).$$

Rewriting: $\text{Sam} - \text{Max} = \text{seg}(n_0) + 2\sum_{i=1}^{K-1}\text{seg}(n_i) + \text{seg}(n_K) - \sum_{i=0}^{K-1}\text{ham}(n_i, n_{i+1})$.

Using $\text{seg}(n_i) + \text{seg}(n_{i+1}) = \text{ham}(n_i, n_{i+1}) + 2\,\text{common}(n_i, n_{i+1})$, we telescope:
$$\text{Sam} - \text{Max} = 2\sum_{i=0}^{K-1}\text{common}(n_i, n_{i+1}). \quad \square$$

## Algorithm

```
Input: primes p in (10^7, 2*10^7)
Output: sum of Sam(p) - Max(p) over all such primes

1. Sieve primes in (10^7, 2*10^7) using Sieve of Eratosthenes.
2. Precompute seg_count[d] and bitmask[d] for d = 0..9.
3. For each prime p:
     a. Build the digital root chain: n_0 = p, n_{i+1} = digit_sum(n_i),
        stopping when n_i is a single digit.
     b. For each consecutive pair (n_i, n_{i+1}):
        - Align digits right, compute bitwise AND of corresponding
          digit bitmasks, sum the popcount.
     c. Add 2 * total_common to the global sum.
4. Return global sum.
```

## Complexity Analysis

- **Time:** $O(N \log \log N)$ for the sieve where $N = 2 \times 10^7$. Per prime: $O(\log p)$ for the chain (at most 3--4 steps, each with $\le 8$ digits). Total: $O(N \log \log N)$.
- **Space:** $O(N)$ for the sieve.

## Answer

$$\boxed{13625242}$$
