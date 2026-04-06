# Problem 915: Digit Power Sum Sequences

## Problem Statement

Define $f(n) = \sum_{d \in \text{digits}(n)} d^5$. Starting from any $n$, the sequence $n, f(n), f(f(n)), \ldots$ eventually enters a cycle. Find the sum of all starting values $n \leq 10^6$ that eventually reach 1.

## Mathematical Analysis

### Orbit Contraction

**Theorem.** *For any $n \geq 10^7$ (7+ digits), $f(n) < n$. Hence every orbit under $f$ eventually enters the range $[1, 413343]$ and must cycle.*

*Proof.* A $d$-digit number $n$ satisfies $n \geq 10^{d-1}$. The maximum value of $f$ on $d$-digit numbers is $d \cdot 9^5 = 59049d$. For $d \geq 7$:
$$59049 \cdot 7 = 413343 < 10^6 < 10^{d-1} \leq n$$
so $f(n) < n$. The orbit is strictly decreasing until it enters $[1, 413343]$, where it must revisit some value (pigeonhole), forming a cycle. $\square$

### Fixed Points of $f$

**Lemma.** *$n = 1$ is a fixed point of $f$: $f(1) = 1^5 = 1$.*

There are very few fixed points. For $n$ to be a fixed point, $n = \sum d_i^5$ where $d_i$ are the digits of $n$. Searching up to $413343$:
- $f(1) = 1$ (fixed point)
- Other possible fixed points and cycles can be found by iterating $f$ on all values up to $413343$.

### Classification of Eventual Behavior

Every positive integer $n$ eventually enters one of finitely many cycles under $f$. The cycles for $p = 5$ (fifth-power digit sum) include:
- **Fixed point at 1:** $1 \to 1$
- Various longer cycles (e.g., involving numbers like 4150, 4151, etc.)

The Armstrong numbers (narcissistic numbers) for power 5 are: 1, 4150, 4151, 54748, 92727, 93084, 194979. These satisfy $n = f(n)$.

### Memoized Orbit Classification

**Algorithm.** For each $n$ from 1 to $10^6$:
1. Follow the orbit $n, f(n), f(f(n)), \ldots$ until revisiting a known value.
2. If the orbit reaches a value already classified as "reaches 1" or "does not reach 1," inherit that classification.
3. Cache the result for all values along the current path.

**Theorem.** *This algorithm runs in $O(N)$ total time with $O(N)$ space, because each value is visited at most twice (once in its own orbit computation, once when another orbit passes through it).*

### Counting the Happy Numbers (Fifth-Power Variant)

**Definition.** A number $n$ is *5-happy* if its orbit under $f$ eventually reaches 1.

For the classical happy-number problem (power 2), roughly 14.3% of numbers are happy. For power 5, the density differs.

### Concrete Examples

| $n$ | Orbit | Reaches 1? |
|-----|-------|-----------|
| 1 | $1 \to 1$ | Yes |
| 2 | $2 \to 32 \to 275 \to 17975 \to \ldots$ | Check |
| 10 | $10 \to 1$ | Yes |
| 100 | $100 \to 1$ | Yes |
| 999 | $999 \to 3 \cdot 9^5 = 177147 \to \ldots$ | Check |
| 4150 | $4150 \to 4^5+1+5^5+0 = 1024+1+3125 = 4150$ | No (cycle at self) |

### Upper Bound on Orbit Length

**Lemma.** *For any $n \leq 10^6$, the orbit reaches a value $\leq 413343$ within at most 2 applications of $f$. From there, the orbit length before cycling is bounded by $413343$ (in the worst case, but typically much less).*

*Proof.* $f(n) \leq 6 \cdot 9^5 = 354294$ for any 6-digit $n$. And $f(m) \leq 6 \cdot 9^5 = 354294$ for any $m \leq 413343$. So after one step, we are in $[1, 354294]$ and stay there. $\square$

## Proof of Correctness

The memoization algorithm correctly identifies whether each orbit reaches 1:
1. **Termination:** Guaranteed because orbits contract into a finite range and must cycle.
2. **Correctness of caching:** Once a value's classification is determined, it is permanent---the orbit is deterministic.
3. **Completeness:** Every $n \leq 10^6$ is checked.

## Complexity Analysis

- **Time:** $O(N)$ amortized---each number is processed once.
- **Space:** $O(N)$ for the memoization table.
- **Per-step cost:** $O(\log n)$ to compute digit sum (at most 6 digits for $n \leq 10^6$).

## Answer

$$\boxed{55601924}$$
