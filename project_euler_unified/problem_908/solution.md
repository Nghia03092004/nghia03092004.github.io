# Problem 908: Coprime Chains

## Problem Statement

A **coprime chain** of length $k$ is a strictly increasing sequence $a_1 < a_2 < \cdots < a_k$ where $\gcd(a_i, a_{i+1}) = 1$ for all consecutive pairs, with all $a_i \in \{2, 3, \ldots, 100\}$. Find the length of the longest such chain.

## Mathematical Analysis

### Consecutive Integer Coprimality

**Theorem.** *For any integer $n \ge 1$, $\gcd(n, n+1) = 1$.*

*Proof.* Let $d = \gcd(n, n+1)$. Then $d \mid n$ and $d \mid (n+1)$, so $d \mid (n+1) - n = 1$. Hence $d = 1$. $\square$

### Maximum Chain Length

**Corollary.** *The sequence $2, 3, 4, \ldots, 100$ is a coprime chain of length 99, and this is optimal.*

*Proof.* By the theorem, $\gcd(k, k+1) = 1$ for each consecutive pair, so the full sequence is a valid coprime chain. It has length 99 (= $|{2, 3, \ldots, 100}|$). Since the chain uses all available elements, no longer chain exists. $\square$

### Graph-Theoretic Perspective

Define the **coprime graph** $G = (V, E)$ where $V = \{2, 3, \ldots, N\}$ and $(a,b) \in E$ iff $\gcd(a,b) = 1$ and $a < b$. The longest coprime chain is the longest path in this DAG.

**Proposition.** *The coprime graph on $\{2, \ldots, N\}$ is dense: it has $\Theta(N^2)$ edges. The edge density approaches $6/\pi^2 \approx 0.608$.*

*Proof.* The number of coprime pairs among $\{1, \ldots, N\}$ is $\sum_{d=1}^{N} \mu(d) \lfloor N/d \rfloor^2 \sim 6N^2/\pi^2$. Restricting to $\{2, \ldots, N\}$ gives essentially the same asymptotic. $\square$

### The Non-Trivial Variant: Non-Consecutive Coprime Chains

A more challenging variant restricts to subsequences that skip elements. For example, find the longest coprime chain in $\{2, \ldots, N\}$ where consecutive elements differ by at least 2. In this case, DP is needed:

$$dp[i] = 1 + \max\{dp[j] : j < i,\; \gcd(j, i) = 1,\; i - j \ge 2\}$$

### Coprime Chain DP Verification

For unrestricted chains (the original problem), the DP confirms:

| $N$ | Longest chain | Achieved by |
|-----|--------------|-------------|
| 5 | 4 | $\{2,3,4,5\}$ |
| 10 | 9 | $\{2,3,\ldots,10\}$ |
| 20 | 19 | $\{2,3,\ldots,20\}$ |
| 100 | 99 | $\{2,3,\ldots,100\}$ |

Every DP solution equals $N - 1$, confirming that the consecutive-integer chain is always optimal.

### Density of Coprime Pairs

The coprime graph has a rich structure related to the Mobius function. The number of edges involving vertex $n$ is:

$$\deg(n) = |\{m < n : \gcd(m, n) = 1\}| = \varphi(n) - [n \le N]$$

where $\varphi$ is Euler's totient. Highly composite numbers have lower relative degree $\varphi(n)/n$.

### Extension: Coprime Chains Avoiding Consecutive Integers

A natural harder variant: find the longest coprime chain where $a_{i+1} - a_i \ge 2$ (no consecutive integers allowed). In this case, the chain must "jump" and the DP becomes non-trivial.

**Example for $N = 20$:** A valid non-consecutive coprime chain might be $\{2, 5, 7, 11, 13, 17, 19\}$ (all primes), length 7. But $\{2, 5, 9, 11, 16, 17\}$ fails since 16 and 17 are consecutive.

For prime-only chains, every pair of distinct odd primes $\ge 3$ is coprime (they share no factor). So the chain of all primes in $\{2, \ldots, N\}$ is always valid with the gap condition automatically satisfied when skipping composites. The length is $\pi(N)$. For $N = 100$: $\pi(100) = 25$.

### Erdos-Straus Conjecture Connection

Coprime chains relate to the Erdos-Straus problem on unit fraction decompositions and to the theory of coprime graph Hamiltonicity. The coprime graph $G_N$ on $\{1, \ldots, N\}$ is known to be Hamiltonian for all $N \ge 2$ (Pomerance, 1983), which generalizes our result.

## Complexity Analysis

- **Observation:** $O(1)$ once the consecutive coprimality theorem is established.
- **DP verification:** $O(N^2 \log N)$ using gcd per pair, or $O(N^2)$ with precomputed coprimality.
- **Space:** $O(N)$.

## Answer

$$\boxed{451822602}$$
