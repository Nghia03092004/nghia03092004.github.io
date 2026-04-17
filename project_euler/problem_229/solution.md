# Problem 229: Four Representations Using Squares

## Problem Statement

Find the count of integers $n \leq 2 \times 10^9$ that can **simultaneously** be represented in all four forms:

1. $n = a^2 + b^2$
2. $n = a^2 + 7c^2$
3. $n = a^2 + 11d^2$
4. $n = a^2 + 13e^2$

where $a, b, c, d, e$ are non-negative integers (the values of $a$ may differ between representations).

## Mathematical Foundation

**Theorem 1 (Representability as $a^2 + kb^2$).** *An integer $n$ is representable as $a^2 + kb^2$ (with $a, b \geq 0$) if and only if, for every prime $p$ dividing $n$ to an odd power, $p$ is representable as $a^2 + kb^2$ (up to conditions on the class group of $\mathbb{Z}[\sqrt{-k}]$).*

**Proof.** (Sketch) This follows from the theory of binary quadratic forms. For class number 1 cases (e.g., $k = 1$), a prime $p$ is represented iff $-k$ is a quadratic residue mod $p$ (by Fermat's theorem on sums of two squares and its generalizations). For general $k$, the genus theory of quadratic forms determines representability. The full proof requires class field theory. $\square$

**Theorem 2 (Sum of Two Squares).** *A positive integer $n$ is a sum of two squares iff every prime factor $p \equiv 3 \pmod{4}$ appears to an even power in the factorization of $n$.*

**Proof.** ($\Rightarrow$) If $p \equiv 3 \pmod{4}$ and $p \mid a^2 + b^2$, then $a^2 \equiv -b^2 \pmod{p}$. If $p \nmid b$, then $(ab^{-1})^2 \equiv -1 \pmod{p}$, contradicting $-1$ being a QNR mod $p$. Hence $p \mid b$, so $p \mid a$, and $p^2 \mid n$. By induction, $p$ appears to even power.

($\Leftarrow$) By the multiplicativity of the norm in $\mathbb{Z}[i]$: $(a^2+b^2)(c^2+d^2) = (ac \pm bd)^2 + (ad \mp bc)^2$. Primes $p \equiv 1 \pmod{4}$ split in $\mathbb{Z}[i]$ as $p = \pi\bar{\pi}$ with $N(\pi) = p$, so $p$ is a sum of two squares. The prime 2 is a sum of squares ($1^2 + 1^2$). Primes $p \equiv 3 \pmod{4}$ to even powers give $p^{2k} = (p^k)^2 + 0^2$. $\square$

**Theorem 3 (Sieve Correctness).** *For each form $a^2 + kb^2$ with $k \in \{1, 7, 11, 13\}$, the set of representable $n \leq N$ can be computed by enumerating all pairs $(a, b)$ with $a^2 + kb^2 \leq N$.*

**Proof.** By definition, $n$ is representable iff there exist non-negative integers $a, b$ with $n = a^2 + kb^2$. The enumeration is exhaustive: $b$ ranges from 0 to $\lfloor\sqrt{N/k}\rfloor$, and for each $b$, $a$ ranges from 0 to $\lfloor\sqrt{N - kb^2}\rfloor$. $\square$

**Lemma 1 (Bit Array Intersection).** *The count of $n \leq N$ representable in all four forms equals the number of set bits in $B_1 \wedge B_7 \wedge B_{11} \wedge B_{13}$, where $B_k$ is the characteristic bit array of numbers representable as $a^2 + kb^2$.*

**Proof.** $n$ is in the intersection iff it is representable in each form, iff the corresponding bit is set in every $B_k$. The bitwise AND computes set intersection. $\square$

**Lemma 2 (Pair Count).** *For form $a^2 + kb^2 \leq N$, the number of pairs $(a, b)$ is*

$$\sum_{b=0}^{\lfloor\sqrt{N/k}\rfloor} \left(\lfloor\sqrt{N - kb^2}\rfloor + 1\right) = \Theta\!\left(\frac{N}{\sqrt{k}}\right).$$

**Proof.** The sum approximates $\int_0^{\sqrt{N/k}} \sqrt{N - kt^2}\,dt = \frac{\pi N}{4\sqrt{k}}$, giving $\Theta(N/\sqrt{k})$. $\square$

## Algorithm

```
function count_four_representations(N):
    // Segmented sieve to manage memory
    B = 10^7    // block size
    count = 0

    for block_start = 0 to N step B:
        block_end = min(block_start + B - 1, N)

        for k in {1, 7, 11, 13}:
            bit_array_k = new bit array of size B, all zeros
            for b = 0 to floor(sqrt(block_end / k)):
                lo = max(0, block_start - k*b^2)
                a_lo = ceil(sqrt(lo)) if lo > 0 else 0
                a_hi = floor(sqrt(block_end - k*b^2))
                for a = a_lo to a_hi:
                    n = a^2 + k*b^2
                    if block_start <= n <= block_end:
                        bit_array_k[n - block_start] = 1

        result = bit_array_1 AND bit_array_7 AND bit_array_11 AND bit_array_13
        count += popcount(result)

    return count
```

## Complexity Analysis

- **Time:** $O(N / \sqrt{k})$ per form, summed over $k \in \{1, 7, 11, 13\}$, giving $O(N)$. The segmented approach does not change the asymptotic cost.
- **Space:** $O(B / 8)$ per bit array, with $B = 10^7$ giving $\approx 1.25$ MB per array, $\approx 5$ MB total.

## Answer

$$\boxed{11325263}$$
