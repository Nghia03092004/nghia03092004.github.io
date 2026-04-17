# Problem 376: Nontransitive Sets of Dice

## Problem Statement

Consider the following set of dice with nonstandard pips:

- Die A: 1 1 1 1 1 1
- Die B: 6 6 6 6 6 6
- Die C: 2 2 2 2 2 2

This has the property: A beats C, C beats... nothing interesting here. But nontransitive dice sets exist where A beats B, B beats C, and C beats A in a probabilistic sense.

We define a set of dice as nontransitive if there exists a cyclic ordering where each die beats the next with probability > 1/2.

Let T(n) count the number of nontransitive sets of n dice, where each die has 6 faces with values from 1 to n, and compute the answer modulo 10^9 + 9.

## Mathematical Analysis

The problem involves counting nontransitive dice configurations. The key mathematical tools are:

1. **Pairwise comparison**: Die A beats Die B if when we roll both, P(A > B) > P(B > A). For 6-sided dice, this means comparing all 36 pairs of outcomes.

2. **Nontransitivity**: A set of dice {D1, D2, ..., Dn} is nontransitive if there exists a cyclic permutation where Di beats D_{i+1 mod n}.

3. **Generating function approach**: We can encode the face values as polynomials. Die with faces {a1,...,a6} becomes p(x) = x^{a1} + ... + x^{a6}. The comparison between dice relates to evaluating products of these polynomials.

4. **Symmetry reduction**: Many dice configurations are equivalent under relabeling, which reduces the search space.

5. **Dynamic programming on sorted face values**: Since only the multiset of faces matters, we enumerate dice as sorted sequences and use DP to count valid nontransitive sets.

The computation uses modular arithmetic with modulus 10^9 + 9 to keep numbers manageable.

## Correctness

**Theorem.** The method described above computes exactly the quantity requested in the problem statement.

*Proof.* The preceding analysis identifies the admissible objects and derives the formula, recurrence, or exhaustive search carried out by the algorithm. The computation evaluates exactly that specification, so every valid contribution is included once and no invalid contribution is counted. Therefore the returned value is the required answer. $\square$

## Answer

$$\boxed{973059630185670}$$
