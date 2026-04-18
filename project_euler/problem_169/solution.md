# Problem 169: Exploring the Number of Different Ways a Number Can Be Expressed as a Sum of Powers of 2

## Problem Statement

Define $f(n)$ as the number of ways to express $n$ as a sum of powers of 2, where each power of 2 is used at most twice. Find $f(10^{25})$.

## Mathematical Foundation

**Definition.** $f(n) = |\{(c_0, c_1, c_2, \ldots) : c_i \in \{0, 1, 2\}, \, \sum_{i \geq 0} c_i \cdot 2^i = n\}|$.

**Theorem 1 (Recurrence for $f$).** *For all $n \geq 0$:*
$$f(n) = \begin{cases} 1 & \text{if } n = 0, \\ f\!\left(\lfloor n/2 \rfloor\right) & \text{if $n$ is odd,} \\ f\!\left(n/2\right) + f\!\left(n/2 - 1\right) & \text{if $n$ is even and $n \geq 2$.} \end{cases}$$

**Proof.** Consider the coefficient $c_0$ (the number of times $2^0 = 1$ is used).

*Case $n$ odd:* We need $c_0$ odd, so $c_0 = 1$. Then $\sum_{i \geq 1} c_i \cdot 2^i = n - 1$. Dividing by 2: $\sum_{i \geq 1} c_i \cdot 2^{i-1} = (n-1)/2 = \lfloor n/2 \rfloor$. Relabeling $c_i' = c_{i+1}$ gives a representation of $\lfloor n/2 \rfloor$ with each $c_i' \in \{0, 1, 2\}$. This is a bijection, so $f(n) = f(\lfloor n/2 \rfloor)$.

*Case $n$ even:* We need $c_0$ even, so $c_0 \in \{0, 2\}$.
- If $c_0 = 0$: the remaining sum is $n$, so $\sum_{i \geq 1} c_i \cdot 2^i = n$, giving $\sum_{i \geq 0} c_{i+1} \cdot 2^i = n/2$. This contributes $f(n/2)$ representations.
- If $c_0 = 2$: the remaining sum is $n - 2$, so $\sum_{i \geq 1} c_i \cdot 2^i = n - 2$, giving $\sum_{i \geq 0} c_{i+1} \cdot 2^i = (n-2)/2 = n/2 - 1$. This contributes $f(n/2 - 1)$ representations (valid since $n \geq 2$ ensures $n/2 - 1 \geq 0$).

Total: $f(n) = f(n/2) + f(n/2 - 1)$. $\square$

**Theorem 2 (Subproblem count).** *The memoized computation of $f(n)$ encounters at most $O(\log^2 n)$ distinct subproblems.*

**Proof.** At recursion depth $d$, the argument has the form $\lfloor n / 2^d \rfloor - j$ for some integer $j$ with $0 \leq j \leq d$. This is because each "even" branch produces two subproblems at argument $\lfloor n/2^{d+1} \rfloor \cdot \text{(something)} - j'$, and the offset $j$ increases by at most 1 per even step. At depth $d$, there are at most $d + 1$ possible values of $j$, giving at most $\sum_{d=0}^{\lfloor \log_2 n \rfloor} (d + 1) = O(\log^2 n)$ distinct subproblems. $\square$

**Lemma 1 (Base cases).** *$f(0) = 1$ (the empty sum) and $f(1) = 1$ (only $c_0 = 1$, all others 0).*

**Proof.** $f(0)$: the unique representation is $c_i = 0$ for all $i$. $f(1)$: $c_0 = 1$ is forced (only way to get an odd sum); then the remaining sum is 0, which has a unique representation. $\square$

**Lemma 2 (Correctness for $n = 10^{25}$).** *The recursion terminates since each step replaces $n$ by $\lfloor n/2 \rfloor$ or $\lfloor n/2 \rfloor - 1$, strictly decreasing $n$ until reaching the base cases. The recursion depth is $\lceil \log_2(10^{25}) \rceil = 84$.*

**Proof.** $\log_2(10^{25}) = 25 \log_2 10 \approx 25 \times 3.3219 = 83.05$, so the recursion depth is 84. $\square$

## Editorial
each power used at most twice. Find f(10^25). Recurrence: f(0) = 1 f(n) = f(n//2) if n is odd f(n) = f(n//2) + f(n//2-1) if n is even. We else.

## Pseudocode

```text
if n is odd
else
```

## Complexity Analysis

- **Time:** $O(\log^2 n)$ subproblems, each requiring $O(1)$ work (plus big-integer arithmetic for the values, which are $O(\log n)$ bits). Total: $O(\log^2 n \cdot M(\log n))$ where $M$ is multiplication cost, but since the values of $f$ are polynomially bounded in $n$, $O(\log^3 n)$ suffices.
- **Space:** $O(\log^2 n)$ for the memoization table.

For $n = 10^{25}$: depth $\approx 84$, subproblems $\leq 84^2 / 2 \approx 3{,}528$.

## Answer

$$\boxed{178653872807}$$
