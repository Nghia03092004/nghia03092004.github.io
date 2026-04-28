# Problem 174: Counting the Number of "Hollow" Square Laminae That Can Form One, Two, Three, ... Distinct Arrangements

## Problem Statement

Define $N(t)$ as the number of hollow square laminae that can be formed using exactly $t$ tiles. How many values of $t \leq 10^6$ satisfy $1 \leq N(t) \leq 10$?

## Mathematical Development

**Theorem 1.** *(Tile-count parameterization.) Every hollow square lamina with outer side $a$, inner side $b$, $a \equiv b \pmod{2}$, $a \geq b + 2$, $b \geq 1$, uses $t = 4k(b + k)$ tiles where $k = (a - b)/2 \geq 1$. In particular, $4 \mid t$.*

**Proof.** From Problem 173, $t = a^2 - b^2 = (a - b)(a + b)$. Setting $a - b = 2k$ gives $a + b = 2b + 2k$, so $t = 2k(2b + 2k) = 4k(b + k)$. Since $k \geq 1$ and $b + k \geq 2$, we have $t \geq 8$ and $4 \mid t$. $\square$

**Theorem 2.** *(Reduction to divisor counting.) Write $t = 4m$. Then $N(t)$ equals the number of divisors $k$ of $m$ satisfying $1 \leq k < \sqrt{m}$.*

**Proof.** Setting $q = b + k$, the equation $m = k \cdot q$ must hold with $q > k$ (since $b = q - k \geq 1$ requires $q > k$). Each such factorization corresponds to a unique lamina. The number of ordered pairs $(k, q)$ with $kq = m$ and $k < q$ is the number of divisors of $m$ strictly less than $\sqrt{m}$:
$$N(4m) = \begin{cases} \frac{d(m) - 1}{2} & \text{if } m \text{ is a perfect square} \\ \frac{d(m)}{2} & \text{otherwise} \end{cases}$$
which in both cases equals $|\{k : k \mid m,\; k < \sqrt{m}\}|$. $\square$

**Lemma 1.** *(Divisor sieve.) For $M = N/4$, the array $N[1 \ldots M]$ can be computed in $O(M \log M)$ time by iterating $k = 1, 2, \ldots, \lfloor\sqrt{M}\rfloor$ and for each $k$, incrementing $N[kq]$ for all $q = k + 1, k + 2, \ldots$ with $kq \leq M$.*

**Proof.** Each pair $(k, q)$ with $k < q$ and $kq = m$ is visited exactly once. The total number of pairs is $\sum_{k=1}^{\lfloor\sqrt{M}\rfloor} \lfloor M/k - k \rfloor \leq \sum_{k=1}^{\lfloor\sqrt{M}\rfloor} M/k = O(M \log \sqrt{M}) = O(M \log M)$. $\square$

## Editorial
The divisor reformulation turns the problem into a counting sieve on $m=t/4$. For each factor pair $m=kq$ with $k<q$, there is exactly one hollow lamina using $t=4m$ tiles. So instead of generating laminae geometrically, the program counts how many strict factor pairs each $m$ has.

The implementation loops over the smaller factor $k$ and increments every product $kq$ with $q>k$. After that sieve pass, the array entry for $m$ is exactly $N(4m)$. The final answer is obtained by counting how many of those entries fall between 1 and 10 inclusive.

## Pseudocode

```text
Let $M = 10^6 / 4$ and create an array of zero counts for $1 \le m \le M$.

For each possible smaller factor $k$:
    let the larger factor start at $k+1$ so that only strict pairs are counted.
    For every $q$ with $kq \le M$:
        increment the counter of $m = kq$.

After all factor pairs are processed:
    scan the array once more,
    and count how many entries lie between 1 and 10 inclusive.

Return that count.
```

## Complexity Analysis

- **Time:** $O(M \log M)$ where $M = 250{,}000$. The inner loop total is bounded by $\sum_{k=1}^{500} M/k \approx M \ln 500 \approx 1.55 \times 10^6$.
- **Space:** $O(M) = O(250{,}000)$ for the count array.

## Answer

$$\boxed{209566}$$
