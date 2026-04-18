# Problem 90: Cube Digit Pairs

## Problem Statement

Two cubes each have 6 faces with a distinct digit (0-9) on each face. By placing the two cubes side by side, we can form two-digit square numbers: 01, 04, 09, 16, 25, 36, 49, 64, 81.

The digit 6 and 9 are interchangeable (either can represent the other).

How many distinct arrangements of two cubes allow all nine square numbers to be displayed?

## Mathematical Analysis

### Setup

Each cube has 6 faces chosen from digits 0-9, so each cube is a 6-element subset of $\{0, 1, 2, 3, 4, 5, 6, 7, 8, 9\}$.

There are $\binom{10}{6} = 210$ possible cubes.

### Required Digit Pairs

The nine two-digit squares require these pairs (first digit, second digit):

| Square | Pair |
|--------|------|
| 01     | (0, 1) |
| 04     | (0, 4) |
| 09     | (0, 9) |
| 16     | (1, 6) |
| 25     | (2, 5) |
| 36     | (3, 6) |
| 49     | (4, 9) |
| 64     | (6, 4) |
| 81     | (8, 1) |

### 6/9 Interchangeability

Since 6 and 9 are interchangeable, whenever we check if a cube contains a 6 or 9, we check if it contains either 6 or 9. This effectively means that if a cube has a 6, it can also represent 9, and vice versa.

### Checking a Pair of Cubes

For each required pair $(a, b)$, the two cubes can display it if:
- Cube 1 has $a$ and Cube 2 has $b$, OR
- Cube 1 has $b$ and Cube 2 has $a$.

(With 6/9 equivalence applied.)

### Counting Arrangements

Since the two cubes are unordered (swapping them gives the same arrangement), we count pairs $(C_1, C_2)$ with $C_1 \le C_2$ (in some canonical ordering), or equivalently count all ordered pairs and divide by 2 for distinct cubes, adding back the cases where $C_1 = C_2$.

More simply: iterate over all pairs $i \le j$ from the 210 possible cubes.

Total pairs to check: $\binom{210}{2} + 210 = \frac{210 \times 211}{2} = 22155$.

### Editorial
Each cube is just a 6-element subset of the ten digits, so the full search space is small enough to enumerate directly. The only subtlety is the $6/9$ symmetry: once a cube carries either of those digits, it can serve as both when displaying a square.

The implementation first generates every possible cube, then checks every unordered pair of cubes. For a pair to be valid, every required square must be displayable in at least one orientation, meaning one cube supplies the tens digit and the other cube supplies the units digit, or vice versa. The candidates are therefore all cube pairs, and the nine square-digit requirements are the filter that discards invalid arrangements.

### Pseudocode

```text
Generate every 6-digit subset of the digits 0 through 9.
Set the answer count to 0.

For each unordered pair of generated cubes:
    Assume the pair is valid

    For each required square-digit pair:
        Check whether the two cubes can display it in one order or the other,
        treating 6 and 9 as interchangeable

        If this square cannot be displayed:
            mark the cube pair invalid and stop checking this pair

    If the pair remained valid:
        increase the answer count

Return the answer count.
```

## Correctness

**Theorem.** The method described above computes exactly the quantity requested in the problem statement.

*Proof.* The preceding analysis identifies the admissible objects and derives the formula, recurrence, or exhaustive search carried out by the algorithm. The computation evaluates exactly that specification, so every valid contribution is included once and no invalid contribution is counted. Therefore the returned value is the required answer. $\square$

## Complexity Analysis

- **Time:** $O\binom{210}{2} \times 9 = O(199{,}395)$. Trivially fast.
- **Space:** $O(210)$ for storing cubes.

## Answer

$$\boxed{1217}$$
