# Problem 89: Roman Numerals

## Problem Statement

The Project Euler data file contains one thousand Roman numerals written in valid (but not necessarily minimal) form. Find the number of characters saved by rewriting each numeral in its minimal form.

## Mathematical Analysis

### Roman Numeral Rules

The seven Roman numeral symbols and their values:

| Symbol | Value |
|--------|-------|
| I      | 1     |
| V      | 5     |
| X      | 10    |
| L      | 50    |
| C      | 100   |
| D      | 500   |
| M      | 1000  |

### Subtractive Notation (Minimal Form)

In minimal form, subtractive notation is used:
- IV = 4, IX = 9
- XL = 40, XC = 90
- CD = 400, CM = 900

### Parsing Roman Numerals

To convert a Roman numeral string to an integer: scan left to right. If the current symbol's value is less than the next symbol's value, subtract it; otherwise, add it.

### Generating Minimal Form

To convert an integer to minimal Roman numeral form, greedily subtract the largest possible value from the following ordered table:

| Representation | Value |
|----------------|-------|
| M              | 1000  |
| CM             | 900   |
| D              | 500   |
| CD             | 400   |
| C              | 100   |
| XC             | 90    |
| L              | 50    |
| XL             | 40    |
| X              | 10    |
| IX             | 9     |
| V              | 5     |
| IV             | 4     |
| I              | 1     |

### Editorial
Each input numeral is already valid, so there is no need to reason about alternative parses. We simply convert it to its integer value and then rebuild the unique minimal Roman representation of that same value. The number of saved characters is just the difference in lengths.

Both transformations are straightforward. Parsing is determined by the subtractive rule: a symbol contributes negatively only when it stands immediately before a larger symbol. Rebuilding is greedy from the standard minimal token table, because the special subtractive forms are already included. So the candidates are the symbols or token pairs allowed by Roman notation, and the minimal-form table is what constrains the output to the shortest legal representation.

### Pseudocode

```text
Set the total number of saved characters to 0.

For each Roman numeral in the data file:
    Parse it from left to right into an integer value
    Rebuild that value as a minimal Roman numeral using the ordered token table
    Add the difference in string lengths to the running total

Return the running total.
```

### Examples of Savings

- `IIII` (4 chars) becomes `IV` (2 chars): saves 2.
- `VIIII` (5 chars) becomes `IX` (2 chars): saves 3.
- `DCCCC` (5 chars) becomes `CM` (2 chars): saves 3.

## Correctness

**Theorem.** The method described above computes exactly the quantity requested in the problem statement.

*Proof.* The preceding analysis identifies the admissible objects and derives the formula, recurrence, or exhaustive search carried out by the algorithm. The computation evaluates exactly that specification, so every valid contribution is included once and no invalid contribution is counted. Therefore the returned value is the required answer. $\square$

## Complexity Analysis

- **Time:** $O(N \cdot L)$ where $N = 1000$ numerals and $L$ is the average length. Essentially $O(1)$ since all values are bounded.
- **Space:** $O(N \cdot L)$.

## Answer

$$\boxed{743}$$
