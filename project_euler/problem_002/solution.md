# Problem 2: Even Fibonacci Numbers

## Problem Statement

By considering the terms in the Fibonacci sequence whose values do not exceed four million, find the sum of the even-valued terms. That is, compute

$$S = \sum_{\substack{F_n \le 4\,000\,000 \\ 2 \mid F_n}} F_n$$

where $F_1 = 1, F_2 = 1$, and $F_n = F_{n-1} + F_{n-2}$ for $n \ge 3$.

## Mathematical Development

### Preliminaries

**Definition 1 (Fibonacci Sequence).** The Fibonacci sequence $(F_n)_{n \ge 1}$ is defined by $F_1 = 1$, $F_2 = 1$, and $F_n = F_{n-1} + F_{n-2}$ for $n \ge 3$.

**Lemma 1 (Fibonacci Addition Identity).** For all integers $m \ge 2$ and $n \ge 1$,

$$F_{m+n} = F_m F_{n+1} + F_{m-1} F_n.$$

**Proof.** We fix $m \ge 2$ and proceed by induction on $n$.

*Base case.* $n = 1$: $F_{m+1} = F_m F_2 + F_{m-1} F_1 = F_m + F_{m-1}$, which is the defining recurrence. The identity holds.

*Inductive step.* Assume the identity holds for $n$ and for $n - 1$ (with $n \ge 2$). Then

$$F_{m+n+1} = F_{m+n} + F_{m+n-1} = (F_m F_{n+1} + F_{m-1} F_n) + (F_m F_n + F_{m-1} F_{n-1})$$
$$= F_m(F_{n+1} + F_n) + F_{m-1}(F_n + F_{n-1}) = F_m F_{n+2} + F_{m-1} F_{n+1}.$$

This is the identity with $n$ replaced by $n+1$, completing the induction. $\square$

### Parity Periodicity

**Theorem 1 (Pisano Period Modulo 2).** $F_n$ is even if and only if $3 \mid n$.

**Proof.** The Fibonacci recurrence modulo 2, $F_n \equiv F_{n-1} + F_{n-2} \pmod{2}$, depends only on the pair $(F_{n-1} \bmod 2, F_{n-2} \bmod 2)$. Since there are at most $2^2 = 4$ distinct pairs, the sequence of pairs is eventually periodic. Computing explicitly:

| $n$ | 1 | 2 | 3 | 4 | 5 | 6 | 7 |
|-----|---|---|---|---|---|---|---|
| $F_n$ | 1 | 1 | 2 | 3 | 5 | 8 | 13 |
| $F_n \bmod 2$ | 1 | 1 | 0 | 1 | 1 | 0 | 1 |

We observe that $(F_4 \bmod 2, F_5 \bmod 2) = (1, 1) = (F_1 \bmod 2, F_2 \bmod 2)$. Since the recurrence is determined by consecutive pairs, the parity sequence is periodic with period $\pi(2) = 3$. The pattern is $(1, 1, 0, 1, 1, 0, \ldots)$, so $2 \mid F_n$ if and only if $n \equiv 0 \pmod{3}$.

*Uniqueness of the period.* Since $F_1 \bmod 2 = 1 \ne 0 = F_3 \bmod 2$, the period cannot be 1 or 2. Hence $\pi(2) = 3$ is the minimal period. $\square$

### The Even-Indexed Fibonacci Subsequence

**Definition 2.** Let $E_k = F_{3k}$ for $k \ge 1$. By Theorem 1, $(E_k)_{k \ge 1}$ is the subsequence of all even Fibonacci numbers.

**Theorem 2 (Even Fibonacci Recurrence).** For all $k \ge 3$,

$$E_k = 4E_{k-1} + E_{k-2}$$

with initial conditions $E_1 = 2$ and $E_2 = 8$.

**Proof.** We must show $F_{3k} = 4F_{3(k-1)} + F_{3(k-2)}$, i.e., $F_{3k} = 4F_{3k-3} + F_{3k-6}$.

Apply Lemma 1 with $m = 3$ and $n = 3k - 3$:

$$F_{3k} = F_3 F_{3k-2} + F_2 F_{3k-3} = 2F_{3k-2} + F_{3k-3}. \tag{1}$$

Now express $F_{3k-2}$ in terms of earlier values. Since $F_{3k-2} = F_{3k-3} + F_{3k-4}$:

$$F_{3k} = 2(F_{3k-3} + F_{3k-4}) + F_{3k-3} = 3F_{3k-3} + 2F_{3k-4}. \tag{2}$$

Apply Lemma 1 with $m = 3$ and $n = 3k - 6$ (valid for $k \ge 3$):

