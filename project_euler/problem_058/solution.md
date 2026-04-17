# Problem 58: Spiral Primes

## Problem Statement

Starting with 1 and spiralling anticlockwise, a square spiral with side length $s$ is formed. The diagonals of this spiral contain certain values. What is the side length of the square spiral for which the ratio of primes along both diagonals first falls below 10%?

## Mathematical Development

**Definition 1 (Ulam Spiral Layers).** The spiral is organized into layers indexed by $k = 0, 1, 2, \ldots$, where layer $k = 0$ is the center (containing only the value 1) and layer $k \geq 1$ has side length $s_k = 2k + 1$.

**Theorem 1 (Corner Values).** For layer $k \geq 1$ (side length $s = 2k + 1$), the four diagonal corner values are:

$$\begin{aligned}
c_1(k) &= 4k^2 - 2k + 1 &\quad &\text{(top-right)}, \\
c_2(k) &= 4k^2 + 1 &\quad &\text{(top-left)}, \\
c_3(k) &= 4k^2 + 2k + 1 &\quad &\text{(bottom-left)}, \\
c_4(k) &= (2k+1)^2 = 4k^2 + 4k + 1 &\quad &\text{(bottom-right)}.
\end{aligned}$$

*Proof.* The bottom-right corner of layer $k$ is the last number placed in that layer, namely $(2k+1)^2$. Moving counterclockwise along the outermost ring, consecutive corners are separated by $s - 1 = 2k$ positions. Thus, stepping back from $(2k+1)^2$:

$$\begin{aligned}
\text{Bottom-left:}  & \quad (2k+1)^2 - 2k = 4k^2 + 4k + 1 - 2k = 4k^2 + 2k + 1, \\
\text{Top-left:}     & \quad (2k+1)^2 - 4k = 4k^2 + 1, \\
\text{Top-right:}    & \quad (2k+1)^2 - 6k = 4k^2 - 2k + 1.
\end{aligned}$$

$\square$

**Lemma 1 (Bottom-Right Corner is Always Composite).** For $k \geq 1$, $c_4(k) = (2k+1)^2$ is composite.

*Proof.* $(2k+1)^2$ has the nontrivial factor $2k + 1 \geq 3$, so it is a perfect square greater than 1, hence composite. $\square$

**Theorem 2 (Diagonal Element and Prime Counts).** After processing layers $1$ through $k$:

(i) The total number of diagonal elements is $T(k) = 4k + 1$ (including the center value 1).

(ii) The number of prime diagonal elements is
$$P(k) = \sum_{j=1}^{k} \sum_{i=1}^{3} \mathbf{1}\bigl[c_i(j) \text{ is prime}\bigr],$$
where $\mathbf{1}[\cdot]$ is the indicator function. The fourth corner $c_4(j)$ is excluded by Lemma 1.

*Proof.* (i) Layer 0 contributes 1 element. Each subsequent layer contributes 4 corners. After $k$ layers: $1 + 4k$. (ii) The center value 1 is not prime. At each layer $j$, at most 3 of the 4 corners can be prime (since $c_4$ is always composite). $\square$

**Theorem 3 (The Ratio $P(k)/T(k) \to 0$).** The prime ratio along the diagonals tends to zero as $k \to \infty$.

*Proof.* The corner values at layer $k$ are $\Theta(k^2)$. By the Prime Number Theorem, the probability that a random integer near $N$ is prime is asymptotically $1/\ln N$. For $N = \Theta(k^2)$, this probability is $\sim 1/(2 \ln k)$.

The expected number of primes contributed by layer $j$ is therefore approximately $3/(2 \ln j)$, and

$$P(k) \approx \sum_{j=2}^{k} \frac{3}{2 \ln j} \sim \frac{3k}{2 \ln k}$$

by the integral estimate $\sum_{j=2}^{k} 1/\ln j \sim k/\ln k$ (a consequence of the prime number theorem applied to the sum). The ratio is

$$\frac{P(k)}{4k + 1} \approx \frac{3k/(2 \ln k)}{4k} = \frac{3}{8 \ln k} \xrightarrow{k \to \infty} 0. \qquad \square$$

**Corollary 1 (Existence of Threshold Crossing).** For any $\varepsilon > 0$, there exists a finite $k_0$ such that $P(k_0)/T(k_0) < \varepsilon$. The problem asks for $s = 2k_0 + 1$ where $\varepsilon = 0.10$.

**Lemma 2 (Primality Testing Bound).** To test whether $n = c_i(k)$ is prime by trial division, it suffices to check divisors up to $\lfloor\sqrt{n}\rfloor \leq 2k + 1$.

*Proof.* If $n$ is composite, it has a factor at most $\sqrt{n}$. Since $c_i(k) \leq (2k+1)^2$, we have $\sqrt{c_i(k)} \leq 2k + 1$. $\square$

**Proposition 1 (Rough Estimate via PNT).** Setting $3/(8 \ln k) = 0.10$ gives $\ln k = 3.75$, hence $k \approx 42$, corresponding to side length $s \approx 85$. This is a very rough lower bound; the actual answer is much larger because the PNT approximation overestimates the prime count for small to moderate $k$.

## Algorithm

We process the spiral layer by layer. For each new side length $s = 2k+1$, only the three non-square corner values need to be tested for primality, because the fourth corner is always $(2k+1)^2$ and therefore composite. Maintaining the cumulative number of diagonal entries and diagonal primes allows the prime ratio to be updated after each layer, and the first side length for which this ratio falls below 10% is returned.

## Pseudocode

```text
Algorithm: Spiral Side Length at Which the Prime Ratio Falls Below One Tenth
Require: Primality testing for the diagonal corner values of the spiral.
Ensure: The first odd side length for which the ratio of prime numbers on the diagonals is less than 10%.
1: Initialize prime_count ← 0, diagonal_count ← 1, and k ← 0.
2: Repeat:
3:     Set k ← k + 1 and s ← 2k + 1, then form the three non-square corner values on the layer of side length s.
4:     Test those three corners for primality, update prime_count accordingly, and set diagonal_count ← diagonal_count + 4.
5:     If prime_count / diagonal_count < 0.1, return s.
```

## Complexity Analysis

**Theorem 4 (Time Complexity).** Let $K$ denote the final layer index (so $s = 2K + 1$). With trial division, the algorithm runs in $O(K^2)$ time. With Miller-Rabin primality testing, the time reduces to $O(K \log^2 K)$.

*Proof.* At each layer $k$, we test 3 numbers for primality. Each corner value $n \leq (2K+1)^2$ has $\sqrt{n} = O(K)$. Trial division with the $6k \pm 1$ optimization tests $O(K/\ln K)$ candidates per number in the worst case, but the dominant cost across all $K$ layers is $\sum_{k=1}^{K} O(k) = O(K^2)$, since the trial division bound grows with $k$.

With deterministic Miller-Rabin using a fixed set of witnesses, each primality test costs $O(\log^2 n) = O(\log^2 K)$, giving total cost $3K \cdot O(\log^2 K) = O(K \log^2 K)$.

For the answer $K = 13120$ (side length $s = 26241$), $O(K^2) \approx 1.7 \times 10^8$, which is feasible. $\square$

**Space:** $O(1)$ -- only a constant number of integer variables are maintained.

## Answer

$$\boxed{26241}$$
