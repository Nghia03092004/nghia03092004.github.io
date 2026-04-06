# Problem 442: Eleven-free Integers

## Problem Statement

An integer is called **eleven-free** if its decimal expansion does not contain any substring representing a power of $11$ (excluding $11^0 = 1$). The forbidden substrings are $11, 121, 1331, 14641, 161051, \ldots$

Examples: $E(3) = 3$, $E(200) = 213$, $E(500000) = 531563$.

Find $E(10^{18})$, the $10^{18}$-th positive eleven-free integer.

## Mathematical Foundation

**Theorem 1 (Aho-Corasick Multi-Pattern Matching).** *Given a finite set of patterns $\mathcal{P} = \{P_1, \ldots, P_k\}$ over alphabet $\Sigma$, the Aho-Corasick automaton $\mathcal{A}$ is a deterministic finite automaton with $O(S)$ states (where $S = \sum |P_i|$) such that for any string $w$, the automaton reaches an accepting state if and only if some $P_i$ is a substring of $w$.*

**Proof.** Construct a trie $T$ of the patterns. Define the failure function $\pi(v)$ for each node $v$ as the longest proper suffix of the string represented by $v$ that is also a prefix of some pattern (equivalently, a node in $T$). By BFS from the root, $\pi$ is computed in $O(S \cdot |\Sigma|)$ time. The goto function $\delta(v, c)$ follows the trie if possible, otherwise follows failure links. A state is accepting if it corresponds to a complete pattern or has an accepting state reachable via failure links (dictionary suffix links). The resulting DFA processes each character in $O(1)$ amortized time and detects any pattern occurrence. $\square$

**Theorem 2 (Digit DP Counting).** *Let $\mathcal{A}$ be a DFA with state set $Q$, non-accepting states $Q_{\text{safe}} \subseteq Q$, and transition function $\delta$. The number of integers in $[1, N]$ whose decimal representation avoids all accepting states of $\mathcal{A}$ can be computed in $O(L \cdot |Q| \cdot |\Sigma|)$ time, where $L$ is the number of digits of $N$ and $|\Sigma| = 10$.*

**Proof.** Define $\text{dp}[i][s][t][z]$ where $i$ is the digit position (from most significant), $s \in Q$ is the current automaton state, $t \in \{0,1\}$ indicates whether we are still bounded by the digits of $N$ (tight constraint), and $z \in \{0,1\}$ indicates whether a nonzero digit has been placed. For each state, we iterate over all possible digits $d \in \{0, \ldots, 9\}$ (or $\{0, \ldots, N_i\}$ if tight), advance the automaton state via $\delta(s, d)$, and accumulate counts only if the new state is in $Q_{\text{safe}}$. The base case at $i = L$ counts 1 if $z = 1$ (a valid positive integer). The total number of states is $O(L \cdot |Q| \cdot 2 \cdot 2)$, and each state considers $O(|\Sigma|)$ transitions. $\square$

**Lemma 1 (Monotonicity and Binary Search).** *The counting function $C(N) = |\{k \in [1, N] : k \text{ is eleven-free}\}|$ is non-decreasing. Hence for any target $n$, $E(n) = \min\{N : C(N) \geq n\}$ can be found by binary search on $N$ in $O(\log U)$ evaluations of $C$, where $U$ is an upper bound on the answer.*

**Proof.** $C(N+1) - C(N) \in \{0, 1\}$, so $C$ is non-decreasing. The function $E$ is well-defined since eleven-free integers have positive density (only finitely many forbidden substrings of bounded length). Binary search applies to any non-decreasing function. $\square$

## Algorithm

```
function FindE(target):
    // Step 1: Generate forbidden patterns
    patterns = []
    power = 11
    while number_of_digits(power) <= 19:
        patterns.append(str(power))
        power *= 11

    // Step 2: Build Aho-Corasick automaton from patterns
    automaton = BuildAhoCorasick(patterns)

    // Step 3: Digit DP to count eleven-free integers <= N
    function Count(N):
        digits = decimal_digits(N)
        L = len(digits)
        // dp[position][automaton_state][tight][started] -> count
        memo = {}
        return digitDP(0, root_state, true, false, digits, automaton, memo)

    // Step 4: Binary search
    lo = target
    hi = 2 * target  // upper bound (density is close to 1)
    while lo < hi:
        mid = (lo + hi) / 2
        if Count(mid) >= target:
            hi = mid
        else:
            lo = mid + 1
    return lo
```

## Complexity Analysis

- **Time:** $O(\log U \cdot L \cdot |Q| \cdot 10)$ where $U \leq 2 \times 10^{18}$, $L \leq 19$ digits, $|Q| = O(S)$ with $S = \sum_{k=1}^{18} \lfloor k \log_{10} 11 \rfloor + 1 \approx 170$ total pattern characters. Thus $|Q| \approx 170$, and the binary search uses $O(\log(2 \times 10^{18})) \approx 61$ iterations. Total: $O(61 \times 19 \times 170 \times 10) \approx 2 \times 10^6$ operations.
- **Space:** $O(|Q| \cdot |\Sigma| + L \cdot |Q|)$ for the automaton transition table and DP memoization.

## Answer

$$\boxed{1295552661530920149}$$
