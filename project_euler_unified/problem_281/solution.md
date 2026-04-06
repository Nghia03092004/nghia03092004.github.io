# Problem 281: Pizza Toppings

## Problem Statement

A pizza (a perfect circle) is cut into $m \cdot n$ equal slices. Let $f(m,n)$ denote the number of distinct distributions of $m$ distinct toppings on the pizza, each topping covering exactly $n$ consecutive slices ($m \ge 2$, $n \ge 1$), where two distributions are identified if and only if one is a rotation of the other (reflections are considered distinct).

Given: $f(2,1)=1$, $f(2,2)=f(3,1)=2$, $f(3,2)=16$.

Find $\displaystyle\sum_{\substack{m \ge 2,\; n \ge 1 \\ f(m,n) \le 10^{15}}} f(m,n)$.

## Mathematical Development

### Reformulation as a Necklace Counting Problem

The problem is equivalent to counting necklaces of $N = mn$ beads with $m$ distinct colors, each color appearing exactly $n$ times, under the cyclic group $C_N$ (rotations only).

### Theorem 1 (Burnside Counting Formula)

**Theorem.** *Let $N = mn$. Then*
$$f(m,n) = \frac{1}{mn} \sum_{t \mid n} \phi\!\left(\frac{n}{t}\right) \cdot \frac{(mt)!}{(t!)^m}.$$

**Proof.** By Burnside's lemma, the number of orbits of the cyclic group $C_N$ acting on the set of valid colorings is
$$f(m,n) = \frac{1}{N} \sum_{k=0}^{N-1} |\operatorname{Fix}(r^k)|,$$
where $r^k$ denotes the rotation by $k$ positions and $\operatorname{Fix}(r^k)$ is the set of colorings invariant under $r^k$.

**Step 1 (Characterizing fixed points).** A coloring is fixed by $r^k$ if and only if it is periodic with period dividing $g = \gcd(k, N)$. Such a coloring consists of $N/g$ identical copies of a block of length $g$. Each block must contain exactly $g/m$ copies of each of the $m$ colors. This requires $m \mid g$; when $m \nmid g$, the count $|\operatorname{Fix}(r^k)| = 0$.

**Step 2 (Counting valid blocks).** When $m \mid g$, the block of length $g$ contains $g/m$ copies of each of $m$ colors. The number of such arrangements is the multinomial coefficient
$$\binom{g}{g/m, g/m, \ldots, g/m} = \frac{g!}{\bigl((g/m)!\bigr)^m}.$$

**Step 3 (Grouping by gcd).** For each divisor $g$ of $N$, the number of integers $k \in \{0, 1, \ldots, N-1\}$ satisfying $\gcd(k,N) = g$ is exactly $\phi(N/g)$, where $\phi$ is Euler's totient function. Therefore
$$f(m,n) = \frac{1}{N} \sum_{\substack{g \mid N \\ m \mid g}} \phi\!\left(\frac{N}{g}\right) \cdot \frac{g!}{\bigl((g/m)!\bigr)^m}.$$

**Step 4 (Substitution).** Set $g = mt$ where $t$ is a positive integer. The condition $g \mid N = mn$ with $m \mid g$ is equivalent to $mt \mid mn$, i.e., $t \mid n$. Furthermore, $N/g = mn/(mt) = n/t$ and $(g/m)! = t!$. Thus
$$f(m,n) = \frac{1}{mn} \sum_{t \mid n} \phi\!\left(\frac{n}{t}\right) \cdot \frac{(mt)!}{(t!)^m}. \qquad \blacksquare$$

### Lemma 1 (Verification of Given Values)

**Lemma.** *The formula reproduces all four given values.*

**Proof.** Direct computation:
- $f(2,1) = \frac{1}{2}\,\phi(1)\,\frac{2!}{(1!)^2} = \frac{1}{2}\cdot 1 \cdot 2 = 1$.
- $f(2,2) = \frac{1}{4}\bigl[\phi(2)\,\frac{2!}{(1!)^2} + \phi(1)\,\frac{4!}{(2!)^2}\bigr] = \frac{1}{4}[1 \cdot 2 + 1 \cdot 6] = 2$.
- $f(3,1) = \frac{1}{3}\,\phi(1)\,\frac{3!}{(1!)^3} = \frac{6}{3} = 2$.
- $f(3,2) = \frac{1}{6}\bigl[\phi(2)\,\frac{3!}{(1!)^3} + \phi(1)\,\frac{6!}{(2!)^3}\bigr] = \frac{1}{6}[6 + 90] = 16$. $\qquad \blacksquare$

### Lemma 2 (Search Bounds)

**Lemma.** *For $m \ge 19$ and $n \ge 1$, we have $f(m,1) = (m-1)! > 10^{15}$. For $m = 2$ and $n \ge 30$, we have $f(2,n) > 10^{15}$. Hence it suffices to search $2 \le m \le 18$ and $1 \le n \le 29$.*

**Proof.** We have $f(m,1) = \frac{1}{m}\phi(1)\frac{m!}{(1!)^m} = (m-1)!$. Since $18! = 6402373705728000 > 10^{15}$, we need $m \le 18$. For fixed $m$, $f(m,n)$ is increasing in $n$ (the dominant term $(mn)!/(n!)^m$ grows super-exponentially), so once $f(m,n) > 10^{15}$ for some $n$, all larger $n$ also exceed the bound. Numerical evaluation confirms $n \le 29$ suffices. $\qquad \blacksquare$

## Algorithm

```
function solve():
    total = 0
    for m = 2, 3, ..., 18:
        for n = 1, 2, ..., 29:
            val = f(m, n)
            if val <= 10^15:
                total += val
    return total

function f(m, n):
    result = 0
    for each divisor t of n:
        result += euler_phi(n / t) * (m*t)! / (t!)^m
    return result / (m * n)
```

## Complexity Analysis

- **Time:** The outer loops iterate over at most $17 \times 29 = 493$ pairs $(m,n)$. For each pair, the inner loop iterates over at most $d(n) \le d(29) = 2$ divisors of $n$ (in general, $d(n) \le 6$ for $n \le 29$). Each iteration performs $O(mn)$ arithmetic operations for the factorial computation. Since all parameters are bounded by small constants, the total running time is $O(1)$.
- **Space:** $O(1)$.

## Answer

$$\boxed{1485776387445623}$$
