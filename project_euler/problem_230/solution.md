# Problem 230: Fibonacci Words

## Problem Statement

For any two strings of digits $A$ and $B$, define the Fibonacci word sequence:

- $F(1) = A$
- $F(2) = B$
- $F(n) = F(n-2) \cdot F(n-1)$ for $n > 2$ (concatenation)

Let $D(k)$ denote the $k$-th digit of the first term in the sequence that has at least $k$ digits.

Given $A$ and $B$ as 100-digit strings from $\pi$, compute

$$\sum_{n=0}^{17} 10^n \cdot D\bigl((127 + 19n) \cdot 7^n\bigr).$$

## Mathematical Foundation

**Theorem 1 (Length Recurrence).** *The lengths $L(n) = |F(n)|$ satisfy $L(1) = L(2) = 100$ and $L(n) = L(n-1) + L(n-2)$ for $n \geq 3$.*

**Proof.** $F(n) = F(n-2) \cdot F(n-1)$ is the concatenation of $F(n-2)$ and $F(n-1)$, so $|F(n)| = |F(n-2)| + |F(n-1)|$. $\square$

**Theorem 2 (Exponential Growth).** *$L(n) = \Theta(\phi^n)$ where $\phi = (1 + \sqrt{5})/2$ is the golden ratio. Specifically,*

$$L(n) = \frac{100}{\phi + 1}\bigl(\phi^{n-1} + (-\phi)^{-(n-1)}\bigr) + \frac{100}{\phi + 1}\bigl(\phi^{n-2} + (-\phi)^{-(n-2)}\bigr).$$

**Proof.** The recurrence $L(n) = L(n-1) + L(n-2)$ with $L(1) = L(2) = 100$ has characteristic equation $x^2 = x + 1$ with roots $\phi$ and $-1/\phi$. The closed form follows from solving the initial conditions. Asymptotically, the $(-1/\phi)^n$ terms vanish, giving $L(n) = \Theta(\phi^n)$. $\square$

**Lemma 1 (Sufficient Depth).** *The largest position queried is $k_{17} = (127 + 19 \cdot 17) \cdot 7^{17} = 450 \cdot 7^{17} \approx 1.05 \times 10^{17}$. We need $L(n) \geq k_{17}$, which is satisfied for $n \approx 85$.*

**Proof.** $L(n) \approx 100 \cdot \phi^{n-2} / (\phi + 1)$. Solving $L(n) \geq 10^{17}$ gives $n \geq \lceil 2 + \log_\phi(10^{17} \cdot (\phi+1)/100) \rceil \approx 85$. $\square$

**Theorem 3 (Recursive Digit Lookup).** *To find the $k$-th digit of $F(n)$ where $n$ is the smallest index with $L(n) \geq k$:*

- *If $n = 1$: return $A[k]$.*
- *If $n = 2$: return $B[k]$.*
- *If $k \leq L(n-2)$: the digit is in $F(n-2)$; recurse with $(n-2, k)$.*
- *If $k > L(n-2)$: the digit is in $F(n-1)$; recurse with $(n-1, k - L(n-2))$.*

**Proof.** Since $F(n) = F(n-2) \cdot F(n-1)$, the first $L(n-2)$ characters are from $F(n-2)$ and the remaining $L(n-1)$ characters are from $F(n-1)$. The recursion correctly decomposes the position. Termination is guaranteed since at each step we reduce $n$ by at least 1 (and $k$ stays bounded by $L(n)$, which decreases exponentially). $\square$

**Lemma 2 (Lookup Complexity).** *Each digit lookup performs $O(n) = O(\log_\phi k)$ recursive steps.*

**Proof.** At each step, $n$ decreases by 1 or 2. Since we start at $n \approx \log_\phi k$ and reach $n \in \{1, 2\}$, the number of steps is $O(\log_\phi k)$. $\square$

## Editorial
F(1) = A (100 digits of pi), F(2) = B (next 100 digits of pi) F(n) = F(n-2) . F(n-1) (concatenation, older part first) Sequence: A, B, AB, BAB, ABBAB, ... Find sum of D((127+19n)*7^n) * 10^n for n = 0 to 17, where D(k) is the k-th digit of the infinite Fibonacci word. We precompute Fibonacci lengths. We then find smallest n with L[n] >= k. Finally, recurse.

## Pseudocode

```text
Precompute Fibonacci lengths
Find smallest n with L[n] >= k
Recurse
else
```

## Complexity Analysis

- **Time:** $O(18 \cdot \log_\phi k_{\max}) = O(18 \times 85) = O(1530)$. Essentially instantaneous.
- **Space:** $O(\log_\phi k_{\max}) = O(85)$ for the precomputed lengths.

## Answer

$$\boxed{850481152593119296}$$
