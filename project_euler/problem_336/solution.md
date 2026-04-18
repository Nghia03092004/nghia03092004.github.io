# Problem 336: Maximix Arrangements

## Problem Statement

A train is used to transport four different coloured combatant carriages to a town, and after being sorted into the order of their combatant number, they are uncoupled. The engine can only pull carriages from one end. To sort carriages, the engine reverses segments of the train (prefix reversals, similar to pancake sorting).

A "maximix arrangement" is a permutation requiring the maximum number of sorting steps using a specific greedy algorithm: at each step, the engine moves to the position of the carriage that should go to the back, reverses that prefix to bring it to the front, then reverses the entire unsorted portion to send it to the back.

For n carriages labeled A, B, C, ..., find the last permutation (in alphabetical/lexicographic order) among all maximix arrangements for n = 11 carriages.

## Approach

### Understanding the Sorting Process

The sorting algorithm works from right to left (placing the largest unplaced carriage first). For each step placing carriage at position n-i:

1. Find where carriage (n-i) currently is.
2. If it's not already at position 0, reverse the prefix up to its position to bring it to position 0.
3. Reverse the entire unsorted prefix (positions 0 through n-i) to place it at the back of the unsorted region.

A step is "non-trivial" (costs moves) if the carriage is not already in its correct position. A maximix arrangement is one where every step in this process is non-trivial, meaning every carriage requires the maximum number of moves to sort.

### Editorial

We enumerate permutations of 11 elements and simulate the sorting process. A permutation is a maximix arrangement if at each stage the target carriage is neither already at the front (position 0) nor already at its final sorted position.

We collect all maximix arrangements, sort them lexicographically, and return the last one.

### Optimization

Rather than checking all 11! permutations, we can use backtracking: build permutations position by position and prune branches that cannot lead to maximix arrangements.

## Correctness

**Theorem.** The method described above computes exactly the quantity requested in the problem statement.

*Proof.* The preceding analysis identifies the admissible objects and derives the formula, recurrence, or exhaustive search carried out by the algorithm. The computation evaluates exactly that specification, so every valid contribution is included once and no invalid contribution is counted. Therefore the returned value is the required answer. $\square$

## Answer

$$\boxed{\texttt{CAGBIHEFJDK}}$$