$$F_{3k-3} = F_3 F_{3k-5} + F_2 F_{3k-6} = 2F_{3k-5} + F_{3k-6}. \tag{3}$$

From (3): $F_{3k-5} = (F_{3k-3} - F_{3k-6})/2$. Also, $F_{3k-4} = F_{3k-5} + F_{3k-6}$, so

$$F_{3k-4} = \frac{F_{3k-3} - F_{3k-6}}{2} + F_{3k-6} = \frac{F_{3k-3} + F_{3k-6}}{2}. \tag{4}$$

Substituting (4) into (2):

$$F_{3k} = 3F_{3k-3} + 2 \cdot \frac{F_{3k-3} + F_{3k-6}}{2} = 3F_{3k-3} + F_{3k-3} + F_{3k-6} = 4F_{3k-3} + F_{3k-6}.$$

For the initial conditions: $E_1 = F_3 = 2$ and $E_2 = F_6 = 8$.

*Verification:* $E_3 = F_9 = 34 = 4 \cdot 8 + 2 = 4E_2 + E_1$. $\checkmark$ $\square$

### Growth Rate and Termination

**Theorem 3 (Closed Form and Growth Rate).** The sequence $(E_k)$ satisfies

$$E_k = \frac{\alpha^k - \beta^k}{\sqrt{5}}$$

where $\alpha = 2 + \sqrt{5}$ and $\beta = 2 - \sqrt{5}$ are the roots of the characteristic equation $x^2 - 4x - 1 = 0$.

**Proof.** The characteristic polynomial of $E_k = 4E_{k-1} + E_{k-2}$ is $x^2 - 4x - 1 = 0$, with roots

$$\alpha = \frac{4 + \sqrt{16 + 4}}{2} = 2 + \sqrt{5}, \qquad \beta = \frac{4 - \sqrt{20}}{2} = 2 - \sqrt{5}.$$

The general solution is $E_k = c_1 \alpha^k + c_2 \beta^k$. Applying initial conditions:

$$E_1 = c_1 \alpha + c_2 \beta = 2, \qquad E_2 = c_1 \alpha^2 + c_2 \beta^2 = 8.$$

From $E_1$: $c_2 = (2 - c_1 \alpha)/\beta$. Substituting into $E_2$ and using $\alpha + \beta = 4$, $\alpha \beta = -1$, $\alpha - \beta = 2\sqrt{5}$:

Since $\alpha^2 = 4\alpha + 1$ and $\beta^2 = 4\beta + 1$ (as roots of $x^2 = 4x + 1$):

$$E_2 = c_1(4\alpha + 1) + c_2(4\beta + 1) = 4(c_1 \alpha + c_2 \beta) + (c_1 + c_2) = 4 \cdot 2 + (c_1 + c_2) = 8 + (c_1 + c_2).$$

So $c_1 + c_2 = 0$, hence $c_2 = -c_1$. Then $E_1 = c_1(\alpha - \beta) = c_1 \cdot 2\sqrt{5} = 2$, giving $c_1 = 1/\sqrt{5}$ and $c_2 = -1/\sqrt{5}$. $\square$

**Corollary 1 (Exponential Growth).** $E_k = \Theta(\alpha^k)$ where $\alpha = 2 + \sqrt{5} \approx 4.236$. Equivalently, $E_k = \Theta(\phi^{3k})$ where $\phi = (1+\sqrt{5})/2$ is the golden ratio.

**Proof.** Since $|\beta| = |2 - \sqrt{5}| \approx 0.236 < 1$, we have $|\beta^k| \to 0$ as $k \to \infty$, so $E_k \sim \alpha^k / \sqrt{5}$. The equivalence $\alpha = \phi^3$ follows from direct computation: $\phi^3 = \phi \cdot \phi^2 = \phi(\phi + 1) = \phi^2 + \phi = 2\phi + 1 = 2 + \sqrt{5} = \alpha$. $\square$

**Corollary 2 (Number of Terms Below $L$).** The number of even Fibonacci terms not exceeding $L$ is $\lfloor \log_\alpha(\sqrt{5} \cdot L) \rfloor + O(1)$. For $L = 4 \times 10^6$, this count is 11.

### Summation Formula

**Theorem 4 (Telescoping Sum for Even Fibonacci Numbers).** Define $T_k = \sum_{j=1}^{k} E_j$. Then

$$T_k = \frac{E_{k+1} + E_k - 2}{4}.$$

**Proof.** By strong induction on $k$.

*Base case.* $k = 1$: $T_1 = E_1 = 2$. And $(E_2 + E_1 - 2)/4 = (8 + 2 - 2)/4 = 8/4 = 2$. $\checkmark$

