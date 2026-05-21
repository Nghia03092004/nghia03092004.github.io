# Problem 230: Fibonacci Words

## Problem Statement

Let $A$ and $B$ be the two given $100$-digit strings from $\pi$, and define

$$
F(1)=A,\qquad F(2)=B,\qquad F(n)=F(n-2)\,F(n-1)\quad(n>2).
$$

For a positive integer $k$, let $D(k)$ be the $k$th digit of the first Fibonacci word long enough to contain that position.

Compute

$$
\sum_{n=0}^{17} 10^n \, D\bigl((127+19n)7^n\bigr).
$$

## Mathematical Development

Let

$$
L(n)=|F(n)|.
$$

Because $F(n)$ is the concatenation of $F(n-2)$ and $F(n-1)$,

$$
L(1)=L(2)=100,
\qquad
L(n)=L(n-2)+L(n-1).
$$

So the lengths follow the Fibonacci recurrence and grow exponentially. This means we can reach any queried position by precomputing only a short table of lengths.

Now suppose $n$ is the smallest index with $L(n)\ge k$. Since

$$
F(n)=F(n-2)\,F(n-1),
$$

the first $L(n-2)$ digits come from $F(n-2)$ and the remaining digits come from $F(n-1)$. Therefore

$$
D_n(k)=
\begin{cases}
D_{n-2}(k), & k \le L(n-2),\\[4pt]
D_{n-1}(k-L(n-2)), & k > L(n-2).
\end{cases}
$$

This reduces the query to a smaller Fibonacci word without ever constructing the actual string.

## Editorial

The strings themselves are a red herring. By the time the index reaches $(127+19n)7^n$ for $n=17$, the relevant Fibonacci word is astronomically long, so building it explicitly is impossible. But the only thing the concatenation rule preserves is length, and length is exactly the information needed to walk a position backwards through the recursion.

So the program does two tiny pieces of precomputation. First it builds the length table until the entries are safely larger than every requested index. Then, for each query position, it repeatedly asks whether the digit lies in the left block $F(n-2)$ or the right block $F(n-1)$. Every step replaces one huge word by a smaller one and eventually lands in either $A$ or $B$, where the digit is read directly.

## Pseudocode

```text
Store the two 100-digit base strings A and B.

Precompute the Fibonacci-style length table L(n)
until it is larger than every queried position.

To answer one query k:
    find the first n with L(n) >= k
    while n > 2:
        if k <= L(n - 2):
            move to F(n - 2)
            n = n - 2
        otherwise:
            subtract L(n - 2) from k
            move to F(n - 1)
            n = n - 1
    read the kth digit from A or B

Evaluate the 18 queries and accumulate digit * 10^n.
```

## Complexity Analysis

- **Time:** $O(\log k)$ per digit lookup, so the whole sum is tiny.
- **Space:** $O(\log k)$ for the length table.

## Answer

$$\boxed{850481152593119296}$$
