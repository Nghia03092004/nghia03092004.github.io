# Problem 224: Almost Right-angled Triangles II

## Problem Statement

How many ordered triples $(a,b,c)$ with

$$
a \le b \le c,
\qquad
a^2+b^2=c^2-1,
\qquad
a+b+c \le 75{,}000{,}000
$$

exist?

## Mathematical Development

Write

$$
Q(a,b,c)=a^2+b^2-c^2.
$$

We want the positive integer solutions of

$$
Q(a,b,c)=-1.
$$

Any common divisor of $a,b,c$ would also divide

$$
a^2+b^2-c^2=-1,
$$

so every solution is primitive.

The classical Berggren matrices for the ternary quadratic form $x^2+y^2-z^2$ are

$$
M_1=
\begin{pmatrix}
1&-2&2\\
2&-1&2\\
2&-2&3
\end{pmatrix},
\quad
M_2=
\begin{pmatrix}
1&2&2\\
2&1&2\\
2&2&3
\end{pmatrix},
\quad
M_3=
\begin{pmatrix}
-1&2&2\\
-2&1&2\\
-2&2&3
\end{pmatrix}.
$$

Each satisfies

$$
Q(M_i v)=Q(v),
$$

so they preserve the equation $Q=-1$.

The smallest positive solution is

$$
(2,2,3),
$$

and the Hall-Berggren theory for this form shows that every positive solution of $Q=-1$ is obtained by repeatedly applying these matrices.

Because $a$ and $b$ play symmetric roles, we sort them after each transformation. When $a=b$, the first and third matrices produce the same sorted child, so only two distinct descendants remain. That gives a duplicate-free tree.

Therefore the counting problem is reduced to a depth-first traversal of that tree, pruning as soon as

$$
a+b+c > 75{,}000{,}000.
$$

## Editorial

The direct factorization

$$
(c-b)(c+b)=a^2+1
$$

is valid, but it is not the most convenient way to count all solutions up to a perimeter of seventy-five million. The better observation is that the equation lives on the quadratic surface

$$
a^2+b^2-c^2=-1,
$$

and that this surface has the same Berggren-style tree structure that ordinary Pythagorean triples have.

So instead of factoring $4m^2+1$ for millions of values of $m$, we start from the root $(2,2,3)$ and generate every solution exactly once. Each matrix application gives a new valid triple, sorting the first two coordinates restores the convention $a \le b$, and the perimeter bound cuts off the recursion naturally. That turns the problem into a pure tree walk with almost no arithmetic overhead.

## Pseudocode

```text
Store the three Berggren matrices that preserve a^2 + b^2 - c^2.

Initialize a stack with the root triple (2, 2, 3).
Set the answer counter to 0.

While the stack is not empty:
    remove one triple (a, b, c)
    reorder the first two entries so that a <= b

    If a + b + c exceeds the perimeter limit:
        discard this branch
        continue

    Count the current triple.

    If a = b:
        only the first two matrices give distinct sorted children
    otherwise:
        use all three matrices

    For each chosen matrix:
        compute the transformed triple
        keep it only if all three coordinates stay positive
        sort the first two coordinates
        push the child onto the stack

Return the counter.
```

## Complexity Analysis

- **Time:** Linear in the number of generated triples; every valid solution is visited once.
- **Space:** Proportional to the maximum size of the DFS stack.

## Answer

$$\boxed{4137330}$$
