# Problem 15: Lattice Paths

## Problem Statement

Starting at the top-left corner of a $20 \times 20$ grid, how many routes exist to the bottom-right corner if only rightward and downward moves are permitted?

Formally: count the number of monotone lattice paths from $(0,0)$ to $(20,20)$ using unit steps $(+1,0)$ (right) and $(0,+1)$ (down).

## Mathematical Development

### Definitions

**Definition 1.** A *monotone lattice path* from $(0,0)$ to $(m,n)$ is a sequence of $m + n$ unit steps, each either $R = (+1,0)$ or $D = (0,+1)$, such that exactly $m$ steps are $R$ and exactly $n$ steps are $D$.

**Definition 2.** Let $P(i,j)$ denote the number of monotone lattice paths from $(0,0)$ to $(i,j)$.

### Theorems

**Theorem 1 (Closed-form path count).** The number of monotone lattice paths from $(0,0)$ to $(m,n)$ is

$$P(m,n) = \binom{m+n}{m} = \frac{(m+n)!}{m!\, n!}.$$

*Proof.* A path is a binary string of length $m + n$ over the alphabet $\{R, D\}$ containing exactly $m$ copies of $R$. The path is uniquely determined by specifying the positions of the $m$ right-steps among the $m + n$ total steps. The number of ways to choose these positions is $\binom{m+n}{m}$. $\square$

**Theorem 2 (Pascal recurrence).** The function $P$ satisfies:

$$P(i,0) = 1 \quad \forall\, i \ge 0, \qquad P(0,j) = 1 \quad \forall\, j \ge 0,$$

$$P(i,j) = P(i-1,j) + P(i,j-1) \quad \text{for } i, j \ge 1.$$

*Proof.* **Boundary conditions:** There is exactly one path from $(0,0)$ to $(i,0)$ (all $R$-steps) and one path to $(0,j)$ (all $D$-steps).

**Recurrence:** Every path to $(i,j)$ with $i,j \ge 1$ takes its last step from either $(i-1,j)$ (a right-step) or $(i,j-1)$ (a down-step). These two sets of paths are disjoint and exhaustive, so $P(i,j) = P(i-1,j) + P(i,j-1)$. $\square$

**Lemma 1 (Equivalence with Pascal's triangle).** $P(i,j) = \binom{i+j}{i}$ for all $i,j \ge 0$.

*Proof.* By strong induction on $i + j$. The base case $i + j = 0$ gives $P(0,0) = 1 = \binom{0}{0}$. For boundary cases, $P(i,0) = 1 = \binom{i}{i}$ and $P(0,j) = 1 = \binom{j}{0}$. For $i,j \ge 1$, by the inductive hypothesis and Theorem 2:

$$P(i,j) = P(i-1,j) + P(i,j-1) = \binom{i-1+j}{i-1} + \binom{i+j-1}{i} = \binom{i+j}{i},$$

where the last equality is Pascal's identity: $\binom{n-1}{k-1} + \binom{n-1}{k} = \binom{n}{k}$. $\square$

**Theorem 3 (Exact evaluation via telescoping product).** The binomial coefficient $\binom{2n}{n}$ can be computed as

$$\binom{2n}{n} = \prod_{k=1}^{n} \frac{n+k}{k}.$$

Moreover, the partial products $\prod_{k=1}^{j} \frac{n+k}{k} = \binom{n+j}{j}$ are integers for all $1 \le j \le n$.

*Proof.* We have

$$\prod_{k=1}^{n} \frac{n+k}{k} = \frac{(n+1)(n+2) \cdots (2n)}{1 \cdot 2 \cdots n} = \frac{(2n)!}{n! \cdot n!} = \binom{2n}{n}.$$

For integrality of partial products: $\prod_{k=1}^{j} \frac{n+k}{k} = \frac{(n+1)(n+2)\cdots(n+j)}{j!} = \binom{n+j}{j}$, which is an integer since it counts the number of $j$-element subsets of an $(n+j)$-element set. $\square$

**Corollary 1.** The multiplicative formula computes $\binom{2n}{n}$ using only integer arithmetic (no fractions), since each intermediate quotient $\text{result} \cdot (n+k) / k$ is exact.

*Proof.* At step $k$, the running product equals $\binom{n+k}{k} \cdot \frac{(n+k-1)!}{(n+k-1)!}$... more directly: after multiplying by $(n+k)$ and dividing by $k$, the result equals $\binom{n+k}{k}$, which is an integer by Theorem 3. $\square$

### Numerical Evaluation

$$\binom{40}{20} = \frac{40!}{(20!)^2} = 137\,846\,528\,820.$$

## Algorithm

We compute the central binomial coefficient multiplicatively instead of expanding full factorials. The loop traverses $k = 1, \ldots, n$ and multiplies the accumulator by $(n+k)/k$ at each step, which stays integral by the binomial-coefficient derivation. This is sufficient because the resulting product is exactly $\binom{2n}{n}$, the number of lattice paths.

**Correctness.** By Theorem 3, this computes $\prod_{k=1}^{n} \frac{n+k}{k} = \binom{2n}{n}$. By Corollary 1, all divisions are exact.

## Pseudocode

```text
function latticePaths(size):
    result <- 1
    for k <- 1 to size:
        result <- result * (size + k) / k
    return result
```

## Complexity Analysis

**Proposition.** The multiplicative formula runs in $O(n)$ time and $O(1)$ auxiliary space (assuming $O(1)$-cost arithmetic on integers of size $O(\log \binom{2n}{n}) = O(n)$ bits).

*Proof.* The loop executes $n$ iterations, each performing one multiplication and one division. The result $\binom{2n}{n}$ has $\Theta(n)$ bits by Stirling's approximation:

$$\log_2 \binom{2n}{n} = 2n - \frac{1}{2}\log_2(\pi n) + O(1/n) \approx 2n.$$

If arbitrary-precision arithmetic is used, each multiplication costs $O(n)$ bit operations, giving $O(n^2)$ total. With machine-word arithmetic (valid for $n = 20$ since $\binom{40}{20} \approx 1.38 \times 10^{11} < 2^{63}$), the cost is $O(n)$. $\square$

**Remark.** The dynamic programming approach (Theorem 2) requires $O(n^2)$ time and $O(n)$ space (using row-by-row computation).

## Answer

$$\boxed{137846528820}$$
