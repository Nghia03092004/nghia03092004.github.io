# Problem 300: Protein Folding

## Problem Statement

A protein of length $n$ is a string over $\{H, P\}$ (hydrophobic/polar). It folds as a self-avoiding walk (SAW) on the 2D square lattice. An **H-H contact** occurs when two H residues at non-consecutive sequence positions occupy adjacent lattice sites. Each protein folds optimally to maximize H-H contacts.

Reference: for $n = 8$, the average optimal contact count is $850/2^8 = 3.3203125$.

Find the average number of H-H contacts in an optimal folding of a random protein of length 15. Give an exact decimal.

## Mathematical Foundation

**Definition.** A *self-avoiding walk* (SAW) of length $n-1$ on $\mathbb{Z}^2$ is an injective sequence of lattice points $(v_0, v_1, \ldots, v_{n-1})$ where each $|v_{i+1} - v_i|_1 = 1$ (adjacent on the lattice).

**Definition.** The *contact topology* of a SAW is the set $\mathcal{C} = \{(i, j) : |i - j| > 1,\; |v_i - v_j|_1 = 1\}$ of non-consecutive adjacent pairs.

**Theorem 1 (Decomposition of Contacts).** *The total H-H contacts for protein $p = (p_0, \ldots, p_{n-1})$ under walk $w$ equals*

$$\text{contacts}(p, w) = \text{consec}(p) + \text{non-consec}(p, w)$$

*where $\text{consec}(p) = |\{i : p_i = H \text{ and } p_{i+1} = H\}|$ depends only on $p$, and $\text{non-consec}(p, w) = |\{(i,j) \in \mathcal{C}(w) : p_i = H \text{ and } p_j = H\}|$ depends on both $p$ and $w$.*

**Proof.** Consecutive residues ($|i - j| = 1$) are always lattice-adjacent in any SAW, so their H-H contacts are determined solely by the protein string. Non-consecutive contacts depend on the walk geometry. $\square$

**Theorem 2 (Optimality via Topology Enumeration).** *The optimal contact count for protein $p$ is*

$$\text{opt}(p) = \text{consec}(p) + \max_{\mathcal{C} \in \mathcal{T}} \sum_{(i,j) \in \mathcal{C}} \mathbf{1}[p_i = H \wedge p_j = H]$$

*where $\mathcal{T}$ is the set of all realizable contact topologies for SAWs of length $n - 1$.*

**Proof.** Since $\text{consec}(p)$ is constant across all walks, maximizing total contacts is equivalent to maximizing non-consecutive contacts. The non-consecutive contacts depend only on the contact topology $\mathcal{C}(w)$, so it suffices to enumerate all distinct topologies rather than all walks. $\square$

**Lemma 1 (Bitmask Evaluation).** *Representing protein $p$ as a bitmask $B \in \{0, 1\}^n$ (where bit $i = 0$ means $p_i = H$ and bit $i = 1$ means $p_i = P$), the contact pair $(i, j)$ contributes iff $(B \mathbin{\&} M_{ij}) = 0$ where $M_{ij} = (1 \ll i) \mid (1 \ll j)$.*

**Proof.** We have $p_i = H$ iff bit $i$ of $B$ is 0, and $p_j = H$ iff bit $j$ is 0. Thus both are H iff $B$ has 0 in positions $i$ and $j$, i.e., $(B \mathbin{\&} M_{ij}) = 0$. $\square$

**Lemma 2 (Symmetry Reduction for SAWs).** *By the 4-fold rotational symmetry and 2-fold reflective symmetry of the square lattice (dihedral group $D_4$), fixing the first step to go rightward reduces the SAW enumeration by a factor of 4. Mirror-image walks produce the same contact topology, further reducing by a factor of 2 in many cases.*

**Proof.** Under rotation by $90°$, a SAW $w$ maps to $w'$ with the same contact topology (since adjacency on $\mathbb{Z}^2$ is rotation-invariant). Fixing the first step eliminates 4 equivalent rotations. Reflection about the $x$-axis also preserves contact topology, though some walks are self-mirror-symmetric and must be handled carefully. $\square$

**Theorem 3 (Exact Rationality).** *The average optimal contact count $E[\text{opt}(p)]$ over all $2^n$ proteins is a rational number with denominator dividing $2^n$.*

**Proof.** Each protein is equally likely with probability $2^{-n}$, and each $\text{opt}(p)$ is a non-negative integer. Thus $E[\text{opt}(p)] = \frac{1}{2^n} \sum_{p} \text{opt}(p) \in \frac{1}{2^n}\mathbb{Z}$. $\square$

## Algorithm

```
function solve(n):
    // Step 1: Enumerate all SAWs of n-1 steps (first step fixed rightward)
    topologies = set()

    function backtrack(pos, visited, step, contacts):
        if step == n - 1:
            topologies.add(frozenset(contacts))
            return
        for each direction in {up, down, left, right}:
            next_pos = pos + direction
            if next_pos in visited: continue
            new_contacts = contacts.copy()
            for i, v in enumerate(visited_list):
                if |next_pos - v| == 1 and step + 1 - i > 1:
                    new_contacts.add((i, step + 1))
            visited.add(next_pos)
            backtrack(next_pos, visited, step + 1, new_contacts)
            visited.remove(next_pos)

    backtrack((1,0), {(0,0), (1,0)}, 1, {})

    // Step 2: For each protein bitmask, find max contacts over topologies
    total = 0
    for B = 0 to 2^n - 1:
        consec = count consecutive HH pairs in B
        best_non_consec = 0
        for C in topologies:
            nc = count of (i,j) in C where (B & ((1<<i)|(1<<j))) == 0
            best_non_consec = max(best_non_consec, nc)
        total += consec + best_non_consec

    return total / 2^n
```

## Complexity Analysis

- **Time:** $O(W + |\mathcal{T}| \cdot 2^n)$ where $W$ is the number of SAWs (approximately $4 \times 10^6$ for 14 steps before symmetry reduction, $\sim 10^6$ after), and $|\mathcal{T}|$ is the number of distinct topologies (approximately $5{,}000$). The evaluation loop costs $|\mathcal{T}| \cdot 2^{15} = 5000 \times 32768 \approx 1.6 \times 10^8$ operations.
- **Space:** $O(|\mathcal{T}| \cdot \overline{c})$ for storing topologies, where $\overline{c}$ is the average number of contact pairs per topology. Plus $O(n)$ for the backtracking stack.

## Answer

$$\boxed{8.0540771484375}$$
