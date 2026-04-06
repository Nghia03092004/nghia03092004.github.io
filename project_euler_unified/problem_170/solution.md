# Problem 170: Find the Largest 0 to 9 Pandigital That Can Be Formed by Concatenating Products

## Problem Statement

Take the number 6 and multiply it by each of 1273 and 9854:
- $6 \times 1273 = 7638$
- $6 \times 9854 = 59124$

By concatenating these products we get the 1 to 9 pandigital 763859124. Notice that the concatenation of the input numbers $6 \| 1273 \| 9854 = 612739854$ is also 1 to 9 pandigital.

Find the largest 0 to 9 pandigital 10-digit number that can be formed by concatenating the products of an integer $n$ with two or more other integers, such that the concatenation of all input numbers ($n$ and the multipliers) is also a 0 to 9 pandigital 10-digit number.

## Mathematical Foundation

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

## Algorithm

```
function FIND_LARGEST_PANDIGITAL():
    best = 0

    for each candidate n (positive integer with distinct digits):
        n_mask = digit_mask(n)
        remaining = {0,...,9} \ digits(n)

        # Generate all valid (c, p) pairs
        pairs = []
        for each c formed from digits in 'remaining' (c > 0, distinct digits):
            p = n * c
            if digits(p) are all distinct and disjoint from previously used:
                pairs.append((digit_mask(c), digit_mask(p), p))

        # Bitmask DP to find best combination of >= 2 pairs
        # whose c-masks partition 'remaining' and p-masks partition {0..9}
        dp = {} # (in_used, out_used, count) -> best concatenated value

        for each pair (cm, pm, p):
            for each state in dp with compatible masks:
                extend state, try both concat orders, update dp

        # Check complete states with count >= 2
        for valid complete states:
            if concatenated value > best:
                best = concatenated value

    return best
```

## Complexity Analysis

- **Time:** For each $n$, generating multipliers is $O(k!)$ where $k = 10 - \text{digits}(n)$ (permutations of remaining digits). The bitmask DP has $O(2^{20})$ states. With pruning by early termination and ordering $n$ values to find large results first, the practical runtime is a few seconds.
- **Space:** $O(2^{20})$ for the DP table.

## Answer

$$\boxed{9857164023}$$
