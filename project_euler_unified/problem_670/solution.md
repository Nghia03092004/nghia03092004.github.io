# Problem 670: Colouring a Strip

## Problem Statement

Tiles of sizes $1\times 1$, $1\times 2$, and $1\times 3$ in four colours tile a $2 \times n$ rectangle. Rules: (1) no four tiles meet at a point, (2) adjacent tiles differ in colour. Let $F(n)$ be the count of valid tilings.

Given: $F(2) = 120$, $F(5) = 45876$, $F(100) \equiv 53275818 \pmod{1{,}000{,}004{,}321}$.

Find $F(10^{16}) \bmod 1{,}000{,}004{,}321$.

## Mathematical Analysis

### Transfer Matrix Method

Model the tiling as a finite automaton. The **state** at column boundary $x$ encodes which cells in columns $x, x+1, \ldots$ are already occupied by tiles starting in earlier columns. With tiles up to $1 \times 3$, the state must track overhang up to 2 columns ahead.

**Definition.** A *profile* at position $x$ is a pair $(c_0, c_1)$ where $c_i \in \{0, 1, 2, 3, 4\}^2$ describes the status and colour of each of the 2 cells in column $x+i$ ($0 =$ empty, $1$-$4 =$ colour of occupying tile).

### State Enumeration

The state space $\mathcal{S}$ consists of all valid profiles. The **transfer matrix** $T$ is defined by:
$$T_{ij} = \text{number of ways to extend profile } i \text{ to profile } j \text{ by placing tiles at column } x$$

This accounts for all valid tile placements (sizes $1\times 1$ to $1\times 3$, in 4 colours) that:
- Fill all empty cells at column $x$
- Respect the colour-adjacency constraint
- Respect the no-four-corners constraint

### Matrix Exponentiation

$$F(n) = \mathbf{v}_0^T \cdot T^{n-1} \cdot \mathbf{v}_f \pmod{p}$$

where $\mathbf{v}_0$ is the initial state (all cells empty) and $\mathbf{v}_f$ is the terminal condition (no overhang).

**Theorem.** Matrix exponentiation by repeated squaring computes $T^{n-1}$ in $O(s^3 \log n)$ operations, where $s = |\mathcal{S}|$.

### Modular Arithmetic

$p = 1{,}000{,}004{,}321$ is prime, so all arithmetic is performed in $\mathbb{F}_p$.

## Concrete Examples

| $n$ | $F(n)$ | Notes |
|-----|---------|-------|
| 1 | 12 | Just 2 cells, 4 colours each minus same-colour |
| 2 | 120 | Given |
| 5 | 45876 | Given |
| 100 | $53275818 \pmod{p}$ | Given |

### State Space Size

With 2 rows and overhang up to 2 columns, the raw state count is $(5)^4 = 625$. After pruning invalid states (conflicting colours, impossible configurations), the effective state count $s$ is much smaller, typically $s \approx 50$-$200$.

## Derivation

### Algorithm

1. **Enumerate states:** Generate all valid profiles encoding overhang and colours.
2. **Build transfer matrix:** For each pair of states, count valid tile placements.
3. **Matrix power:** Compute $T^{n-1} \bmod p$ via repeated squaring ($\sim 53$ squarings for $n = 10^{16}$).
4. **Extract answer:** Multiply by initial/terminal vectors.

### Colour Adjacency

Two tiles are **adjacent** if they share an edge (not just a corner). The adjacency constraint requires different colours for adjacent tiles. With 4 colours, this is a proper $4$-colouring of the tile adjacency graph.

### No-Four-Corners Constraint

At any interior lattice point, at most 3 tiles may meet. This prevents certain $2 \times 2$ configurations where all 4 tiles differ.

## Proof of Correctness

**Theorem.** *The transfer matrix method correctly counts all valid tilings.*

*Proof.* The state at each column boundary completely determines the constraints on future tile placements. The transfer matrix enumerates all valid one-column extensions. By induction on $n$, $T^{n-1}$ counts all valid $n$-column tilings. $\square$

**Lemma.** *The modular computation is exact since $p$ is prime and all intermediate values are bounded by $p^2 < 2^{64}$.*

## Complexity Analysis

- **State enumeration:** $O(s)$ where $s$ is the number of valid profiles.
- **Transfer matrix construction:** $O(s^2 \cdot 4^{O(1)})$ for checking all transitions.
- **Matrix exponentiation:** $O(s^3 \log n)$ operations in $\mathbb{F}_p$.
- **Total:** $O(s^3 \cdot 53)$ for $n = 10^{16}$.

With $s \approx 100$, this is $\sim 10^6 \times 53 \approx 5 \times 10^7$ operations.

## Answer

$$\boxed{551055065}$$
