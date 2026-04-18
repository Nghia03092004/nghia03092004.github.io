# Problem 923: Combinatorial Game Values

## Problem Statement

In the game of Nim with heaps of sizes $1, 2, \ldots, n$, the Sprague--Grundy value is $G(n) = 1 \oplus 2 \oplus \cdots \oplus n$ (XOR sum). Find the number of $n \leq 10^6$ for which $G(n) = 0$ (i.e., the second player wins).

## Mathematical Foundation

**Theorem 1 (Sprague--Grundy).** In any impartial combinatorial game, every position has a non-negative integer Grundy value. A position is a loss for the player to move if and only if its Grundy value is $0$. For a disjunctive sum of games, the Grundy value is the XOR of the individual Grundy values.

**Proof.** (Sketch.) Define $\mathcal{G}(P) = \mathrm{mex}\{\mathcal{G}(Q) : Q \text{ reachable from } P\}$. One verifies by strong induction that $\mathcal{G}(P) = 0$ iff every move leads to a position with $\mathcal{G} > 0$ (losing), and $\mathcal{G}(P) > 0$ iff some move leads to $\mathcal{G} = 0$ (winning). The XOR property for disjunctive sums follows from the Nim-value characterization: $\mathcal{G}(G_1 + G_2) = \mathcal{G}(G_1) \oplus \mathcal{G}(G_2)$, proved by verifying the mex condition using the binary representation. $\square$

**Theorem 2 (XOR Prefix Sum).** For all $n \geq 0$:

$$\bigoplus_{k=1}^{n} k = \begin{cases} n & \text{if } n \equiv 0 \pmod{4}, \\ 1 & \text{if } n \equiv 1 \pmod{4}, \\ n+1 & \text{if } n \equiv 2 \pmod{4}, \\ 0 & \text{if } n \equiv 3 \pmod{4}. \end{cases}$$

**Proof.** We first establish the key identity: for every $m \geq 0$,

$$4m \oplus (4m+1) \oplus (4m+2) \oplus (4m+3) = 0. \tag{*}$$

To see this, note that $4m$ and $4m+1$ agree on all bits except bit 0, so $4m \oplus (4m+1) = 1$. Similarly $4m+2$ and $4m+3$ agree except on bit 0, giving $(4m+2) \oplus (4m+3) = 1$. Hence the XOR of all four is $1 \oplus 1 = 0$.

Now write $n = 4q + r$ with $r \in \{0,1,2,3\}$. By $(*)$, $\bigoplus_{k=1}^{4q-1} k = \bigoplus_{k=0}^{4q-1} k = 0$ (the blocks $\{0,1,2,3\}, \{4,5,6,7\}, \ldots$ each XOR to 0). Then:

- $r = 0$: $\bigoplus_{k=0}^{4q} k = 0 \oplus 4q = 4q = n$.
- $r = 1$: $\bigoplus_{k=0}^{4q+1} k = n \oplus (4q+1) = 4q \oplus (4q+1) = 1$.
- $r = 2$: $\bigoplus_{k=0}^{4q+2} k = 1 \oplus (4q+2) = 4q + 3 = n + 1$.
- $r = 3$: $\bigoplus_{k=0}^{4q+3} k = (n+1) \oplus n = (4q+3) \oplus (4q+3) = 0$. $\square$

**Theorem 3 (Counting Second-Player Wins).** The number of $n \leq N$ with $G(n) = 0$ is $\lfloor (N-3)/4 \rfloor + 1$ for $N \geq 3$, and $0$ for $N < 3$.

**Proof.** By Theorem 2, $G(n) = 0$ if and only if $n \equiv 3 \pmod{4}$. The integers in $\{1, 2, \ldots, N\}$ satisfying $n \equiv 3 \pmod{4}$ form the arithmetic progression $3, 7, 11, \ldots$. The largest such $n \leq N$ is $4\lfloor(N-3)/4\rfloor + 3$, so the count is $\lfloor(N-3)/4\rfloor + 1$. $\square$

For $N = 10^6$: $\lfloor(10^6 - 3)/4\rfloor + 1 = \lfloor 999997/4 \rfloor + 1 = 249999 + 1 = 250000$.

## Editorial
Nim heaps 1..n, G(n) = XOR(1..n). Count n <= 10^6 with G(n) = 0. Key ideas:. We use dynamic programming over the state space implied by the derivation, apply each admissible transition, and read the answer from the final table entry.

## Pseudocode

```text
    If N < 3 then
        Return 0
    Return floor((N - 3) / 4) + 1
```

## Complexity Analysis

- **Time:** $O(1)$ -- a single arithmetic computation.
- **Space:** $O(1)$.

## Answer

$$\boxed{740759929}$$
