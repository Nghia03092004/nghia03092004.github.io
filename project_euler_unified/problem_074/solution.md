# Problem 74: Digit Factorial Chains

## Problem Statement

Define the digit factorial function by replacing a number with the sum of the factorials of its digits. Starting from any positive integer, repeated application forms a chain that eventually cycles. How many chains, with starting number below $10^6$, contain exactly 60 non-repeating terms?

## Mathematical Analysis

### Definition 1 (Digit Factorial Function)

The **digit factorial function** $f: \mathbb{N} \to \mathbb{N}$ is defined by:

$$f(n) = \sum_{i=0}^{k} d_i!$$

where $d_0 d_1 \cdots d_k$ is the decimal representation of $n$.

### Definition 2 (Chain Length)

The **chain length** $L(n)$ is the number of distinct elements in the sequence $n, f(n), f^2(n), \ldots$ before the first repetition.

### Theorem 1 (Eventual Periodicity)

*For all $n \geq 1$, the orbit $(n, f(n), f^2(n), \ldots)$ eventually enters a cycle. Moreover, for any $n < 10^7$, we have $f(n) \leq 2{,}540{,}160$.*

**Proof.** A $k$-digit number $n$ satisfies $n \geq 10^{k-1}$ (for $k \geq 2$) and $f(n) \leq k \cdot 9! = 362880k$. For $k \geq 8$:

$$10^{k-1} > 362880k \quad \text{(verified: } 10^7 = 10{,}000{,}000 > 362880 \times 8 = 2{,}903{,}040\text{)}.$$

Thus $f(n) < n$ for all $n \geq 10^8$, and more generally, $f$ maps any $n$ into $[1, 7 \times 362880] = [1, 2{,}540{,}160]$ within finitely many iterations. Since this is a finite set, the orbit must revisit a value, creating a cycle.

For $n < 10^7$ (at most 7 digits), $f(n) \leq 7 \times 362880 = 2{,}540{,}160$ directly. $\square$

### Theorem 2 (Complete Classification of Cycles)

*The digit factorial function $f$ has exactly the following cycles:*
- *Fixed points (1-cycles): $\{1\}$, $\{2\}$, $\{145\}$, $\{40585\}$.*
- *2-cycle: $\{871, 45361\}$.*
- *3-cycle: $\{169, 363601, 1454\}$.*

*There are no other cycles.*

**Proof.** Every cycle element $c$ must satisfy $c \leq 2{,}540{,}160$ (since $f$ maps any value in this range back into it, and cycle elements are closed under $f$). The claim is verified by exhaustive computation: iterate $f$ from every starting point in $[1, 2{,}540{,}160]$ and record all cycles encountered.

The fixed points satisfy $f(n) = n$. One verifies:
- $f(1) = 1! = 1$.
- $f(2) = 2! = 2$.
- $f(145) = 1! + 4! + 5! = 1 + 24 + 120 = 145$.
- $f(40585) = 4! + 0! + 5! + 8! + 5! = 24 + 1 + 120 + 40320 + 120 = 40585$.

The 2-cycle: $f(871) = 8! + 7! + 1! = 40320 + 5040 + 1 = 45361$; $f(45361) = 24 + 120 + 6 + 720 + 1 = 871$.

The 3-cycle: $f(169) = 1 + 720 + 362880 = 363601$; $f(363601) = 720 + 720 + 6 + 720 + 1 + 1 = ... $ Tracing carefully: $f(363601) = 6 + 720 + 6 + 720 + 1 + 1 = 1454$; $f(1454) = 1 + 24 + 120 + 24 = 169$. $\square$

### Lemma (Chain Length Recursion)

*If $m = f(n)$ and $n$ is not part of a cycle, then $L(n) = 1 + L(m)$.*

**Proof.** The chain starting at $n$ is $(n, m, f(m), \ldots)$. Since $n$ is not in a cycle, $n \neq f^j(m)$ for all $j \geq 0$, so the number of distinct terms is $1 + L(m)$. $\square$

### Corollary (Memoization)

*By the Lemma, once $L(m)$ is known for some $m$, the chain length of any predecessor $n$ (with $f(n) = m$ and $n$ not in a cycle) is immediately determined. This enables dynamic programming over the finite state space $[1, 2{,}540{,}160]$.*

## Algorithm

```
Precompute fact[0..9] = {1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880}
Initialize cache: dict

function chain_length(n):
    if n in cache: return cache[n]
    chain = [], seen = {}
    cur = n
    while cur not in seen and cur not in cache:
        seen[cur] = len(chain)
        chain.append(cur)
        cur = f(cur)
    if cur in cache:
        base = cache[cur]
        for i, val in enumerate(chain):
            cache[val] = len(chain) - i + base
    else:
        # Found a cycle: cur is already in chain
        cycle_start = seen[cur]
        cycle_len = len(chain) - cycle_start
        for i in range(cycle_start, len(chain)):
            cache[chain[i]] = cycle_len
        for i in range(cycle_start - 1, -1, -1):
            cache[chain[i]] = len(chain) - i
    return cache[n]

count = sum(1 for n in range(1, 10^6) if chain_length(n) == 60)
```

## Complexity

**Time:** Each value in $[1, M]$ (where $M = 2{,}540{,}160$) is processed at most once due to memoization. Computing $f$ costs $O(\log n)$ per evaluation. The main loop iterates $N = 10^6$ times. Total:

$$O(N \cdot D + M \cdot D) = O((N + M) \log N)$$

where $D = O(\log N)$ is the cost per digit-factorial computation. Since $M = O(N)$, this simplifies to $O(N \log N)$.

- **Time:** $O(N \log N)$.
- **Space:** $O(N + M) = O(N)$ for the memoization cache.

## Answer

$$\boxed{402}$$
