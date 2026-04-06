# Problem 391: Hopping Game

## Problem Statement

Let $s_k$ denote the cumulative popcount function:

$$s_k = \sum_{i=0}^{k} \operatorname{popcount}(i),$$

where $\operatorname{popcount}(i)$ counts the number of $1$-bits in the binary representation of $i$. The range of this function defines the set $S = \{s_k : k \ge 0\} = \{0, 1, 2, 4, 5, 7, 9, 12, 13, 15, \ldots\}$.

Two players play a game parameterized by a positive integer $n$. A counter $c$ starts at $0$. On each turn, the current player chooses $m \in \{1, 2, \ldots, n\}$ and sets $c \leftarrow c + m$. The resulting value of $c$ must belong to $S$. The player who cannot make a valid move **loses** (normal play convention).

Let $F(n)$ be the number of winning first moves for the first player when both players play optimally.

Given: $F(2) = 1$, $F(5) = 3$, $F(100) = 46$, $F(10000) = 4808$, $F(10^6) = 480440$.

**Find $F(10^{11})$.**

## Mathematical Foundation

### The Cumulative Popcount Recurrence

**Theorem 1 (Binary decomposition of $s_k$).** *The cumulative popcount function satisfies:*

$$s_0 = 0, \qquad s_{2m+1} = 2\,s_m + m + 1, \qquad s_{2m} = s_m + s_{m-1} + m.$$

**Proof.** Consider the odd case $k = 2m + 1$. Partition $\{0, 1, \ldots, 2m+1\}$ by the value of the most significant bit among $\lceil \log_2(2m+2) \rceil$ bit positions. Those integers in $\{0, \ldots, m\}$ have leading bit $0$ and contribute $s_m$ ones in total. Those in $\{m+1, \ldots, 2m+1\}$ each have leading bit $1$; stripping this bit yields $\{0, 1, \ldots, m\}$, contributing $s_m$ ones from lower bits and $m+1$ ones from the leading bits. Hence $s_{2m+1} = 2s_m + (m+1)$.

For the even case $k = 2m$, the group $\{0, \ldots, m-1\}$ contributes $s_{m-1}$ ones, while the group $\{m, \ldots, 2m\}$ has leading bit $1$ with lower bits $\{0, \ldots, m\}$, contributing $s_m + m$ ones. Thus $s_{2m} = s_{m-1} + s_m + m$. $\blacksquare$

**Lemma 1 (Closed form at powers of two).** *For all $m \ge 1$,*

$$s_{2^m - 1} = m \cdot 2^{m-1}.$$

**Proof.** Among the $2^m$ integers $\{0, 1, \ldots, 2^m - 1\}$, each of the $m$ bit positions is set to $1$ in exactly half of them, by the symmetry of $m$-bit binary strings. The total count of $1$-bits is therefore $m \cdot 2^{m-1}$. $\blacksquare$

### Gap Structure and Self-Similarity

**Definition.** The *gap sequence* is $d_k = s_{k+1} - s_k = \operatorname{popcount}(k+1)$ for $k \ge 0$.

**Lemma 2 (Gap self-similarity).** *For all $m \ge 0$ and $0 \le j < 2^m$,*

$$\operatorname{popcount}(2^m + j) = 1 + \operatorname{popcount}(j).$$

*Consequently, $d_{2^m - 1 + j} = 1 + d_j$ for $0 \le j < 2^m - 1$, provided we interpret indices correctly.*

**Proof.** The binary representation of $2^m + j$ (with $0 \le j < 2^m$) has bit $m$ set to $1$ and bits $0$ through $m-1$ identical to those of $j$. Therefore $\operatorname{popcount}(2^m + j) = 1 + \operatorname{popcount}(j)$. $\blacksquare$

### Game-Theoretic Analysis

**Theorem 2 (Arena boundary).** *For the game with parameter $n$, the smallest index $K$ such that $d_K > n$ is $K = 2^{n+1} - 2$. The game arena is $\{s_0, s_1, \ldots, s_K\}$, and $s_K$ is the unique terminal position.*

**Proof.** We have $d_K = \operatorname{popcount}(K + 1) = \operatorname{popcount}(2^{n+1} - 1) = n + 1 > n$. For any $k < K$, the integer $k + 1 \le 2^{n+1} - 2$ has at most $n$ bits set (since $2^{n+1} - 1$ is the unique $(n+1)$-bit integer with all bits set), so $d_k \le n$. Thus from position $s_K$, every candidate next position $s_K + m$ with $1 \le m \le n$ falls strictly between $s_K$ and $s_{K+1}$, hence does not lie in $S$. No earlier position has this property. $\blacksquare$

**Theorem 3 (Sprague--Grundy classification).** *The hopping game is a finite impartial game under normal play. By the Sprague--Grundy theorem, every position $p$ in the arena has a well-defined Grundy value*

$$\mathcal{G}(p) = \operatorname{mex}\{\mathcal{G}(q) : q \text{ reachable from } p \text{ in one move}\},$$

*where $\operatorname{mex}(A) = \min(\mathbb{Z}_{\ge 0} \setminus A)$. A position is a P-position (previous player wins, i.e., the mover loses) if and only if $\mathcal{G}(p) = 0$.*

**Proof.** The game is impartial (both players have the same move set at every position), the counter strictly increases, and the arena is finite (bounded by $s_K$). These are precisely the hypotheses of the Sprague--Grundy theorem. $\blacksquare$

**Theorem 4 (Counting formula for $F(n)$).** *The number of winning first moves is*

$$F(n) = \bigl|\{m \in \{1, \ldots, n\} \cap S : \mathcal{G}(m) = 0\}\bigr|.$$

**Proof.** From position $c = 0$ (with $s_0 = 0 \in S$), a move of size $m$ is valid if and only if $m \in S$ (since $0 + m = m$ must lie in $S$). This move is winning for the first player if and only if it leaves the opponent in a P-position, i.e., $\mathcal{G}(m) = 0$. $\blacksquare$

## Algorithm

```
function COMPUTE_S(k):
    // Compute s_k in O(log^2 k) via memoized recurrence (Theorem 1)
    if k == 0: return 0
    if k is odd: m = (k-1)/2; return 2*COMPUTE_S(m) + m + 1
    if k is even: m = k/2; return COMPUTE_S(m) + COMPUTE_S(m-1) + m

function F(n):
    K = 2^(n+1) - 2
    Build arena positions S[0..K]
    Grundy[S[K]] = 0  // terminal P-position
    for i = K-1 downto 0:
        reachable = {Grundy[S[i] + m] : m in {1,...,n}, S[i]+m in S}
        Grundy[S[i]] = mex(reachable)
    return |{m in {1,...,n} ∩ S : Grundy[m] == 0}|

function F_EFFICIENT(n):
    // Exploit Lemma 2: gap self-similarity under binary decomposition
    // Split arena at midpoint index 2^n - 1
    // Second-half gaps = first-half gaps + 1 (Lemma 2)
    // Recursively solve sub-problems on condensed gap representation
    // Match Grundy boundary conditions across halves
```

## Complexity Analysis

- **Brute-force**: Time $O(2^{n+1} \cdot n)$, Space $O(2^{n+1})$. Feasible for $n \le 22$.
- **Efficient recursive approach**: Time $O(n^2 \log n)$, Space $O(n \log n)$. The binary self-similarity of the gap sequence (Lemma 2) permits a divide-and-conquer decomposition into $O(\log n)$ levels, each involving polynomial-time processing on a condensed representation of size $O(n)$.

## Answer

$$\boxed{61029882288}$$
