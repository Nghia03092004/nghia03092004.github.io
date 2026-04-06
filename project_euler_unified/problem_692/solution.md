# Problem 692: Siegbert and Jo

## Problem Statement

Siegbert and Jo play a Fibonacci Nim variant. Define:
- $H(N)$ = the smallest Fibonacci number in the Zeckendorf representation of $N$.
- $G(n) = \sum_{k=1}^{n} H(k)$.

Given that $G(10) = 22$ and $G(1000) = 396673$, find $G(23416728348467685)$.

## Mathematical Foundation

**Definition (Zeckendorf Representation).** Every positive integer $n$ can be uniquely written as $n = \sum_{i \in I} F_i$ where $\{F_i\}$ are Fibonacci numbers (with $F_0 = 1, F_1 = 2, F_2 = 3, F_3 = 5, \ldots$) and no two consecutive indices appear in $I$. This is the *Zeckendorf representation*.

**Lemma 1 (Subset Counting).** Let $A(i)$ be the number of non-empty subsets of non-consecutive elements from $\{F_0, F_1, \ldots, F_i\}$, and let $S(i)$ be the sum of the minimum elements over all such subsets. Then:
$$A(0) = 1, \quad A(1) = 2, \quad A(i) = A(i-1) + 1 + A(i-2) \text{ for } i \geq 2,$$
$$S(0) = 1, \quad S(1) = 3, \quad S(i) = S(i-1) + F_i + S(i-2) \text{ for } i \geq 2.$$

**Proof.** For $A(i)$: subsets not containing $F_i$ number $A(i-1)$; subsets containing $F_i$ alone contribute $1$; subsets containing $F_i$ together with elements from $\{F_0, \ldots, F_{i-2}\}$ (skipping $F_{i-1}$ for non-consecutivity) number $A(i-2)$.

For $S(i)$: subsets not containing $F_i$ contribute $S(i-1)$ to the sum of minima. The singleton $\{F_i\}$ contributes $F_i$. For subsets containing $F_i$ plus elements from $\{F_0, \ldots, F_{i-2}\}$, the minimum is always from the lower-index elements (since $F_i > F_j$ for all $j < i$), contributing $S(i-2)$. $\square$

**Theorem (Digit DP on Zeckendorf).** Let the Zeckendorf representation of $n$ be $n = F_{z_0} + F_{z_1} + \cdots + F_{z_{m-1}}$ with $z_0 > z_1 > \cdots > z_{m-1}$ and $z_j \geq z_{j+1} + 2$. Then:
$$G(n) = F_{z_{m-1}} + \sum_{j=0}^{m-1} C(j),$$
where:
$$C(0) = \begin{cases} S(z_0 - 1) & \text{if } z_0 > 0, \\ 0 & \text{if } z_0 = 0, \end{cases}$$
$$C(j) = F_{z_{j-1}} + \begin{cases} S(z_j - 1) & \text{if } z_j > 0, \\ 0 & \text{if } z_j = 0, \end{cases} \quad \text{for } j \geq 1.$$

**Proof.** We decompose numbers in $[1, n]$ by where their Zeckendorf representation first deviates below that of $n$.

The term $F_{z_{m-1}}$ accounts for $H(n)$ itself (the minimum component of $n$'s own representation).

For each $j \in \{0, \ldots, m-1\}$, we consider numbers that match $n$'s representation at positions $z_0, \ldots, z_{j-1}$ (all present) but have $0$ at position $z_j$, with any valid non-consecutive subset from $\{F_0, \ldots, F_{z_j - 1}\}$ below.

**Case $j = 0$:** No committed bits above. The numbers are exactly the positive integers representable by non-consecutive Fibonacci numbers from $\{F_0, \ldots, F_{z_0-1}\}$. Their $H$-values sum to $S(z_0 - 1)$.

**Case $j \geq 1$:** The committed bits are $\{z_0, \ldots, z_{j-1}\}$ with smallest committed Fibonacci number $F_{z_{j-1}}$.
- If the subset below is empty: the number is $\sum_{l=0}^{j-1} F_{z_l}$, and $H = F_{z_{j-1}}$.
- If the subset below is non-empty: its minimum element is strictly less than $F_{z_{j-1}}$ (since all indices $\leq z_j - 1 < z_{j-1}$), so $H = \min(\text{subset})$. The sum of these minima is $S(z_j - 1)$.

Non-adjacency between committed and free positions is guaranteed since Zeckendorf gives $z_{j-1} \geq z_j + 2$, so the highest free index $z_j - 1 \leq z_{j-1} - 3$. $\square$

**Verification.** $G(1)$: $n=1$, Zeckendorf $=\{0\}$. $G(1) = F_0 + C(0) = 1 + 0 = 1$. Correct ($H(1) = 1$).

$G(5)$: $n=5$, Zeckendorf $= \{3\}$. $G(5) = F_3 + S(2) = 5 + 7 = 12$. Indeed $H(1)+H(2)+H(3)+H(4)+H(5) = 1+2+3+1+5 = 12$. $\checkmark$

## Algorithm

```
function G(n):
    // Precompute Fibonacci numbers up to n
    fib = [1, 2]; while fib[-1] + fib[-2] <= n: fib.append(fib[-1] + fib[-2])
    sz = len(fib)

    // Precompute A[i], S[i]
    A[0] = 1; S[0] = 1
    A[1] = 2; S[1] = 3
    for i = 2 to sz-1:
        A[i] = A[i-1] + 1 + A[i-2]
        S[i] = S[i-1] + fib[i] + S[i-2]

    // Zeckendorf representation of n (greedy, largest first)
    z = []
    temp = n
    for i = sz-1 downto 0:
        if fib[i] <= temp: z.append(i); temp -= fib[i]
    m = len(z)

    // Compute G(n)
    ans = fib[z[m-1]]
    for j = 0 to m-1:
        if j == 0:
            if z[0] > 0: ans += S[z[0] - 1]
        else:
            ans += fib[z[j-1]]
            if z[j] > 0: ans += S[z[j] - 1]
    return ans
```

## Complexity Analysis

- **Time:** $O(\log_\varphi n)$ for computing the Zeckendorf representation and the precomputation of $A$, $S$ arrays, since the number of Fibonacci numbers up to $n$ is $O(\log_\varphi n)$.
- **Space:** $O(\log_\varphi n)$ for storing the Fibonacci sequence and auxiliary arrays.

For $n = 23416728348467685 \approx 2.3 \times 10^{16}$, we need roughly $80$ Fibonacci numbers, so the computation is essentially instantaneous. Note that intermediate sums in $S(i)$ may exceed 64-bit integers and require 128-bit arithmetic.

## Answer

$$\boxed{842043391019219959}$$
