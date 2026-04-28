# Problem 120: Square Remainders

## Problem Statement

Let $r$ be the remainder when $(a-1)^n + (a+1)^n$ is divided by $a^2$.

For $3 \le a \le 1000$, find $\sum r_{\max}$, where $r_{\max}$ is the maximum value of $r$ over all positive integers $n$.

## Mathematical Development

### Binomial Expansion Modulo $a^2$

Using the binomial theorem:
$$(a-1)^n = \sum_{k=0}^{n} \binom{n}{k} a^k (-1)^{n-k}$$
$$(a+1)^n = \sum_{k=0}^{n} \binom{n}{k} a^k$$

When we add these and reduce modulo $a^2$, only terms with $k = 0$ and $k = 1$ survive (terms with $k \ge 2$ are divisible by $a^2$):

$$(a-1)^n + (a+1)^n \equiv [(-1)^n + 1] + [(-1)^{n-1} + 1] \cdot na \pmod{a^2}$$

### Case Analysis

**When $n$ is even:**
$$(-1)^n = 1, \quad (-1)^{n-1} = -1$$
$$r = (1 + 1) + (-1 + 1) \cdot na = 2$$

**When $n$ is odd:**
$$(-1)^n = -1, \quad (-1)^{n-1} = 1$$
$$r = (-1 + 1) + (1 + 1) \cdot na = 2na \bmod a^2$$

### Maximizing $r$ for Odd $n$

For odd $n$, $r = 2na \bmod a^2$. We want to maximize $2na \bmod a^2$.

Let $m = n \bmod a$ (since $2na \bmod a^2$ depends only on $n \bmod a$, as $2na \bmod a^2 = a(2n \bmod a)$... let me be more precise).

Actually: $2na \bmod a^2 = a \cdot (2n \bmod a)$. So we want to maximize $2n \bmod a$ where $n$ is odd.

If $a$ is odd: As $n$ ranges over odd values, $2n \bmod a$ takes all values in $\{0, 1, \ldots, a-1\}$ (since $\gcd(2, a) = 1$). The maximum of $2n \bmod a$ is $a - 1$, giving $r_{\max} = a(a-1)$.

If $a$ is even: As $n$ ranges over odd values, $2n$ ranges over $\{2, 6, 10, \ldots\}$, i.e., $2n \equiv 2 \pmod{4}$. Then $2n \bmod a$ can reach at most $a - 2$ (when $a$ is even). So $r_{\max} = a(a-2)$.

### Closed-Form Formula

$$r_{\max}(a) = \begin{cases} a(a-1) & \text{if } a \text{ is odd} \\ a(a-2) & \text{if } a \text{ is even} \end{cases}$$

### Summation

$$\sum_{a=3}^{1000} r_{\max}(a) = \sum_{\substack{a=3 \\ a \text{ odd}}}^{999} a(a-1) + \sum_{\substack{a=4 \\ a \text{ even}}}^{1000} a(a-2)$$

Computing this sum yields **333082500**.

### Proof of the Formula

For odd $a$: Since $\gcd(2, a) = 1$, as $n$ ranges over all odd positive integers, the residues $2n \bmod a$ cycle through all residues $\{0, 1, \ldots, a-1\}$. The maximum residue is $a - 1$, achieved when $2n \equiv a - 1 \pmod{a}$, i.e., $n \equiv (a-1)/2 \cdot 1 \pmod{a}$ (using the inverse of 2 mod $a$). Since $(a-1)/2$ is an integer and we can always find an odd $n$ achieving this, $r_{\max} = a(a-1)$.

For even $a$: Write $a = 2m$. Then $2n \bmod a = 2n \bmod 2m = 2(n \bmod m)$. For odd $n$, $n \bmod m$ ranges over values achievable by odd $n$, and the maximum of $2(n \bmod m)$ is $2(m - 1) = a - 2$. Thus $r_{\max} = a(a - 2)$.

### Correctness

**Theorem.** The method described above computes exactly the quantity requested in the problem statement.

*Proof.* The preceding analysis identifies the admissible objects and derives the formula, recurrence, or exhaustive search carried out by the algorithm. The computation evaluates exactly that specification, so every valid contribution is included once and no invalid contribution is counted. Therefore the returned value is the required answer. $\square$

## Editorial
The decisive observation is that almost the entire binomial expansion disappears modulo $a^2$. After expanding $(a-1)^n$ and $(a+1)^n$, only the constant term and the linear term in $a$ survive, so the remainder can be written down explicitly from the parity of $n$. Even exponents contribute only the trivial remainder $2$, so the maximum must come from odd exponents.

For odd $n$, the remainder is $2na \bmod a^2$, which is the same as $a \cdot (2n \bmod a)$. That reduces the problem to a simple residue maximization. When $a$ is odd, the multiplier 2 is invertible modulo $a$, so the largest attainable residue is $a-1$; when $a$ is even, the best possible residue is $a-2$. The implementation therefore evaluates the resulting closed form for each $a$ from 3 to 1000 and adds the values directly.

## Pseudocode

```text
Set the total to zero.

For each a from 3 through 1000:
    If a is odd, use r_max = a(a - 1).
    If a is even, use r_max = a(a - 2).
    Add r_max to the total.

Return the total.
```

## Complexity Analysis

- **Time**: $O(N)$ where $N = 1000$.
- **Space**: $O(1)$.

## Answer

$$\boxed{333082500}$$
