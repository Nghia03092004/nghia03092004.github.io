# Problem 298: Selective Amnesia

## Problem Statement

Larry and Robin play a memory game. In each of 50 turns, a number is drawn uniformly at random from $\{1, 2, \ldots, 10\}$. Each player maintains a memory of at most 5 numbers. If the called number is in memory, the player scores a point. If not, the number is added to memory, evicting one number if memory is full.

- **Larry (LRU):** On eviction, removes the number not called in the longest time. On a hit, the item moves to the "most recently used" position.
- **Robin (FIFO):** On eviction, removes the number that has been in memory the longest. On a hit, the item does not move.

Find $E[|L - R|]$ after 50 turns, to 8 decimal places.

## Mathematical Foundation

**Theorem 1 (Markov Property).** *The joint state $(S_L, S_R)$ -- where $S_L$ is Larry's LRU cache state and $S_R$ is Robin's FIFO queue state -- evolves as a Markov chain under the uniform i.i.d. draws.*

**Proof.** Given the current state $(S_L, S_R)$ and the drawn number $x$, the next state $(S_L', S_R')$ is determined entirely by the transition rules of LRU and FIFO. The draw $x$ is independent of past draws. Therefore the process is Markovian. $\square$

**Theorem 2 (Symmetry Reduction).** *By the symmetry of the uniform distribution over $\{1, \ldots, 10\}$ and the label-invariance of both LRU and FIFO policies, the state space can be compressed using equivalence classes under the action of the symmetric group $\mathfrak{S}_{10}$ permuting labels.*

**Proof.** Let $\sigma \in \mathfrak{S}_{10}$ be any permutation of $\{1, \ldots, 10\}$. If $(S_L, S_R)$ transitions to $(S_L', S_R')$ upon drawing $x$, then $(\sigma(S_L), \sigma(S_R))$ transitions to $(\sigma(S_L'), \sigma(S_R'))$ upon drawing $\sigma(x)$. Since $x$ and $\sigma(x)$ are equidistributed under the uniform distribution, the transition probabilities depend only on the equivalence class of $(S_L, S_R)$ under $\mathfrak{S}_{10}$. $\square$

**Lemma 1 (Compressed State Description).** *The compressed state records the relative structure between Larry's and Robin's memories: specifically, the partition of $\{1, \ldots, 10\}$ into categories based on membership/position in each memory, along with the orderings within each memory. The number of equivalence classes is finite and manageable for exact computation.*

**Proof.** Each card falls into one of four categories: (i) in both memories, (ii) in Larry's only, (iii) in Robin's only, (iv) in neither. For cards in the same category, their relative orderings in each relevant memory matter, but the specific label does not (by symmetry). The number of such structural configurations is bounded by $\binom{10}{5}^2 \times 5!^2 / |\text{symmetry}|$, which after proper accounting yields a manageable number of equivalence classes. $\square$

**Theorem 3 (Score Difference Tracking).** *We track the probability distribution over (compressed state, score difference $L - R$) at each turn. Since $|L - R| \leq 50$, the score difference takes values in $\{-50, \ldots, 50\}$, and the final answer is*

$$E[|L - R|] = \sum_{d=-50}^{50} |d| \cdot \Pr[L - R = d \text{ after 50 turns}].$$

**Proof.** This follows from the definition of expected value of the absolute value of a discrete random variable. $\square$

## Algorithm

```
function solve():
    // Step 1: Enumerate compressed states
    // A state tracks: for each of the 4 categories (both, L-only, R-only, neither),
    // the count of items, plus the orderings in L's LRU list and R's FIFO queue.
    states = enumerate_compressed_states()

    // Step 2: Build transition function
    // For each state and each possible draw (10 equally likely):
    //   Determine hit/miss for Larry, hit/miss for Robin
    //   Compute new state, score_diff change in {-1, 0, +1}

    // Step 3: DP over 50 turns
    // dist[state][score_diff] = probability
    dist = initial_distribution()  // empty memories, score_diff = 0

    for turn = 1 to 50:
        new_dist = {}
        for (state, diff) in dist:
            for draw = 1 to 10:  // or by category with multiplicity
                (new_state, delta) = transition(state, draw)
                new_dist[new_state][diff + delta] += dist[state][diff] / 10
        dist = new_dist

    // Step 4: Compute E[|L - R|]
    result = 0
    for (state, diff) in dist:
        result += |diff| * dist[state][diff]
    return result
```

## Complexity Analysis

- **Time:** $O(50 \cdot |S| \cdot D \cdot C)$ where $|S|$ is the number of compressed states, $D = 101$ is the range of score differences, and $C$ is the number of distinct draw categories (at most 4). The dominant factor is $|S|$, which after symmetry reduction is on the order of $10^5$ to $10^6$.
- **Space:** $O(|S| \cdot D)$ for the probability distribution table at each turn.

## Answer

$$\boxed{1.76882294}$$
