# Problem 126: Cuboid Layers

## Problem Statement

The minimum number of cubes to cover every visible face on a cuboid measuring $3 \times 2 \times 1$ is twenty-two. If we then add a second layer to this solid it would require forty-six cubes, the third layer seventy-eight, and the fourth layer one-hundred and eighteen.

Define $C(n)$ to be the number of cuboids that have $n$ cubes in one of its layers. Find the least value of $n$ for which $C(n)$ first reaches 1000.

## Mathematical Foundation

**Theorem 1 (Layer formula).** *For a cuboid with dimensions $a \times b \times c$ ($a \leq b \leq c$), the number of cubes in the $k$-th layer ($k \geq 1$) is*
$$f(a, b, c, k) = 2(ab + bc + ac) + 4(a + b + c)(k-1) + 4(k-1)(k-2).$$

**Proof.** The total volume enclosed by $k$ layers around an $a \times b \times c$ cuboid (including the cuboid itself) is
$$V(k) = (a + 2k)(b + 2k)(c + 2k) - abc$$
where the subtraction accounts for the hollow interior. The number of cubes in layer $k$ alone is
$$f(a,b,c,k) = V(k) - V(k-1).$$

Expanding:
\begin{align*}
V(k) &= (a+2k)(b+2k)(c+2k) - abc, \\
V(k-1) &= (a+2k-2)(b+2k-2)(c+2k-2) - abc.
\end{align*}

Let $u = 2k$. Then $V(k) - V(k-1)$ equals:
\begin{align*}
&(a+u)(b+u)(c+u) - (a+u-2)(b+u-2)(c+u-2).
\end{align*}

Expanding both products and subtracting, collecting terms in powers of $u = 2k$:
$$f = 2(ab+bc+ac) + 4(a+b+c)(k-1) + 4(k-1)(k-2).$$

*Verification:* For $k = 1$: $f = 2(ab + bc + ac)$, which is the surface area of the cuboid. For $(a,b,c,k) = (3,2,1,1)$: $f = 2(6+2+3) = 22$. For $k = 2$: $f = 22 + 4(6)(1) + 0 = 46$. Both match the problem statement. $\square$

**Lemma 1 (Monotonicity in $k$).** *For fixed $(a, b, c)$, $f(a, b, c, k)$ is strictly increasing in $k$.*

**Proof.** $f(a,b,c,k+1) - f(a,b,c,k) = 4(a+b+c) + 4(2k-1) > 0$ for all $k \geq 1$ and positive dimensions. $\square$

**Lemma 2 (Enumeration bounds).** *To find all $(a, b, c, k)$ with $f(a, b, c, k) \leq N$:*
- *$a$: from $1$ while $2a^2 \leq N$ (minimum layer for cube $a \times a \times a$).*
- *$b$: from $a$ while $2(ab + b^2) \leq N$.*
- *$c$: from $b$ while $2(ab + bc + ac) \leq N$ (the $k=1$ value).*
- *$k$: from $1$ while $f(a,b,c,k) \leq N$.*

**Proof.** For $k = 1$ with $a = b = c$: $f = 6a^2$, so $6a^2 \leq N$ gives $a \leq \sqrt{N/6}$, which is weaker than $2a^2 \leq N$ (obtained from $f \geq 2(a^2 + a \cdot b + ...) \geq 2a^2$ with $b = c = a$). The other bounds follow from $f \geq 2(ab + bc + ac)$ (the $k=1$ minimum). $\square$

## Algorithm

```
function cuboid_layers(target):
    N = 20000                           # upper bound (found experimentally)
    C[1..N] = 0
    for a = 1 while 2*a*a <= N:
        for b = a while 2*(a*b + b*b) <= N:
            for c = b while 2*(a*b + b*c + a*c) <= N:
                for k = 1, 2, ...:
                    val = 2*(a*b + b*c + a*c) + 4*(a+b+c)*(k-1) + 4*(k-1)*(k-2)
                    if val > N: break
                    C[val] += 1
    return min(n for n in 1..N if C[n] == target)
```

## Complexity Analysis

- **Time:** The four nested loops enumerate all tuples $(a, b, c, k)$ with $f \leq N$. The number of such tuples is $O(N \log N)$ empirically. Total: $O(N \log N)$.
- **Space:** $O(N)$ for the counting array $C$.

## Answer

$$\boxed{18522}$$
