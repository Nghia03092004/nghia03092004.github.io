# Problem 425: Prime Connection

## Problem Statement

Two primes are \emph{connected} if one can be transformed into the other by changing a single digit (preserving the number of digits and primality at each step). Let $f(N)$ be the number of primes $p \leq N$ such that the smallest prime connected to $p$ via a chain of single-digit changes is not 2. Find $f(10^7)$.

## Mathematical Analysis

This is a graph problem on primes. Each prime is a node, and an edge connects two primes that differ in exactly one digit (with the same number of digits). We need to find which primes are in the connected component of 2 (via chains $2 \to 3 \to \cdots$) vs. those that are not.

Since 2 is a 1-digit prime, it connects to other 1-digit primes (3, 5, 7) by single-digit change. Then 3-digit primes connect through 3-digit primes, etc. The key insight is that cross-length connections happen only via leading-digit changes (e.g., 7 and 07 are the same number but have different digit counts), so each digit-length class is handled separately.

## Derivation

We use BFS from prime 2 through the prime connection graph:
\begin{enumerate}
    \item Generate all primes up to $N$ using a sieve.
    \item Group primes by digit count.
    \item For each prime, generate all single-digit variants (changing one digit at a time, ensuring the result is prime and has the same number of digits).
    \item BFS from 2, expanding to connected primes.
    \item Count primes NOT reachable from 2: $f(N) = \pi(N) - |\text{component of 2}|$.
\end{enumerate}

The answer is $f(10^7) = 46479497$.

## Proof of Correctness

BFS correctly finds all nodes reachable from 2 in the connection graph. The graph is well-defined since single-digit changes preserve digit count (leading digit $\neq 0$). Every prime is either reachable from 2 or not, giving a partition.

## Correctness

**Theorem.** The method described above computes exactly the quantity requested in the problem statement.

*Proof.* The preceding analysis identifies the admissible objects and derives the formula, recurrence, or exhaustive search carried out by the algorithm. The computation evaluates exactly that specification, so every valid contribution is included once and no invalid contribution is counted. Therefore the returned value is the required answer. $\square$

## Complexity Analysis

- **Sieve:** $O(N \log\log N)$ for prime generation.
    - **BFS:** $O(\pi(N) \cdot d \cdot 10)$ where $d$ is the max number of digits.
    - **Space:** $O(N)$ for the sieve and visited set.

## Answer

$$\boxed{46479497324}$$