*Inductive step.* Assume $T_{k-1} = (E_k + E_{k-1} - 2)/4$. Then

$$T_k = T_{k-1} + E_k = \frac{E_k + E_{k-1} - 2}{4} + E_k = \frac{E_k + E_{k-1} - 2 + 4E_k}{4} = \frac{5E_k + E_{k-1} - 2}{4}.$$

We need to show this equals $(E_{k+1} + E_k - 2)/4$, i.e., $5E_k + E_{k-1} = E_{k+1} + E_k$, i.e., $E_{k+1} = 4E_k + E_{k-1}$. This is precisely the recurrence of Theorem 2. $\square$

### Numerical Evaluation

**Proposition 1.** The sum of all even Fibonacci numbers not exceeding $4\,000\,000$ is $4\,613\,732$.

**Proof.** Computing the even Fibonacci sequence:

| $k$ | $E_k = F_{3k}$ | Running sum |
|-----|-----------------|-------------|
| 1 | 2 | 2 |
| 2 | 8 | 10 |
| 3 | 34 | 44 |
| 4 | 144 | 188 |
| 5 | 610 | 798 |
| 6 | 2,584 | 3,382 |
| 7 | 10,946 | 14,328 |
| 8 | 46,368 | 60,696 |
| 9 | 196,418 | 257,114 |
| 10 | 832,040 | 1,089,154 |
| 11 | 3,524,578 | 4,613,732 |
| 12 | 14,930,352 | (exceeds limit) |

Each $E_k$ is verified via the recurrence $E_k = 4E_{k-1} + E_{k-2}$: e.g., $E_{12} = 4 \cdot 3\,524\,578 + 832\,040 = 14\,930\,352 > 4\,000\,000$, so $k = 11$ is the last term included.

*Verification via Theorem 4:* $T_{11} = (E_{12} + E_{11} - 2)/4 = (14\,930\,352 + 3\,524\,578 - 2)/4 = 18\,454\,928/4 = 4\,613\,732$. $\checkmark$ $\square$

## Algorithm

We generate only the even Fibonacci terms, not the entire Fibonacci sequence. Starting from $E_1 = 2$ and $E_2 = 8$, we repeatedly use the recurrence $E_k = 4E_{k-1} + E_{k-2}$, add each term to a running total, and stop once the next even term exceeds the limit. This is sufficient because Theorem 1 shows that every third Fibonacci number is even, and Theorem 2 gives a recurrence that enumerates exactly those terms.

**Theorem 5 (Algorithm Correctness).** `SumEvenFibonacci(L)` returns $\sum \{E_k : E_k \le L\}$ for all $L \ge 1$.

**Proof.** We establish two loop invariants. Let $a_i, b_i$ denote the values of $a, b$ at the start of iteration $i$ (with $i = 1$ being the first iteration).

*Invariant 1:* $a_i = E_i$ and $b_i = E_{i+1}$.

This holds initially ($a_1 = 2 = E_1$, $b_1 = 8 = E_2$). After the update $(a, b) \leftarrow (b, 4b + a)$, we have $a_{i+1} = b_i = E_{i+1}$ and $b_{i+1} = 4b_i + a_i = 4E_{i+1} + E_i = E_{i+2}$ by Theorem 2.

*Invariant 2:* At the start of iteration $i$, `total` $= \sum_{j=1}^{i-1} E_j$.

This holds initially (total = 0). After adding $a_i = E_i$, total becomes $\sum_{j=1}^{i} E_j$.

The loop terminates when $a_i = E_i > L$, at which point total $= \sum_{j=1}^{i-1} E_j = \sum\{E_k : E_k \le L\}$. Termination is guaranteed by Corollary 1 ($E_k \to \infty$). $\square$

## Pseudocode

```text
function sumEvenFibonacci(limit):
    current <- 2
    next <- 8
    total <- 0
    while current <= limit:
        total <- total + current
        (current, next) <- (next, 4 * next + current)
    return total
```

## Complexity Analysis

**Theorem 6.** `SumEvenFibonacci(L)` runs in $\Theta(\log L)$ time and $O(1)$ space.

**Proof.** *Time:* Each iteration performs $O(1)$ arithmetic operations (one comparison, one addition, one multiplication by 4, one addition). The loop executes exactly $K$ iterations where $K = \max\{k : E_k \le L\}$. By Corollary 2, $K = \lfloor \log_\alpha(\sqrt{5} \cdot L) \rfloor + O(1) = \Theta(\log L)$ since $\alpha > 1$ is constant. Hence total time is $\Theta(\log L)$.

*Space:* The algorithm maintains exactly three integer variables (`a`, `b`, `total`), which is $O(1)$. $\square$

## Answer

$$\boxed{4613732}$$
