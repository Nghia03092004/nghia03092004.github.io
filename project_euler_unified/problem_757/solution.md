# Problem 757: Stealthy Numbers

## Problem Statement

A positive integer $N$ is **stealthy** if there exist positive integers $a, b, c, d$ such that $a \cdot b = c \cdot d = N$ and $a + b = c + d + 1$.

For example, $36 = 4 \times 9 = 6 \times 6$ is stealthy since $4 + 9 = 13 = 6 + 6 + 1$.

There are 2851 stealthy numbers not exceeding $10^6$.

**Question:** How many stealthy numbers are there that don't exceed $10^{14}$?

## Mathematical Analysis

### Key Observation: Bipronic Numbers

A stealthy number $N$ can be written as $N = a \cdot b$ where $a + b = c + d + 1$ and $c \cdot d = N$.

If we write $a \cdot b = c \cdot d$ and $a + b = c + d + 1$, we can use the identity relating factorizations to sums. Consider two factorizations of $N$: $N = a \cdot b = c \cdot d$ with $a + b - (c + d) = 1$.

### Connection to Pronic Numbers

A pronic (oblong) number is a number of the form $n(n+1)$. The key insight is:

**Theorem:** A positive integer $N$ is stealthy if and only if $N$ can be expressed as $N = p \cdot q$ where both $p$ and $q$ are pronic numbers (i.e., $p = x(x+1)$ and $q = y(y+1)$ for positive integers $x, y$).

**Proof:** If $N = x(x+1) \cdot y(y+1)$, set:
- $a = x \cdot y(y+1)$, $b = (x+1)$... more carefully:
- $a = (x+1)(y+1)$, $b = xy$, so $ab = xy(x+1)(y+1) = N$
- $c = x(y+1)$, $d = y(x+1)$, so $cd = xy(x+1)(y+1) = N$
- $a + b = (x+1)(y+1) + xy = xy + x + y + 1 + xy = 2xy + x + y + 1$
- $c + d = x(y+1) + y(x+1) = xy + x + xy + y = 2xy + x + y$
- Therefore $a + b = c + d + 1$. QED.

### Algorithm

Enumerate all products $x(x+1) \cdot y(y+1) \leq 10^{14}$ where $x \leq y$, and count distinct values using a hash set or sorted merge approach.

For the upper bound on $x$: since $x(x+1) \cdot x(x+1) \leq 10^{14}$, we need $x(x+1) \leq 10^7$, giving $x \leq \approx 3162$.

For each $x$, $y$ ranges from $x$ to the largest value such that $x(x+1) \cdot y(y+1) \leq 10^{14}$.

## Complexity Analysis

- **Time:** $O(M \log M)$ where $M$ is the total number of products generated (around $10^7$), dominated by sorting/deduplication.
- **Space:** $O(M)$ to store all products.

## Answer

$$\boxed{75737353}$$
