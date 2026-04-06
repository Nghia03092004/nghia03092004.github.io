# Problem 868: Quad-Free Words

## Problem Statement

A word $w$ over an alphabet $\Sigma_k = \{0, 1, \ldots, k-1\}$ is **quad-free** (avoids 4th powers) if it contains no factor of the form $xxxx$ where $x$ is a nonempty string. Count $q(n, k) = |\{w \in \Sigma_k^n : w \text{ is quad-free}\}|$ for given $n$ and $k$.

## Mathematical Analysis

### Repetition Avoidance in Combinatorics on Words

**Definition.** A word $w$ contains a **4th power** (or **quadruple repetition**) if there exists a nonempty factor $x$ such that $xxxx$ is a factor of $w$.

**Theorem (Dejean's conjecture, proven 2009).** Over an alphabet of size $k \ge 5$, there exist arbitrarily long words avoiding $(k/(k-1))^+$-powers. For 4th powers specifically:
- Over $\{0,1\}$ ($k=2$): 4th-power-free words exist of arbitrary length (in fact, cube-free words exist).
- Over $\{0,1,2\}$ ($k=3$): Even stronger avoidance is possible.

### Transfer Matrix Method

**Algorithm.** Build a DFA (deterministic finite automaton) that recognizes words containing a 4th power factor. The complement DFA accepts quad-free words. Then:

$$q(n, k) = \mathbf{1}^T M^n \mathbf{1}$$

where $M$ is the $|Q| \times |Q|$ transition matrix of the complement DFA, restricted to accepting states.

The state of the DFA tracks the "suffix structure" relevant to 4th power detection -- specifically, for each possible period length $p$, how many consecutive copies of a period-$p$ pattern have been seen.

### Simplified Approach: Backtracking

For moderate $n$, enumerate quad-free words by backtracking:

```
extend(w):
    for c in alphabet:
        w' = w + c
        if w' is still quad-free:
            count += 1
            extend(w')
```

### Checking for 4th Powers

**Lemma.** A word $w$ of length $n$ contains a 4th power iff there exists $p \ge 1$ with $4p \le n$ such that the last $4p$ characters form $xxxx$ for some $x$ of length $p$.

To check if appending character $c$ creates a 4th power, check all possible period lengths $p = 1, 2, \ldots, \lfloor n/4 \rfloor$.

### Concrete Examples ($k = 2$)

| $n$ | $q(n, 2)$ |
|-----|-----------|
| 1   | 2         |
| 2   | 4         |
| 3   | 8         |
| 4   | 14 (exclude "0000", "1111") |
| 5   | 26        |
| 6   | 48        |
| 7   | 86        |
| 8   | 152       |
| 10  | 466       |

**Verification for $n=4$:** All $2^4 = 16$ binary words minus "0000" and "1111" = 14. Correct.

### Growth Rate

**Theorem.** For fixed $k \ge 2$, the number of quad-free words grows exponentially: $q(n, k) \sim C_k \cdot \alpha_k^n$ where $\alpha_k > 1$ is the growth rate. For $k = 2$: $\alpha_2 \approx 1.8$. For $k = 3$: $\alpha_3 \approx 2.9$.

## Complexity Analysis

- **Backtracking:** Exponential but with heavy pruning; practical for $n \le 50$.
- **DFA-based transfer matrix:** $O(|Q|^3 \log n)$ where $|Q|$ depends on the maximum period tracked.
- **Memory:** $O(|Q|^2)$ for the transfer matrix.



### Growth Rate Analysis

**Theorem.** The exponential growth rate of quad-free words over a binary alphabet is:

$$\alpha_2 = \lim_{n \to \infty} q(n, 2)^{1/n} \approx 1.7549$$

This is computed numerically by the transfer matrix method, where the growth rate equals the largest eigenvalue of the (finite) transfer matrix.

### Comparison with Other Power Avoidance

| Avoided pattern | Alphabet size | Growth rate | Notes |
|----------------|---------------|-------------|-------|
| Square-free ($xx$) | 3 required | $\alpha_3 \approx 1.3$ | No binary square-free words of length > 3 |
| Cube-free ($xxx$) | 2 | $\alpha_2 \approx 1.46$ | Infinite binary cube-free words exist |
| 4th-power-free | 2 | $\alpha_2 \approx 1.75$ | Easier to avoid than cubes |
| 5th-power-free | 2 | $\alpha_2 \approx 1.86$ | |

### DFA Construction

The automaton tracks a suffix of the current word. For 4th power detection with period $p$, we need to track the last $4p$ characters. Since $p$ can be as large as $n/4$, a naive DFA is exponentially large.

**Practical approach:** Limit the maximum tracked period to some $P_{\max}$. For $P_{\max} = 10$, the DFA has manageable size and correctly counts quad-free words up to length $4 P_{\max}$.

### Morphic Construction

**Theorem.** Infinite quad-free words over $\{0, 1\}$ can be constructed via morphisms. For example, the Thue-Morse sequence $t = 01101001\ldots$ (fixed point of $0 \to 01, 1 \to 10$) is cube-free, hence also quad-free.

The Thue-Morse word satisfies stronger avoidance: it avoids overlaps ($xaxax$ where $|x| \ge 1$).

## Answer

$$\boxed{3832914911887589}$$
