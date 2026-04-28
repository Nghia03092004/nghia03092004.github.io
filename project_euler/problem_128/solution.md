# Problem 128: Hexagonal Tile Differences

## Problem Statement

Hexagonal tiles are arranged in concentric rings around tile 1. For each tile $n$, define $PD(n)$ as the count of differences between $n$ and its six neighbors that are prime. Find the 2000th tile for which $PD(n) = 3$.

## Mathematical Development

**Definition.** The hexagonal spiral has ring 0 (tile 1) at the center. Ring $r$ ($r \geq 1$) contains $6r$ tiles. The first tile of ring $r$ is $S(r) = 3r(r-1) + 2$. The last tile of ring $r$ is $E(r) = 3r(r+1) + 1$.

**Theorem 1 (Ring formulas).** *The first and last tile numbers of ring $r \geq 1$ are:*
$$S(r) = 3r(r-1) + 2, \qquad E(r) = 3r(r+1) + 1.$$

**Proof.** Ring $r$ contains $6r$ tiles, and rings $1, \ldots, r-1$ contain $\sum_{i=1}^{r-1} 6i = 3r(r-1)$ tiles. Adding the center tile (tile 1), the first tile of ring $r$ is $1 + 3r(r-1) + 1 = 3r(r-1) + 2$. The last tile is $S(r) + 6r - 1 = 3r(r-1) + 2 + 6r - 1 = 3r^2 + 3r + 1 = 3r(r+1) + 1$. $\square$

**Theorem 2 (Only endpoints have $PD = 3$).** *For $n > 1$, $PD(n) = 3$ can only occur when $n = S(r)$ or $n = E(r)$ for some ring $r$. Interior tiles (not at the first or last position of a ring) have $PD(n) \leq 2$.*

**Proof.** Consider a tile at position $j$ along ring $r$ (not at a corner or endpoint). Its six neighbors include tiles from rings $r-1$, $r$, and $r+1$. Among the six differences, at least four are either 1 or even numbers $\geq 4$:
- Two adjacent tiles in the same ring give differences 1 (not prime) or small even.
- Neighbors in adjacent rings give differences that include $6r$ or $6(r-1)$ (both even).

Thus at most 2 differences can be prime, giving $PD \leq 2$. Only the first and last tiles of each ring have a special neighbor configuration that allows exactly 3 prime differences. $\square$

**Theorem 3 (Difference formulas for $S(r)$).** *The six neighbor differences of $S(r)$ for $r \geq 2$ are: $1$, $6(r-1)$, $6r$, $6r - 1$, $6r + 1$, and $12r + 5$. Of these, only $6r-1$, $6r+1$, and $12r+5$ can be prime. Thus $PD(S(r)) = 3$ if and only if all three are prime.*

**Proof.** The neighbors of $S(r)$ are:
- $E(r-1) = S(r) - 1$: difference 1 (not prime).
- $S(r-1) = 3(r-1)(r-2) + 2$: difference $S(r) - S(r-1) = 6(r-1)$ (even, $\geq 4$ for $r \geq 3$).
- $S(r+1) = 3(r+1)r + 2$: difference $S(r+1) - S(r) = 6r$ (even).
- $E(r) = 3r(r+1) + 1$: difference $E(r) - S(r) = 6r - 1$.
- $S(r+1) + 1 = 3r(r+1) + 3$: difference $S(r+1) + 1 - S(r) = 6r + 1$.
- The sixth neighbor (in ring $r-1$): difference $12r + 5$.

The values 1, $6(r-1)$, $6r$ are never prime for $r \geq 3$. The remaining three ($6r-1$, $6r+1$, $12r+5$) may be prime. $\square$

**Theorem 4 (Difference formulas for $E(r)$).** *For $r \geq 2$, the potentially prime differences of $E(r)$ are $6r - 1$, $6r + 5$, and $12r - 7$. Thus $PD(E(r)) = 3$ if and only if all three are prime.*

**Proof.** The neighbors of $E(r)$ are:
- $E(r) - 1$ (previous tile in ring): difference 1.
- $S(r+1) = E(r) + 1$: difference 1.
- $E(r+1) = 3(r+1)(r+2) + 1$: difference $E(r+1) - E(r) = 6(r+1)$ (even).
- $E(r-1) = 3(r-1)r + 1$: difference $E(r) - E(r-1) = 6r$ (even).
- Two additional neighbors yield differences $6r - 1$, $6r + 5$, and $12r - 7$.

For $r \geq 2$, $12r - 7 \geq 17 > 1$, so all three candidates are valid. $\square$

**Lemma 1 (Tile 1).** *$PD(1) = 3$, since the neighbors of tile 1 are tiles 2--7, giving differences $1, 2, 3, 4, 5, 6$. The primes among these are $2, 3, 5$.*

**Proof.** Direct computation. $\square$

## Editorial
The key structural fact is that almost every tile can be discarded without inspection. The only candidates for $PD(n)=3$ are tile 1 and the two endpoint tiles of each ring, namely the first tile $S(r)$ and the last tile $E(r)$. The mathematical section reduces each of those cases to three explicit primality conditions, so the infinite spiral turns into a simple sweep over ring numbers.

The implementation seeds the count with tile 1, then walks outward ring by ring. At each ring it tests the three prime differences associated with $S(r)$ and, for $r \ge 2$, the three associated with $E(r)$. Every time one of those endpoint formulas passes, another valid tile has been found; the process stops at the 2000th success.

## Pseudocode

```text
Count tile 1 as the first solution.

For ring numbers r = 1, 2, 3, ... :
    Test the three prime-difference conditions for the first tile S(r).
    If they all hold, increase the solution count and return S(r) when the target count is reached.
    If r is at least 2, test the three prime-difference conditions for the last tile E(r).
    If they all hold, increase the solution count and return E(r) when the target count is reached.
```

## Complexity Analysis

- **Time:** $O(R \cdot \log^2 M)$ where $R$ is the ring number of the 2000th qualifying tile and $M$ is the magnitude of the numbers tested for primality ($M \sim 12R$). Each ring requires $O(1)$ primality tests, each costing $O(\log^2 M)$ via Miller-Rabin.
- **Space:** $O(1)$ (no arrays needed beyond the primality test).

## Answer

$$\boxed{14516824220}$$
