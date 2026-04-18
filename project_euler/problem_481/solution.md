# Problem 481: Chef Showdown

## Problem Statement

A group of $n$ chefs participate in a turn-based strategic cooking competition. On each chef's turn, he/she cooks a dish. Chef $k$ has skill level $S(k) = F_k / F_{n+1}$, where $F_k$ is the $k$-th Fibonacci number ($F_1 = F_2 = 1$). If the dish receives a favorable rating (probability $S(k)$), the chef eliminates one other chef of their choice. Turns iterate sequentially among remaining chefs. The last chef standing wins.

All chefs play optimally to maximize their own winning probability. Ties in elimination choice are broken by choosing the chef whose turn comes next.

Let $E(n)$ be the expected number of dishes cooked in a competition with $n$ chefs.

Given: $E(7) = 42.28176050$.

Find $E(14)$ rounded to 8 decimal places.

## Mathematical Foundation

**Definition 1 (Game state).** A *state* is a pair $(\mathcal{S}, \tau)$ where $\mathcal{S} \subseteq \{1, \ldots, n\}$ is the set of remaining chefs with $|\mathcal{S}| = m \geq 1$, and $\tau$ is the index within the ordered set $\mathcal{S}$ indicating whose turn begins the next round.

**Theorem 1 (Geometric round structure).** Let the turn order within state $(\mathcal{S}, \tau)$ be $(c_1, c_2, \ldots, c_m)$, and define $q_i = 1 - S(c_i)$ for each $i$. A *round* consists of all $m$ chefs attempting a dish in sequence. Then:

(i) The probability that no chef succeeds in a full round is
$$Q(\mathcal{S}) = \prod_{i=1}^{m} q_i.$$

(ii) The probability that chef $c_j$ is the first to succeed in a given round is
$$P_j(\mathcal{S}) = S(c_j) \prod_{i=1}^{j-1} q_i.$$

(iii) These probabilities satisfy $\sum_{j=1}^{m} P_j(\mathcal{S}) = 1 - Q(\mathcal{S})$.

(iv) The number of completely failed rounds before the first success is geometrically distributed with parameter $1 - Q(\mathcal{S})$.

*Proof.* Each chef's success is an independent Bernoulli trial with parameter $S(c_i)$.

For (i): a complete failure occurs if and only if every chef fails, which has probability $\prod_{i=1}^{m} q_i$ by independence.

For (ii): chef $c_j$ is the first to succeed in a round if and only if $c_1, \ldots, c_{j-1}$ all fail and $c_j$ succeeds. By independence, this has probability $S(c_j) \prod_{i=1}^{j-1} q_i$.

For (iii): we verify
$$\sum_{j=1}^{m} S(c_j) \prod_{i=1}^{j-1} q_i = 1 - \prod_{i=1}^{m} q_i.$$
This is proved by induction on $m$. For $m = 1$: $S(c_1) = 1 - q_1$. Assuming it holds for $m - 1$ chefs, adding the $m$-th chef contributes $S(c_m) \prod_{i=1}^{m-1} q_i$, and the sum becomes $(1 - \prod_{i=1}^{m-1} q_i) + (1 - q_m) \prod_{i=1}^{m-1} q_i = 1 - \prod_{i=1}^{m} q_i$.

