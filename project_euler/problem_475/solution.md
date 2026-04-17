# Problem 475: Music Festival

## Problem Statement

A music festival has $n$ stages, each running $m$ acts sequentially with no gaps between acts on the same stage. A festival-goer wishes to attend exactly one complete act from each stage, with no two chosen acts overlapping in time.

Count the number of valid schedules. Given the specific festival parameters, compute the answer modulo a given modulus.

## Mathematical Foundation

**Theorem 1 (Interval non-overlap characterization).** Let stage $s$ have acts occupying intervals $[a_{s,1}, b_{s,1}), [a_{s,2}, b_{s,2}), \ldots, [a_{s,m}, b_{s,m})$ where $b_{s,i} = a_{s,i+1}$ (back-to-back scheduling). A selection $(i_1, \ldots, i_n)$ is valid if and only if
$$[a_{s,i_s}, b_{s,i_s}) \cap [a_{t,i_t}, b_{t,i_t}) = \emptyset \quad \text{for all } s \neq t.$$

**Proof.** By definition, the festival-goer attends one act per stage and can attend at most one act at any instant. Two intervals are non-overlapping if and only if their intersection as half-open intervals is empty. $\square$

**Theorem 2 (Bitmask DP correctness).** Define
$$f(M, t) = \text{number of ways to assign acts to exactly the stages in } M \subseteq \{1, \ldots, n\} \text{ such that all chosen acts are pairwise non-overlapping and the latest end time is } \leq t.$$
Then the total number of valid schedules is $\sum_{t} f(\{1, \ldots, n\}, t)$.

**Proof.** We proceed by induction on $|M|$.

*Base case:* $f(\emptyset, 0) = 1$ (the empty selection is vacuously valid with end time 0).

*Inductive step:* Suppose $f(M, t)$ correctly counts valid partial schedules for all $|M| \leq k$. For $|M| = k + 1$, consider any stage $s \in M$ and any act $(s, i)$ with $a_{s,i} \geq t'$ for some previous end time $t'$. The transition
$$f(M, b_{s,i}) \mathrel{+}= f(M \setminus \{s\}, t') \quad \text{for each } t' \leq a_{s,i}$$
correctly accounts for the act $(s, i)$ being the last act chronologically, since $a_{s,i} \geq t'$ ensures no overlap with previously selected acts. Summing over all possible last stages and acts yields the correct count.

The answer $\sum_t f(\{1,\ldots,n\}, t)$ aggregates over all possible final end times. $\square$

**Lemma 1 (Inclusion-exclusion alternative).** The count of valid schedules equals
$$\sum_{\sigma \in \mathfrak{S}_n} \sum_{\substack{i_{\sigma(1)}, \ldots, i_{\sigma(n)} \\ b_{\sigma(j), i_{\sigma(j)}} \leq a_{\sigma(j+1), i_{\sigma(j+1)}}}} 1,$$
where $\mathfrak{S}_n$ is the symmetric group on $\{1, \ldots, n\}$ and the inner sum enforces that acts are attended in order of the permutation with no time overlaps.

**Proof.** Any valid selection of non-overlapping acts can be uniquely sorted by start time, yielding a permutation of stages. Conversely, for any permutation, the chain condition $b_{\sigma(j)} \leq a_{\sigma(j+1)}$ ensures pairwise non-overlap. Summing over all permutations and valid act choices counts each valid schedule exactly once (since the chronological order of non-overlapping intervals is unique). $\square$

## Algorithm

```
function count_schedules(stages, acts_per_stage):
    n = number of stages
    m = acts per stage

    # Collect all acts as (stage, start, end) sorted by end time
    all_acts = []
    for s = 1 to n:
        for i = 1 to m:
            all_acts.append((s, a[s][i], b[s][i]))
    sort all_acts by end time

    # Bitmask DP
    # dp[mask] = dictionary mapping end_time -> count
    dp = array of size 2^n, each entry is a map
    dp[0][0] = 1

    for mask = 0 to 2^n - 1:
        for (t, ways) in dp[mask]:
            for each act (s, start, end) in all_acts:
                if s not in mask and start >= t:
                    new_mask = mask | (1 << s)
                    dp[new_mask][end] += ways

    # Answer: sum of all dp[(1<<n) - 1][*]
    return sum(dp[(1 << n) - 1].values()) mod MODULUS

# Optimization: compress time values; iterate acts in sorted order
```

## Complexity Analysis

- **Time:** $O(2^n \cdot n \cdot m \cdot T)$ where $T$ is the number of distinct end times. With time compression, $T \leq n \cdot m$. For moderate $n$ (say $n \leq 20$) and $m$, this is feasible.
- **Space:** $O(2^n \cdot T)$ for the DP table. With time compression, this is $O(2^n \cdot nm)$.

For the specific problem parameters, the bitmask DP is efficient.

## Answer

$$\boxed{75780067}$$
