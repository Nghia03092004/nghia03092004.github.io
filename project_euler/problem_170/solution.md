# Problem 170: Find the Largest 0 to 9 Pandigital That Can Be Formed by Concatenating Products

## Problem Statement

Take the number 6 and multiply it by each of 1273 and 9854:
- $6 \times 1273 = 7638$
- $6 \times 9854 = 59124$

By concatenating these products we get the 1 to 9 pandigital 763859124. Notice that the concatenation of the input numbers $6 \| 1273 \| 9854 = 612739854$ is also 1 to 9 pandigital.

Find the largest 0 to 9 pandigital 10-digit number that can be formed by concatenating the products of an integer $n$ with two or more other integers, such that the concatenation of all input numbers ($n$ and the multipliers) is also a 0 to 9 pandigital 10-digit number.

## Mathematical Development

**Definition.** A *0-to-9 pandigital* is a 10-digit string using each of the digits $\{0, 1, 2, \ldots, 9\}$ exactly once.

**Theorem 1 (Digit conservation).** *If $n \| c_1 \| c_2 \| \cdots \| c_m$ is a 10-digit 0-9 pandigital and $n \cdot c_1 \| n \cdot c_2 \| \cdots \| n \cdot c_m$ is also a 10-digit 0-9 pandigital, then the total number of digits in $\{n, c_1, \ldots, c_m\}$ is 10 and the total number of digits in $\{n \cdot c_1, \ldots, n \cdot c_m\}$ is also 10.*

**Proof.** A 0-9 pandigital has exactly 10 digits. The concatenation of the inputs uses each digit exactly once across all the input numbers, so the total digit count is 10. Similarly for the products. $\square$

**Theorem 2 (Digit-set disjointness).** *The digit sets of $n, c_1, c_2, \ldots, c_m$ must be pairwise disjoint and their union must be $\{0, 1, \ldots, 9\}$. Likewise for $n \cdot c_1, n \cdot c_2, \ldots, n \cdot c_m$.*

**Proof.** Since the concatenation is a pandigital (each digit exactly once), no digit can appear in more than one of the input numbers, and every digit must appear in some input number. The same argument applies to the products. $\square$

**Lemma 1 (Multiplier count).** *We require $m \geq 2$, i.e., at least two multipliers.*

**Proof.** This is given in the problem statement. $\square$

**Lemma 2 (Bitmask DP for optimal concatenation).** *Given a set of valid $(c_i, p_i)$ pairs where $p_i = n \cdot c_i$, with associated digit masks, the problem of selecting a subset of size $\geq 2$ whose input masks partition the available digits and whose output masks partition $\{0, \ldots, 9\}$ can be solved via bitmask DP over $2^{10} = 1024$ states.*

**Proof.** Define $\text{dp}[\text{in\_mask}][\text{out\_mask}]$ = maximum concatenated product achievable using multipliers whose combined input digit mask is $\text{in\_mask}$ and combined output digit mask is $\text{out\_mask}$. Transitions add one $(c_i, p_i)$ pair at a time, requiring the new masks to be disjoint from the accumulated masks. The state space is at most $1024 \times 1024 = 2^{20} \approx 10^6$, and the answer is $\text{dp}[\text{all\_remaining}][2^{10}-1]$ maximized over valid $n$. When combining groups, both concatenation orders are tried to select the larger result. $\square$

**Theorem 3 (Search strategy).** *To find the largest pandigital result, we search $n$ starting from values that can produce large leading digits (e.g., $n$ such that $n \cdot c_1$ starts with 9). The search terminates early once a candidate is found that cannot be exceeded.*

**Proof.** The output is a 10-digit pandigital, so the largest possible starts with 9876543... . For a fixed $n$, the largest product is obtained by choosing the largest valid multiplier. By searching $n$ values in an order that prioritizes large leading output digits and pruning branches where the partial result cannot exceed the current best, we guarantee finding the global maximum. $\square$

## Editorial
The search is organized around a candidate base value $n$. Once $n$ is fixed, its digits cannot be reused by any multiplier, so the remaining digits define a pool from which every multiplier must be formed. The implementation enumerates all such multipliers, computes their products, and keeps only the pairs whose input digits and product digits are each internally non-repeating.

After that, the task becomes a mask-combination problem. Every valid multiplier/product pair contributes one mask on the input side and one mask on the output side. A dynamic program then combines disjoint pairs, always keeping the numerically largest concatenated product obtainable for each pair of accumulated masks. Whenever the input masks cover exactly the digits not used by $n$ and the output masks cover all ten digits, and at least two multipliers have been used, the resulting concatenation is a valid candidate for the global maximum.

## Pseudocode

```text
Initialize the global best answer to zero.

For each candidate base value `n` in the search range:
    Compute the digit mask of `n`.
    Let the remaining digits be the only digits available for multipliers.
    Enumerate every multiplier built from those remaining digits.

    For each multiplier `c`:
        compute the product `p = n * c`.
        Keep the pair only if the digits of `p` are all distinct.
        Record the input mask of `c`, the output mask of `p`,
        and the product value itself.

    Compress duplicate `(input mask, output mask)` pairs by keeping only the largest product.

    Run a bitmask DP over disjoint unions of these pairs.
    When combining states, try both concatenation orders and keep the larger numeric result.

    If a DP state uses at least two multipliers, covers all remaining input digits,
    and covers all ten output digits, compare its value with the global best.

Return the largest value found.
```

## Complexity Analysis

- **Time:** For each $n$, generating multipliers is $O(k!)$ where $k = 10 - \text{digits}(n)$ (permutations of remaining digits). The bitmask DP has $O(2^{20})$ states. With pruning by early termination and ordering $n$ values to find large results first, the practical runtime is a few seconds.
- **Space:** $O(2^{20})$ for the DP table.

## Answer

$$\boxed{9857164023}$$