For (iv): rounds are independent (each round's outcome depends only on fresh Bernoulli trials), so the count of completely failed rounds before the first success follows $\text{Geom}(1 - Q(\mathcal{S}))$. $\square$

**Corollary 1.** Conditioned on the first success occurring in *some* round, the probability that chef $c_j$ is the one to succeed is
$$\frac{P_j(\mathcal{S})}{1 - Q(\mathcal{S})}.$$

*Proof.* Immediate from the law of total probability and the memoryless property of the geometric distribution. $\square$

**Theorem 2 (Optimal elimination strategy).** For each state $(\mathcal{S}, \tau)$ and each chef $c_j \in \mathcal{S}$, define $W(c_j, \mathcal{S}, \tau)$ as the probability that $c_j$ ultimately wins. When $c_j$ succeeds and must choose whom to eliminate, the optimal target is
$$t^* = \arg\max_{t \in \mathcal{S} \setminus \{c_j\}} W\bigl(c_j,\, \mathcal{S} \setminus \{t\},\, \tau'(t)\bigr),$$
with ties broken by choosing the chef whose turn comes soonest after $c_j$ in the cyclic order, where $\tau'(t)$ denotes the appropriate starting index in $\mathcal{S} \setminus \{t\}$.

*Proof.* By backward induction on $|\mathcal{S}|$.

*Base case:* $|\mathcal{S}| = 1$. The remaining chef wins with probability 1; no elimination decision is required.

*Inductive step:* Suppose the winning probabilities $W(\cdot, \mathcal{S}', \cdot)$ are uniquely determined for all states $\mathcal{S}'$ with $|\mathcal{S}'| < |\mathcal{S}|$. When chef $c_j$ succeeds in state $(\mathcal{S}, \tau)$, eliminating target $t$ transitions the game to state $(\mathcal{S} \setminus \{t\}, \tau'(t))$, where $\tau'(t)$ is determined by the cyclic turn order (the next chef after $c_j$ who is not $t$). A rational agent maximizes $W(c_j, \mathcal{S} \setminus \{t\}, \tau'(t))$ over all valid targets $t$. This is well-defined by the inductive hypothesis. The tie-breaking rule is deterministic and consistent. $\square$

**Theorem 3 (Expected dish count).** The expected number of dishes cooked from state $(\mathcal{S}, \tau)$ satisfies the recurrence
$$E(\mathcal{S}, \tau) = \frac{E_{\text{partial}}(\mathcal{S}, \tau)}{1 - Q(\mathcal{S})} + \frac{m \cdot Q(\mathcal{S})}{1 - Q(\mathcal{S})} + \sum_{j=1}^{m} \frac{P_j(\mathcal{S})}{1 - Q(\mathcal{S})} \cdot E(\mathcal{S} \setminus \{t_j^*\},\, \tau_j'),$$
where $E_{\text{partial}}(\mathcal{S}, \tau) = \sum_{j=1}^{m} j \cdot P_j(\mathcal{S})$ is the expected number of dishes cooked within the successful round, $t_j^*$ is the optimal elimination target for chef $c_j$, and $\tau_j'$ is the resulting starting index.

Equivalently, combining the first two terms:
$$E(\mathcal{S}, \tau) = \frac{m \cdot Q(\mathcal{S}) + E_{\text{partial}}(\mathcal{S}, \tau)}{1 - Q(\mathcal{S})} + \sum_{j=1}^{m} \frac{P_j(\mathcal{S})}{1 - Q(\mathcal{S})} \cdot E(\mathcal{S} \setminus \{t_j^*\},\, \tau_j'),$$
with the base case $E(\{c\}, \cdot) = 0$ for any single chef $c$.

*Proof.* Condition on the geometric number of failed rounds $R \sim \text{Geom}(1 - Q(\mathcal{S}))$. In each of the $R$ failed rounds (expected count $Q/(1-Q)$), exactly $m$ dishes are cooked, contributing $m \cdot Q/(1-Q)$ in expectation. In the successful round, chef $c_j$ succeeds at position $j$ (i.e., after $j$ dishes in that round), with conditional probability $P_j/(1-Q)$. This gives the partial-round expected dish count $E_{\text{partial}}/(1-Q)$.

After the elimination, the game transitions to state $(\mathcal{S} \setminus \{t_j^*\}, \tau_j')$ and the expected remaining dishes are $E(\mathcal{S} \setminus \{t_j^*\}, \tau_j')$, weighted by the conditional probability $P_j/(1-Q)$.

The base case $E(\{c\}, \cdot) = 0$ holds because no more dishes are cooked once a single chef remains. $\square$

## Editorial
Chef k has skill S(k) = F_k / F_{n+1} (Fibonacci ratio). Chefs eliminate each other optimally. Compute E(14), the expected number of dishes cooked. Method: bitmask DP over game states (remaining chefs, start index). For each state, compute winning probabilities via Theorem 2 (backward induction on optimal elimination) and expected dishes via Theorem 3 (geometric round structure + recursive decomposition).

## Pseudocode

```text
    Compute Fibonacci numbers F[1..n+1]
    S[k] = F[k] / F[n+1] for k = 1..n

    States are (bitmask of remaining chefs, start index within the ordered set)

    For each each state (mask, start_idx) in order of increasing popcount(mask):
        If popcount(mask) == 1 then
            W[mask][start_idx][lone_chef] = 1.0
            E[mask][start_idx] = 0.0
            continue

        chefs = sorted list of set bits in mask
        m = len(chefs)
        Reorder chefs cyclically starting at start_idx
        Compute Q, P[j], and reach probabilities p_reach[j]

        For each chef c_j, find optimal target t_j* via argmax of W
        (with tie-breaking by cyclic distance)

        Compute W[mask][start_idx][k] for all k via weighted sum
        Compute E[mask][start_idx] via Theorem 3

    Return E[full_mask][0]
```

## Complexity Analysis

- **Time:** $O(n^2 \cdot 2^n)$. There are $O(n \cdot 2^n)$ states (mask, start); for each state with $m$ chefs, evaluating all elimination choices costs $O(m)$ per chef, yielding $O(m^2)$ per state. Summing over all masks: $\sum_{\text{masks}} m^2 = O(n^2 \cdot 2^n)$. For $n = 14$, this is approximately $14^2 \times 16384 \approx 3.2 \times 10^6$ operations.
- **Space:** $O(n \cdot 2^n)$ for storing winning probabilities $W[\text{mask}][\text{start}][\text{chef}]$ and expected dishes $E[\text{mask}][\text{start}]$.

## Answer

$$\boxed{729.12106947}$$
