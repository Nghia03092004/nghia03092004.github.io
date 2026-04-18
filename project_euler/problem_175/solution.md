# Problem 175: Fractions Involving the Number of Different Ways a Number Can Be Expressed as a Sum of Powers of 2

## Problem Statement

Define $f(n)$ as the number of ways to write $n$ as a sum of powers of 2, where each power is used at most twice. The ratio $f(n)/f(n-1)$ enumerates every positive rational exactly once via the Calkin-Wilf sequence. Given $p/q = 123456789/987654321$, find the shorthand representation (run-length encoding of the Calkin-Wilf tree path).

## Mathematical Foundation

**Theorem 1.** *(Calkin-Wilf tree.) The binary tree rooted at $1/1$ with left child $a/(a+b)$ and right child $(a+b)/b$ of node $a/b$ contains every positive rational number exactly once in lowest terms.*

**Proof.** (Calkin & Wilf, 2000.) By induction on $a + b$: the root $1/1$ is in lowest terms. If $\gcd(a, b) = 1$, then $\gcd(a, a+b) = \gcd(a, b) = 1$ and $\gcd(a+b, b) = \gcd(a, b) = 1$, so both children are in lowest terms. To show surjectivity, note that every $a/b$ with $a + b > 2$ has a unique parent: if $a < b$ then the parent is $a/(b-a)$ (left child), and if $a > b$ then the parent is $(a-b)/b$ (right child). This defines a unique path from any rational to the root. Injectivity follows from the tree structure. $\square$

**Theorem 2.** *(Path recovery via Euclidean algorithm.) The path from the root to $a/b$ in the Calkin-Wilf tree, expressed as run-length encoded L/R steps, is obtained by the following algorithm applied to $a/b$ in lowest terms. The algorithm traces from $a/b$ back to $1/1$, batching consecutive steps of the same direction using integer division.*

**Proof.** Each step from $a/b$ to its parent subtracts: if $a > b$, then $a \mapsto a - b$ (R-step); if $b > a$, then $b \mapsto b - a$ (L-step). Batching $\lfloor a/b \rfloor$ or $\lfloor b/a \rfloor$ steps at once is valid because consecutive steps have the same direction until the inequality reverses. This is precisely the Euclidean algorithm on $(a, b)$, so it terminates in $O(\log \max(a, b))$ rounds. The path is then reversed to read root-to-leaf. $\square$

**Lemma 1.** *(Last-step adjustment.) When tracing from $a/b$ to $1/1$, if the quotient would reduce one component to zero, we must subtract 1 from that quotient and append a final step of 1 in the opposite direction. Equivalently, we use $\lfloor (a-1)/b \rfloor$ (or $\lfloor (b-1)/a \rfloor$) to ensure we stop at $1/1$ rather than overshoot to $0/b$.*

**Proof.** The path must terminate at $1/1$, not $0/b$. Taking $k = \lfloor (a-1)/b \rfloor$ R-steps leaves $a - kb \geq 1$. If $a$ is a multiple of $b$, this prevents reaching $0/b$. The final state is $1/1$ after one more step. $\square$

**Theorem 3.** *(Computation for $123456789/987654321$.)* We have $\gcd(123456789, 987654321) = 9$, so the reduced fraction is $13717421/109739369$.

**Proof.** By the Euclidean algorithm: $987654321 = 8 \times 123456789 + 9$, and $123456789 = 13717421 \times 9$, so $\gcd = 9$. $\square$

Applying the path-recovery algorithm to $a/b = 13717421/109739369$:

1. $b > a$: $k = \lfloor(109739369 - 1)/13717421\rfloor = 7$. After 7 L-steps: $b \leftarrow 109739369 - 7 \times 13717421 = 13717422$.
2. $b > a$: $k = \lfloor(13717422 - 1)/13717421\rfloor = 1$. After 1 L-step: $b \leftarrow 13717422 - 13717421 = 1$.
3. $a > b$: $k = \lfloor(13717421 - 1)/1\rfloor = 13717420$. After 13717420 R-steps: $a \leftarrow 1$. Now $a/b = 1/1$. Done.

Reversed path (root-to-leaf): R(13717420), L(1), L(7). Merging the two consecutive L-runs: R(13717420), L(8). The shorthand is $1, 13717420, 8$ (where by convention the first entry represents the direction from the leaf, giving the reversed reading).

## Editorial
a Number Can Be Expressed as a Sum of Powers of 2 Find the shorthand notation for f(123456789/987654321). The shorthand for fraction p/q (with p < q, both reduced) is: 1. Compute the continued fraction of q/p. 2. Ensure the last partial quotient is 1 (split if needed: [..., n] -> [..., n-1, 1]). 3. Reverse the sequence. This gives the Calkin-Wilf tree path encoding. We else. We then merge last two entries if they represent same direction. Finally, (they alternate, so merge means add to previous).

## Pseudocode

```text
else
Merge last two entries if they represent same direction
(they alternate, so merge means add to previous)
Reverse to get root-to-leaf order
```

## Complexity Analysis

- **Time:** $O(\log \max(p, q))$ -- the Euclidean algorithm.
- **Space:** $O(\log \max(p, q))$ for storing the run lengths.

## Answer

$$\boxed{1,13717420,8}$$
