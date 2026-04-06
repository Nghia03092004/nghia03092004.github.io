# Problem 244: Sliders

## Problem Statement

A sliding puzzle on a $4 \times 4$ grid contains Red (R), Blue (B), and one White/empty (W) tile arranged in a specific starting configuration. Tiles adjacent to the empty space may slide into it. Each move is encoded by the direction the empty space moves: L (76), R (82), U (85), D (68) -- using ASCII values.

For a move sequence $m_1, m_2, \ldots, m_k$ reaching a configuration, its **checksum** is:
$$C = \left(\sum_{i=1}^{k} d_i \cdot 243^{k-i}\right) \bmod 100000007$$
where $d_i$ is the ASCII value of the $i$-th move direction.

Find the sum of the checksums over all reachable configurations (using the shortest path to each).

## Mathematical Foundation

**Theorem 1 (State space finiteness).** The state space of the puzzle is finite, with at most $\binom{8}{4} \times 8 = 560$ states for a 2x4 grid (choosing positions for 4 tiles of one colour, with 8 possible positions for the blank). More generally, for an $m \times n$ grid with $r$ red, $b$ blue, and 1 white tile, the state count is $\dfrac{(mn)!}{r!\,b!\,1!}$.

**Proof.** Each state is a bijection from grid positions to tile labels. Tiles of the same colour are indistinguishable, so we divide by the factorial of each colour count. $\square$

**Theorem 2 (BFS shortest-path optimality).** Breadth-first search from the initial state discovers every reachable state at minimum distance. If a state is first reached at distance $k$, no shorter path exists.

**Proof.** Standard BFS correctness: BFS explores states in non-decreasing order of distance. A state dequeued at distance $k$ cannot be reached at distance $< k$, since all states at distance $< k$ have already been dequeued. $\square$

**Lemma 1 (Checksum recurrence).** If a state $s'$ is reached from state $s$ by a move with direction code $d$, and the BFS checksum of $s$ is $C_s$, then the checksum of $s'$ via this path is:
$$C_{s'} = (C_s \cdot 243 + d) \bmod 100000007.$$

**Proof.** By definition, $C_{s'} = \sum_{i=1}^{k+1} d_i \cdot 243^{k+1-i} = 243 \cdot \sum_{i=1}^{k} d_i \cdot 243^{k-i} + d_{k+1} = 243 \cdot C_s + d$. $\square$

**Lemma 2 (Multiple shortest paths).** When multiple shortest paths lead to the same state $s'$, the problem requires the checksum of the lexicographically first (or any canonical) shortest path. In the BFS, we record the checksum of whichever shortest path discovers $s'$ first.

**Proof.** By the problem statement, each reachable configuration contributes exactly one checksum corresponding to its shortest path. BFS guarantees the first discovery is via a shortest path. $\square$

## Algorithm

```
function SolveSliders(initial_state):
    queue = deque([(initial_state, 0)])  # (state, checksum)
    visited = {initial_state: 0}         # state -> checksum
    total = 0

    while queue is not empty:
        state, checksum = queue.popleft()
        total = (total + checksum) % 100000007

        for each valid move direction d in {L, R, U, D}:
            new_state = apply_move(state, d)
            if new_state not in visited:
                new_checksum = (checksum * 243 + ascii(d)) % 100000007
                visited[new_state] = new_checksum
                queue.append((new_state, new_checksum))

    return total

function apply_move(state, direction):
    Find position of empty tile
    Compute neighbour position in given direction
    If valid, swap empty tile with neighbour
    Return new state (or None if invalid)
```

## Complexity Analysis

- **Time:** $O(|V| + |E|)$ where $|V|$ is the number of reachable states and $|E|$ is the number of edges (moves). For the given puzzle, $|V| \le 560$ and each state has at most 4 neighbours, so $|E| \le 2240$. Total: $O(1)$ in terms of the problem size.
- **Space:** $O(|V|)$ for the visited set and BFS queue, i.e., $O(560)$.

## Answer

$$\boxed{96356848}$$
