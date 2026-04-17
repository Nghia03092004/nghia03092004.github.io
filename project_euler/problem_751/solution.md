# Problem 751: Concatenation Coincidence

## Problem Statement

A non-decreasing sequence of positive integers $a_1, a_2, a_3, \ldots$ is derived from a real number $\theta$ by the recurrence:

$$b_1 = \theta, \qquad b_n = \lfloor b_{n-1} \rfloor \cdot (b_{n-1} - \lfloor b_{n-1} \rfloor + 1) \quad \text{for } n \geq 2, \qquad a_n = \lfloor b_n \rfloor.$$

The concatenation $\tau(\theta)$ is the real number formed by placing $a_1$ before the decimal point and concatenating the decimal digits of $a_2, a_3, \ldots$ after it. For example, $\theta = 2.956938\ldots$ yields the sequence $2, 3, 5, 8, 13, 21, 34, 55, 89, \ldots$ and $\tau = 2.3581321345589\ldots$

Find the unique $\theta$ with $a_1 = 2$ and $\tau(\theta) = \theta$, rounded to 24 decimal places.

## Mathematical Foundation

**Definition.** Let $C: [2,3) \to \mathbb{R}$ be the *concatenation map* that sends $\theta$ to $\tau(\theta)$ as defined above.

**Theorem 1 (Existence and uniqueness of the fixed point).** There exists a unique $\theta^* \in [2,3)$ such that $C(\theta^*) = \theta^*$.

**Proof.** We show $C$ is a contraction in the metric $d(\theta, \theta') = 10^{-m}$ where $m$ is the index of the first decimal digit at which $\tau(\theta)$ and $\tau(\theta')$ differ. Suppose $\theta$ and $\theta'$ agree in their first $k$ decimal digits. Then $b_1 = \theta$ and $b_1' = \theta'$ agree to $k$ digits past the decimal. Since $a_1 = \lfloor b_1 \rfloor = 2$ for both, and $b_2 = a_1 \cdot (b_1 - a_1 + 1)$ depends continuously on $b_1$, the sequences $(a_n)$ and $(a_n')$ agree for all indices $n$ whose terms are determined by the first $k$ digits. The concatenation $\tau$ therefore reproduces at least those $k$ digits and typically more, because each subsequent term $a_n$ contributes $\lfloor \log_{10} a_n \rfloor + 1 \geq 1$ additional digits. Hence $d(C(\theta), C(\theta')) < d(\theta, \theta')$, so $C$ is a contraction on the complete metric space $([2,3), d)$. By the Banach fixed-point theorem, a unique fixed point exists. $\square$

**Lemma 1 (Rapid convergence).** For any $\theta_0 \in [2,3)$, the iteration $\theta_{n+1} = C(\theta_n)$ converges to $\theta^*$. In practice, 2--3 iterations with 30-digit precision suffice for 24 correct decimal places.

**Proof.** This follows directly from the contraction property established in Theorem 1. The convergence rate is superlinear because each iteration at least doubles the number of correct decimal digits (the concatenation preserves all digits that were already correct and extends them). $\square$

## Algorithm

```
function FIND_FIXED_POINT(digits_needed):
    set decimal precision to digits_needed + 10
    theta = 2.0
    for iteration = 1 to 5:
        b = theta
        sequence = []
        total_digits = 0
        while total_digits < digits_needed + 10:
            a = floor(b)
            sequence.append(a)
            total_digits += number_of_digits(a)
            fractional = b - a
            b = a * (fractional + 1)
        tau = concatenate(sequence[0], ".", sequence[1], sequence[2], ...)
        theta = tau
    return round(theta, digits_needed)
```

## Complexity Analysis

- **Time:** $O(I \cdot D)$ where $I$ is the number of fixed-point iterations (typically $I \leq 5$) and $D$ is the number of digits of precision. Each iteration generates $O(D)$ sequence terms using $O(1)$ arbitrary-precision operations per term.
- **Space:** $O(D)$ to store the high-precision number and the concatenated string.

## Answer

$$\boxed{2.223561019313554106173177}$$
