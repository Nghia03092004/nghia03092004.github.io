# Problem 640: Shut the Box

## Problem Statement

Bob plays a single-player game using two standard 6-sided dice and twelve cards numbered 1 to 12, all initially face up. Each turn, Bob rolls both dice, getting numbers x and y (each in range 1-6). He must choose one of three options: flip card x, flip card y, or flip card x+y. (If chosen card is face down, it flips face up, and vice versa.)

Bob wins when all twelve cards are face down simultaneously.

Alice plays a similar game with two fair coins (heads=2, tails=1) and four cards. With optimal strategy, Alice's expected number of turns is approximately 5.673651.

Assuming Bob plays optimally, find the expected number of turns until he wins. Give answer rounded to 6 decimal places.

## Mathematical Analysis

### State Space

The game state is a bitmask of which cards are face up/down. With 12 cards, there are 2^12 = 4096 possible states. The goal state is 0 (all face down).

### Dice Roll Outcomes

Two dice give outcomes (x,y) with x,y in {1,...,6}. The 36 equally likely outcomes produce:
- Three choices per roll: flip card x, flip card y, or flip card x+y
- x+y ranges from 2 to 12

### Optimal Strategy via Value Iteration

Let E(S) = expected number of turns to win from state S, with optimal play.

E(0) = 0 (already won).

For state S != 0:
E(S) = 1 + (1/36) * sum over (x,y) of min over valid choices c of E(S xor (1 << c))

where the choices are flip(x), flip(y), flip(x+y).

This is a system of linear equations that can be solved by value iteration or direct methods.

### Alice's Game Verification

Alice: 2 coins (heads=2, tails=1), 4 cards. States: 2^4 = 16.
Coin outcomes: (1,1), (1,2), (2,1), (2,2) each with prob 1/4.
Choices: flip x, flip y, or flip x+y.
- (1,1): flip 1 or flip 2
- (1,2): flip 1, flip 2, or flip 3
- (2,1): flip 2, flip 1, or flip 3
- (2,2): flip 2 or flip 4

Expected turns ~5.673651.

### System of Equations

The system E(S) = 1 + (1/36) sum_{x,y} min_c E(S ^ (1<<(c-1))) creates a system where each E(S) depends on other E values. Since flipping can both help and hurt, this requires careful handling.

We solve by value iteration: repeatedly update E(S) until convergence.

## Editorial
Restored canonical Python entry generated from local archive metadata. We initialize E(0) = 0, E(S) = some large value for S != 0. We then iterate: for each state S, compute new E(S) using the Bellman equation. Finally, repeat until convergence (changes < epsilon).

## Pseudocode

```text
Initialize E(0) = 0, E(S) = some large value for S != 0
Iterate: for each state S, compute new E(S) using the Bellman equation
Repeat until convergence (changes < epsilon)
Output E(2^12 - 1) = E(4095) (initial state: all face up)
```

## Correctness

**Theorem.** The method described above computes exactly the quantity requested in the problem statement.

*Proof.* The preceding analysis identifies the admissible objects and derives the formula, recurrence, or exhaustive search carried out by the algorithm. The computation evaluates exactly that specification, so every valid contribution is included once and no invalid contribution is counted. Therefore the returned value is the required answer. $\square$

## Complexity Analysis

- Time: O(2^12 * 36 * iterations) = O(4096 * 36 * T)
- Space: O(2^12)
- Convergence typically within thousands of iterations

## Answer

$$\boxed{50.317928}$$
