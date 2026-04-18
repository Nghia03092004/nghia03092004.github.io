# Problem 551: Sum of Digit Sequence

## Problem Statement

Let $a_0, a_1, a_2, \ldots$ be an integer sequence defined by:
- $a_0 = 1$.
- $a_n = a_{n-1} + s(a_{n-1})$ for $n \geq 1$, where $s(m)$ denotes the sum of the base-10 digits of $m$.

The first terms are $1, 2, 4, 8, 16, 23, 28, 38, 49, 62, \ldots$

Given that $a_{10^6} = 31054319$, find $a_{10^{15}}$.

## Mathematical Foundation

**Definition 1.** For a non-negative integer $m$ with base-10 representation $m = \sum_{j=0}^{d-1} m_j \cdot 10^j$ where $0 \leq m_j \leq 9$, the *digit sum* is $s(m) = \sum_{j=0}^{d-1} m_j$.

**Theorem 1 (Digit-Sum Growth Bound).** For all $n \geq 0$, if $a_n$ has $d$ digits, then $1 \leq s(a_n) \leq 9d$. Consequently, $a_n = O(n \log n)$.

*Proof.* Since $a_n \geq 1$ for all $n \geq 0$ (by induction, as $a_0 = 1$ and each step adds a positive digit sum), every digit representation is non-trivial and $s(a_n) \geq 1$. Each digit is at most 9, so $s(a_n) \leq 9d$ where $d = \lfloor \log_{10}(a_n) \rfloor + 1$. The recurrence gives $a_n = a_0 + \sum_{k=0}^{n-1} s(a_k)$, and since $s(a_k) \leq 9(\lfloor \log_{10}(a_k) \rfloor + 1) = O(\log a_k)$, standard analysis yields $a_n = O(n \log n)$. In particular, $a_{10^{15}}$ has at most 17 digits. $\square$

**Lemma 1 (Block Invariance).** Write $a_n$ in positional blocks of width $B$: decompose $a_n = c_0 + c_1 \cdot 10^B + c_2 \cdot 10^{2B} + \cdots$ where $0 \leq c_j < 10^B$. Define $s_{\mathrm{upper}} = \sum_{j \geq 1} s(c_j)$, the digit sum contributed by all blocks above the lowest. Then:

(i) The full digit sum satisfies $s(a_n) = s(c_0) + s_{\mathrm{upper}}$.

(ii) If $c_0 + s(c_0) + s_{\mathrm{upper}} < 10^B$ (no carry out of the lowest block), then the transition $a_n \mapsto a_{n+1}$ leaves all upper blocks unchanged.

(iii) As a consequence, $s_{\mathrm{upper}}$ remains constant across all steps in which no carry propagates out of the lowest block.

*Proof.* Part (i) follows from the additivity of digit sums across independent blocks. For (ii), adding $s(a_n) = s(c_0) + s_{\mathrm{upper}}$ to $a_n$ modifies only the lowest block: $c_0 \mapsto c_0 + s(c_0) + s_{\mathrm{upper}}$, and the no-carry hypothesis ensures this remains below $10^B$, leaving $c_1, c_2, \ldots$ intact. Part (iii) is immediate from (ii). $\square$

**Theorem 2 (Block Transition Function).** For block size $B$, define the transition
$$T(V, U) = (\Delta, V')$$
where $V \in \{0, \ldots, 10^B - 1\}$ is the current lowest-block value, $U \geq 0$ is the upper digit sum, $\Delta$ is the number of sequence steps until the first carry propagates out of the block, and $V'$ is the new lowest-block value after those $\Delta$ steps. Then:

(i) $T$ is well-defined: starting from any $(V, U)$ with $V + U \geq 1$, a carry must eventually occur.

(ii) $\Delta \leq 10^B$, and in practice $\Delta = O(10^B / (U + 1))$.

(iii) $V' = (V_{\mathrm{final}} + s(V_{\mathrm{final}}) + U) - 10^B$, where $V_{\mathrm{final}}$ is the block value at the last step before carry.

*Proof.* For (i), by Lemma 1(iii), $U$ is constant until carry. Each step increments $V$ by $s(V) + U \geq 1$ (since $a_n \geq 1$ ensures $s(V) + U \geq 1$). Hence $V$ strictly increases and must eventually reach or exceed $10^B$. For (ii), the minimum per-step increment is 1, giving $\Delta \leq 10^B$; when $U$ is large the average increment is roughly $U + O(\log V)$, so $\Delta \approx 10^B / U$. Part (iii) follows directly from the definition. $\square$

**Corollary 1 (Memoizability).** The function $T(V, U)$ depends only on $(V, U)$. With $B = 4$, the domain is $\{0, \ldots, 9999\} \times \{0, \ldots, 9 \cdot 16\}$, giving a table of $10^4 \times 145 \approx 1.45 \times 10^6$ entries that can be lazily computed and cached.

## Editorial
The algorithm maintains $a_n$ as an array of 20 base-10 digits and uses block-based acceleration with memoization. We use dynamic programming over the state space implied by the derivation, apply each admissible transition, and read the answer from the final table entry.

## Pseudocode

```text
repeat
else
```

## Complexity Analysis

- **Table construction:** At most $10^4 \times 145 \approx 1.45 \times 10^6$ entries, each requiring $O(10^4)$ steps in the worst case but $O(10^4 / (U+1))$ on average.
- **Main loop:** Each block jump advances the sequence by $\Theta(10^4 / U)$ steps on average, so the number of outer iterations is $O(N \cdot U_{\mathrm{avg}} / 10^4)$. With $N = 10^{15}$ and typical average $U \approx 70$, the outer loop executes on the order of $10^{13}$ iterations in the worst case but much fewer due to caching and carry propagation patterns. Total runtime is several minutes.
- **Space:** $O(10^4 \times U_{\max}) \approx O(1.5 \times 10^6)$ for the memo table.

## Answer

$$\boxed{73597483551591773}$$
