# Problem 8: Largest Product in a Series

## Problem Statement

Given a 1000-digit number $D = d_0 d_1 \cdots d_{999}$ (where each $d_i \in \{0, 1, \ldots, 9\}$), find the maximum product of 13 consecutive digits:
$$M = \max_{0 \le i \le 987} \prod_{j=0}^{12} d_{i+j}.$$

## Mathematical Development

### Window Products

**Definition 1.** For integers $i$ and $k$ with $0 \le i \le N-k$, define the *window product*

$$P_i = \prod_{j=0}^{k-1} d_{i+j}.$$

For this problem, $N = 1000$ and $k = 13$, so there are

$$N-k+1 = 1000-13+1 = 988$$

candidate windows.

**Theorem 1 (Exhaustive search is sufficient).** The desired quantity is

$$M = \max_{0 \le i \le N-k} P_i,$$

so evaluating all $988$ window products and retaining the largest one yields the correct answer.

*Proof.* The set

$$\mathcal{W} = \{P_i : 0 \le i \le N-k\}$$

is finite. Therefore it has a maximum element. An exhaustive search computes every element of $\mathcal{W}$ and returns the largest value encountered, which is exactly $\max \mathcal{W} = M$. $\square$

**Lemma 1 (Zero windows).** If one of the digits in a length-13 window is zero, then the product of that window is zero.

*Proof.* A product containing a factor equal to zero is zero. $\square$

*Remark.* Lemma 1 explains why many windows are automatically non-optimal, but the reference implementation does not need any special-case optimization: it simply computes all $988$ products directly.

### Numerical Evaluation

**Proposition 1.** The maximum product is attained by the 13-digit block

$$5576689664895,$$

which begins at index $197$ (using zero-based indexing), and its product is

$$23{,}514{,}624{,}000.$$

*Proof.* Exhaustive evaluation of the $988$ window products shows that the largest one occurs for the block

$$d_{197} d_{198} \cdots d_{209} = 5576689664895.$$

Its product is computed directly as

$$\begin{aligned}
5 \cdot 5 &= 25,\\
25 \cdot 7 &= 175,\\
175 \cdot 6 &= 1050,\\
1050 \cdot 6 &= 6300,\\
6300 \cdot 8 &= 50400,\\
50400 \cdot 9 &= 453600,\\
453600 \cdot 6 &= 2721600,\\
2721600 \cdot 6 &= 16329600,\\
16329600 \cdot 4 &= 65318400,\\
65318400 \cdot 8 &= 522547200,\\
522547200 \cdot 9 &= 4702924800,\\
4702924800 \cdot 5 &= 23514624000.
\end{aligned}$$

Hence the maximal product is $23{,}514{,}624{,}000$. $\square$

## Algorithm

```
function LargestProductInSeries(d[0..N-1], k):
    best <- 0
    for i <- 0 to N-k:
        P <- 1
        for j <- 0 to k-1:
            P <- P * d[i+j]
        if P > best:
            best <- P
    return best
```

**Theorem 2 (Algorithm correctness).** `LargestProductInSeries(d, k)` returns the largest product of $k$ consecutive digits.

*Proof.* For each index $i$ with $0 \le i \le N-k$, the inner loop computes exactly

$$P_i = \prod_{j=0}^{k-1} d_{i+j}.$$

The outer loop visits every admissible starting position exactly once and keeps the maximum value seen so far in `best`. By Theorem 1, the maximum over these values is precisely the desired quantity. Therefore the returned value is correct. $\square$

## Complexity Analysis

**Theorem 3.** The algorithm runs in $\Theta((N-k+1)k)$ time and uses $O(N)$ space in the given implementation.

*Proof.* There are exactly $N-k+1$ windows, and for each window the inner loop performs exactly $k$ multiplications. Hence the running time is

$$\Theta((N-k+1)k).$$

For this problem,

$$ (N-k+1)k = 988 \cdot 13 = 12{,}844,$$

so the computation is tiny. The Python implementation first stores the 1000 digits in a list, which uses $O(N)$ space; aside from that list, only a constant number of scalar variables are maintained. $\square$

## Answer

$$\boxed{23514624000}$$
