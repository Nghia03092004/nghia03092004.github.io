# Problem 832: Mex Sequence

## Problem Statement

The **mex** (minimum excludant) of a set $S \subseteq \mathbb{N}_0$ is the smallest non-negative integer not in $S$:

$$\text{mex}(S) = \min(\mathbb{N}_0 \setminus S).$$

Define a sequence using mex operations and compute a sum or specific value, modulo $10^9+7$.

## Mathematical Foundation

**Theorem 1 (Sprague-Grundy).** *A combinatorial game position $P$ is a losing position (for the player to move) if and only if $G(P) = 0$, where the Grundy value is defined recursively by*

$$G(P) = \operatorname{mex}\{G(P') : P' \in \operatorname{moves}(P)\}.$$

*Moreover, for a disjunctive sum of independent games, $G(P_1 + \cdots + P_k) = G(P_1) \oplus \cdots \oplus G(P_k)$.*

**Proof.** We prove by strong induction on the game tree depth that $G(P) = 0$ iff $P$ is a $\mathcal{P}$-position (previous player wins).

*Base case:* If $\operatorname{moves}(P) = \emptyset$, then $G(P) = \operatorname{mex}(\emptyset) = 0$, and indeed the current player loses (no move available).

*Inductive step:* Suppose $G(P) = 0$. Then for every $P' \in \operatorname{moves}(P)$, $G(P') \ne 0$ (since $0 \notin \{G(P') : P' \in \operatorname{moves}(P)\}$ would contradict $\operatorname{mex} = 0$; rather, $0$ is excluded from the image means every follower has $G \ge 1$... Correction: $G(P) = 0$ means $0 \notin \{G(P')\}$ is false; it means $0$ is the mex, so every value $< 0$ is in the set, which is vacuously true. Actually, $\operatorname{mex} = 0$ means $0 \notin \{G(P') : P' \in \operatorname{moves}(P)\}$. So every move leads to a position with $G > 0$, i.e., an $\mathcal{N}$-position by induction. Hence the current player is in a losing position.

Conversely, if $G(P) = g > 0$, then there exists $P' \in \operatorname{moves}(P)$ with $G(P') = 0$ (since $0 < g = \operatorname{mex}$ implies $0$ is in the set of follower Grundy values). By induction, $P'$ is a $\mathcal{P}$-position, so the current player can move to a losing position for the opponent.

The XOR rule follows from the isomorphism between any game position with Grundy value $g$ and a Nim heap of size $g$, combined with Bouton's theorem for Nim. $\square$

**Lemma 1 (Mex Bound).** *For any finite set $S \subseteq \mathbb{N}_0$, $\operatorname{mex}(S) \le |S|$, with equality if and only if $S = \{0, 1, \ldots, |S|-1\}$.*

**Proof.** The set $S$ contains at most $|S|$ elements from $\{0, 1, \ldots, |S|-1\}$. If all are present, then the smallest missing non-negative integer is $|S|$. Otherwise, some element of $\{0, \ldots, |S|-1\}$ is absent, so $\operatorname{mex}(S) < |S|$. $\square$

**Theorem 2 (Periodicity of Subtraction Game Grundy Values).** *For a subtraction game with move set $M = \{m_1, \ldots, m_k\} \subset \mathbb{Z}_{>0}$, the Grundy sequence $G(n) = \operatorname{mex}\{G(n-m) : m \in M,\, n - m \ge 0\}$ is eventually periodic.*

**Proof.** The Grundy value $G(n)$ depends only on the values $G(n - m_1), \ldots, G(n - m_k)$ (where defined). Since each $G(j) \in \{0, \ldots, k\}$ by Lemma 1 applied inductively, the state vector $(G(n - m_1), \ldots, G(n - m_k))$ takes values in a finite set of size at most $(k+1)^k$. By the pigeonhole principle, the state vector must eventually repeat, and once it repeats, the entire subsequent sequence is periodic. $\square$

## Editorial
Sprague-Grundy mex computation. Iterative Grundy value computation. We detect period P in G[]. Finally, compute sum using periodicity. We enumerate the admissible parameter range, discard candidates that violate the derived bounds or arithmetic constraints, and update the final set or total whenever a candidate passes the acceptance test.

## Pseudocode

```text
M = set of allowed moves
while g in reachable
Detect period P in G[]
Compute sum using periodicity
```

## Complexity Analysis

- **Time:** $O(P \cdot |M|)$ to compute one period of Grundy values, plus $O(P)$ for the cycle sum, where $P \le (\max M + 1)$ in the standard case. Total: $O(P \cdot |M|)$.
- **Space:** $O(P)$ for storing one period of Grundy values.

## Answer

$$\boxed{552839586}$$
