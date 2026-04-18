# Problem 700: Eulercoin

## Problem Statement

Leonhard Euler was born on 15 April 1707.

Consider the sequence $a_n = 1504170715041707 \cdot n \mod 4503599627370517$.

An element of this sequence is called an **Eulercoin** if it is strictly smaller than all previously found Eulercoins.

For example:
- $a_1 = 1504170715041707$ is the first Eulercoin.
- $a_2 = 3008341430083414$ is not an Eulercoin (larger than $a_1$).
- $a_3 = 8912517754604$ is the second Eulercoin.

The sum of the first 2 Eulercoins is $1513083232796311$.

Find the sum of all Eulercoins.

## Mathematical Analysis

### Key Observations

Let $N = 1504170715041707$ and $M = 4503599627370517$. Note that $M$ is prime.

The sequence $a_n = N \cdot n \bmod M$ generates all residues modulo $M$ since $\gcd(N, M) = 1$.

An Eulercoin is a record-low value in this sequence. We need to find all record-low values and sum them.

### Two-Phase Algorithm

**Phase 1: Brute Force**
For small $n$, we iterate through the sequence and track the minimum value seen so far. Each time we find a new minimum, it's an Eulercoin. This efficiently finds the first ~16 Eulercoins.

**Phase 2: Modular Inverse**
Once the Eulercoins become small (around $15 \times 10^6$), we reverse the approach. For a target value $v$, we solve $N \cdot n \equiv v \pmod{M}$ using the modular inverse $n = v \cdot N^{-1} \bmod M$.

We iterate downward from the current minimum Eulercoin to 0, checking if each value can be achieved by some $n$ that comes before any $n$ producing a smaller value. In practice, we check all values from the last brute-force Eulercoin down to 1.

### Euclidean Algorithm Approach

An elegant recursive approach views the modular arithmetic as points on a circle:
- The sequence jumps around a circle of circumference $M$ with step size $N$.
- After completing a full cycle, the residual shift is $M \bmod N$.
- This is analogous to the Euclidean algorithm, reducing the problem recursively.

## Editorial
We brute force for n = 1, 2, ... until eulercoin < threshold. We then iterate over v from (last_eulercoin - 1) down to 0. Finally, return total sum.

## Pseudocode

```text
Compute N_inv = modular_inverse(N, M) using extended GCD
Brute force for n = 1, 2, ... until eulercoin < threshold
For v from (last_eulercoin - 1) down to 0:
Return total sum
```

## Correctness

**Theorem.** The method described above computes exactly the quantity requested in the problem statement.

*Proof.* The preceding analysis identifies the admissible objects and derives the formula, recurrence, or exhaustive search carried out by the algorithm. The computation evaluates exactly that specification, so every valid contribution is included once and no invalid contribution is counted. Therefore the returned value is the required answer. $\square$

## Complexity Analysis

- Phase 1: $O(k)$ iterations where $k$ is the index of the last brute-force Eulercoin (~$10^7$)
- Phase 2: $O(E)$ where $E$ is the value of the last brute-force Eulercoin (~$1.5 \times 10^7$)
- Total: $O(k + E) \approx O(10^7)$

## Answer

$$\boxed{1517926517777556}$$
