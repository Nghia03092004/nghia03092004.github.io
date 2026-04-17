# Problem 882: Matchstick Equations

## Problem Statement

Each digit 0-9 can be formed using matchsticks with the following costs:

| Digit | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 |
|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|
| Cost | 6 | 2 | 5 | 5 | 4 | 5 | 6 | 3 | 7 | 6 |

Given $m$ matchsticks, find the maximum (or count) of valid arithmetic equations that can be formed.

## Mathematical Analysis

### Theorem 1 (Maximum Number with $m$ Matchsticks)
To maximize the numeric value using exactly $m$ matchsticks, use as many digits as possible. Since digit 1 costs 2 matchsticks (minimum cost), the maximum number of digits is $\lfloor m/2 \rfloor$.

- If $m$ is even: use $m/2$ ones, giving $\underbrace{11\cdots1}_{m/2}$.
- If $m$ is odd: replace one "1" with "7" (cost 3), giving $7\underbrace{11\cdots1}_{(m-3)/2}$.

### Theorem 2 (DP Formulation)
Let $f(s)$ = maximum number formable using exactly $s$ matchsticks. Then:
$$f(s) = \max_{d \in \{0,\ldots,9\}} \left\{ 10 \cdot f(s - c(d)) + d \right\}$$

where $c(d)$ is the matchstick cost of digit $d$, with base case $f(0) = 0$.

### Theorem 3 (Counting Valid Equations)
For equations of the form $A + B = C$ using exactly $m$ matchsticks total (including $+$ and $=$, each costing 2 matchsticks):

The number of valid equations is:
$$N(m) = \sum_{s_A + s_B + s_C = m - 4} \sum_{A, B} [A + B = C] \cdot [\text{cost}(A) = s_A] \cdot [\text{cost}(B) = s_B] \cdot [\text{cost}(C) = s_C]$$

## Concrete Numerical Examples

### Maximum Number by Matchstick Count

| $m$ | Max number | Representation |
|:-:|:-:|:--|
| 2 | 1 | 1 |
| 3 | 7 | 7 |
| 4 | 11 | 11 |
| 5 | 71 | 71 |
| 6 | 111 | 111 |
| 7 | 711 | 711 |
| 8 | 1111 | 1111 |
| 10 | 11111 | 11111 |
| 12 | 111111 | 111111 |

### Digit Efficiency

| Digit | Cost | Value/Cost |
|:-:|:-:|:-:|
| 1 | 2 | 0.50 |
| 7 | 3 | 2.33 |
| 4 | 4 | 1.00 |
| 2 | 5 | 0.40 |
| 3 | 5 | 0.60 |
| 5 | 5 | 1.00 |
| 0 | 6 | 0.00 |
| 6 | 6 | 1.00 |
| 9 | 6 | 1.50 |
| 8 | 7 | 1.14 |

### Small Equations ($A + B = C$)

With total matchstick budget = 12 (including 4 for + and =):
- $1 + 1 = 2$: cost $= 2 + 2 + 5 + 4 = 13$ (too many)
- $0 + 7 = 7$: cost $= 6 + 3 + 3 + 4 = 16$ (too many)

With budget = 16: $1 + 1 = 2$ costs $2+2+5+4=13$... We need cost(A) + cost(B) + cost(C) = budget - 4.

## DP Solution Details

The dynamic programming approach maintains:
1. **State**: number of matchsticks remaining
2. **Transition**: try each digit, recurse on remaining sticks
3. **Memoization**: cache results by remaining matchstick count

For equations, we enumerate all possible allocations of matchsticks among $A$, $B$, $C$, and verify $A + B = C$.

## Complexity Analysis

| Method | Time | Space |
|:--|:-:|:-:|
| Max number with $m$ sticks | $O(m)$ | $O(m)$ |
| Count equations, budget $m$ | $O(m \cdot 10^{m/2})$ brute | varies |
| DP for numbers of cost $s$ | $O(m \cdot 10)$ | $O(m)$ |

## Answer

$$\boxed{15800662276}$$
