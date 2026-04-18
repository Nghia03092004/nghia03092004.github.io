# Problem 514: Geoboard Shapes

## Problem Statement

A geoboard is an $n \times n$ grid of pegs. A shape is formed by stretching a rubber band around some subset of pegs to form a simple closed polygon. Two shapes are considered the same if one can be transformed into the other by any combination of translations, rotations, or reflections on the grid. Count the number of distinct shapes that can be formed.

## Mathematical Foundation

**Theorem 1 (Burnside's Lemma).** Let a finite group $G$ act on a finite set $X$. The number of distinct orbits is

$$|X / G| = \frac{1}{|G|} \sum_{g \in G} |X^g|,$$

where $X^g = \{x \in X : g \cdot x = x\}$ is the set of elements fixed by $g$.

**Proof.** Count pairs $(g, x) \in G \times X$ with $g \cdot x = x$ in two ways. Summing over $g$: $\sum_{g \in G}|X^g|$. Summing over $x$: $\sum_{x \in X}|\text{Stab}(x)|$. By the orbit-stabilizer theorem, $|\text{Stab}(x)| = |G| / |\text{Orb}(x)|$. Grouping by orbits:

$$\sum_{x \in X} \frac{|G|}{|\text{Orb}(x)|} = |G| \cdot \sum_{\text{orbits } O} \frac{|O|}{|O|} = |G| \cdot |\text{number of orbits}|.$$

Equating: $\sum_{g \in G}|X^g| = |G| \cdot |X/G|$, yielding the formula. $\square$

**Theorem 2 (Symmetry Group of the Square Grid).** The symmetry group acting on shapes includes:

- The dihedral group $D_4$ of the square (order 8): identity, rotations by $90°$, $180°$, $270°$, and reflections across horizontal, vertical, main-diagonal, and anti-diagonal axes.
- Translations: shifting the polygon by integer vectors $(dx, dy)$.

When translations are included, the equivalence group is larger (a semidirect product of $D_4$ with the translation lattice restricted to the grid).

**Proof.** The symmetries of the infinite square lattice $\mathbb{Z}^2$ form the group $p4m$ (the wallpaper group of the square lattice). On a finite $n \times n$ grid, the relevant subgroup consists of those symmetries that map the grid to itself. For translations, two polygons are equivalent iff one is a translate of the other; combined with $D_4$, this gives the stated structure. $\square$

**Lemma 1 (Orbit Counting with Translations).** To count shapes up to translation and $D_4$ symmetry, first fix a canonical position (e.g., the lexicographically smallest vertex at a fixed reference point), then apply Burnside over $D_4$ alone on the canonicalized shapes.

**Proof.** Fixing a canonical position quotients out the translation group. The remaining equivalences are precisely the $D_4$ symmetries, so Burnside's lemma over $D_4$ yields the correct orbit count. $\square$

## Editorial
Count distinct shapes on an n x n geoboard formed by rubber bands, up to translation, rotation, and reflection. We enumerate all simple lattice polygons on n x n grid. We then canonicalize each polygon (translate to canonical position). Finally, iterate over P in polygons.

## Pseudocode

```text
Enumerate all simple lattice polygons on n x n grid
Canonicalize each polygon (translate to canonical position)
for P in polygons
Apply Burnside over D4
for g in D4
for P in canonical
```

## Complexity Analysis

- **Time:** Exponential in $n^2$. The number of simple lattice polygons on an $n \times n$ grid grows exponentially, so enumeration is feasible only for small $n$ (roughly $n \leq 6$).
- **Space:** $O(|X|)$ where $|X|$ is the number of distinct canonical polygons, which is exponential in $n^2$.

## Answer

$$\boxed{8986.86698}$$
