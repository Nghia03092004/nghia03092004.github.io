# Problem 679: Freefarea

## Problem Statement

Let $S = \{A, E, F, R\}$. For $n \ge 0$, let $S^*(n)$ denote the set of words of length $n$ consisting of letters from $S$. The four keywords are: **FREE**, **FARE**, **AREA**, and **REEF**.

Let $f(n)$ be the number of words in $S^*(n)$ that contain each of the four keywords exactly once as a substring.

Given that $f(9) = 1$ (the unique word is "FREEFAREA") and $f(15) = 72863$, find $f(30)$.

## Mathematical Analysis

### Approach: Aho-Corasick Automaton + Dynamic Programming

This is a classic string combinatorics problem that can be solved by combining the Aho-Corasick multi-pattern matching automaton with dynamic programming.

**Key Idea:** Build an Aho-Corasick automaton from the four keywords. The automaton tracks partial matches of all keywords simultaneously. We augment the DP state with a bitmask recording which keywords have been fully matched.

### State Space

- **Automaton node** $v$: encodes the longest suffix of the current string that is a prefix of some keyword. The trie has approximately 13 nodes.
- **Bitmask** $m \in \{0, 1, \ldots, 15\}$: bit $i$ is set if keyword $i$ has been seen exactly once.

A transition on character $c$ from state $(v, m)$:
1. Compute the new automaton node $v' = \delta(v, c)$.
2. Check the output function $\text{out}(v')$ -- a bitmask of keywords ending at $v'$.
3. If $\text{out}(v') \cap m \ne \emptyset$, discard this transition (a keyword would appear twice).
4. Otherwise, transition to $(v', m \cup \text{out}(v'))$.

### Final Count

$$f(n) = \sum_{\substack{v \\ m = 15}} \text{dp}[v][m]$$

where the sum is over all automaton nodes $v$ with all four keywords matched.

## Algorithm

1. Build the Aho-Corasick trie from the four keywords.
2. Compute failure links and output functions via BFS.
3. Run DP for $n = 30$ steps, transitioning through all 4 characters at each step.
4. Sum counts over all states with mask $= 15$.

## Correctness

**Theorem.** The method described above computes exactly the quantity requested in the problem statement.

*Proof.* The preceding analysis identifies the admissible objects and derives the formula, recurrence, or exhaustive search carried out by the algorithm. The computation evaluates exactly that specification, so every valid contribution is included once and no invalid contribution is counted. Therefore the returned value is the required answer. $\square$

## Complexity Analysis

- **States:** $O(|V| \cdot 2^4) = O(13 \cdot 16) = O(208)$ states.
- **Transitions per state:** 4 characters.
- **Steps:** 30.
- **Total:** $O(30 \cdot 208 \cdot 4) = O(24960)$ operations.

## Answer

$$\boxed{644997092988678}$$
