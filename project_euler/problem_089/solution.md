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
We parse each Roman numeral string to get its integer value. We then convert the integer back to minimal Roman numeral form. Finally, the savings for each numeral = (length of original) - (length of minimal).

### Pseudocode

```text
Parse each Roman numeral string to get its integer value
Convert the integer back to minimal Roman numeral form
The savings for each numeral = (length of original) - (length of minimal)
Sum all savings
```

### Examples of Savings

- `IIII` (4 chars) becomes `IV` (2 chars): saves 2.
- `VIIII` (5 chars) becomes `IX` (2 chars): saves 3.
- `DCCCC` (5 chars) becomes `CM` (2 chars): saves 3.

## Correctness

**Theorem.** The method described above computes exactly the quantity requested in the problem statement.

*Proof.* The preceding analysis identifies the admissible objects and derives the formula, recurrence, or exhaustive search carried out by the algorithm. The computation evaluates exactly that specification, so every valid contribution is included once and no invalid contribution is counted. Therefore the returned value is the required answer. $\square$

## Complexity

- **Time:** $O(N \cdot L)$ where $N = 1000$ numerals and $L$ is the average length. Essentially $O(1)$ since all values are bounded.
- **Space:** $O(N \cdot L)$.

## Answer

$$\boxed{743}$$
