# Problem 329: Prime Frog

## Problem Statement

A frog sits on one of the squares numbered 1 to 500, chosen uniformly at random. At each step, the frog jumps to an adjacent square with equal probability (from square 1 it must jump to 2; from square 500 it must jump to 499).

On each square, the frog croaks "P" with probability $\frac{2}{3}$ if the square number is prime and $\frac{1}{3}$ if it is composite; it croaks "N" with the complementary probabilities.

Given that the frog makes 15 croaks producing the sequence "PPPPNNPPPNPPNPN", find the probability of this sequence. Express the answer as a reduced fraction.

## Mathematical Foundation

**Lemma 1 (Transition probabilities).** *Define the transition matrix $P$ on $\{1, 2, \ldots, 500\}$ by*
$$P(i, j) = \begin{cases} 1 & \text{if } i = 1, j = 2 \\ 1 & \text{if } i = 500, j = 499 \\ 1/2 & \text{if } 2 \leq i \leq 499 \text{ and } |i - j| = 1 \\ 0 & \text{otherwise.} \end{cases}$$
*This is a valid stochastic matrix (rows sum to 1).*

**Proof.** Each interior square has exactly two neighbors, each reached with probability $1/2$. The boundary squares have a single forced neighbor. $\square$

**Lemma 2 (Emission probabilities).** *Let $\pi = \{2, 3, 5, 7, 11, \ldots\}$ be the set of primes. For croak symbol $c \in \{P, N\}$ on square $i$:*
$$e(i, P) = \begin{cases} 2/3 & \text{if } i \in \pi \\ 1/3 & \text{if } i \notin \pi \end{cases}, \qquad e(i, N) = 1 - e(i, P).$$

**Proof.** Direct from the problem statement. $\square$

**Theorem 1 (Forward algorithm / Hidden Markov Model).** *The probability of observing the sequence $c_0 c_1 \cdots c_{14}$ is*
$$\Pr(\mathbf{c}) = \sum_{i=1}^{500} \alpha_{14}(i)$$
*where the forward variables are defined by:*
$$\alpha_0(i) = \frac{1}{500} \cdot e(i, c_0), \qquad \alpha_{t+1}(j) = e(j, c_{t+1}) \cdot \sum_{i=1}^{500} P(i,j) \cdot \alpha_t(i).$$

**Proof.** By the law of total probability, summing over all possible paths $(s_0, s_1, \ldots, s_{14})$:
$$\Pr(\mathbf{c}) = \sum_{s_0, \ldots, s_{14}} \frac{1}{500} \prod_{t=0}^{14} e(s_t, c_t) \prod_{t=0}^{13} P(s_t, s_{t+1}).$$
The forward recursion computes this sum efficiently by marginalizing over intermediate states. This is the standard forward algorithm for Hidden Markov Models (Rabiner, 1989). $\square$

**Theorem 2 (Exact rational arithmetic).** *All quantities in the forward algorithm are rational numbers with denominators dividing $500 \cdot 2^{14} \cdot 3^{15}$.*

**Proof.** The initial distribution contributes a factor of $1/500$. Each transition multiplies by $1/2$ (or 1 at boundaries). Each emission multiplies by $1/3$ or $2/3$. Over 15 emissions and 14 transitions, the denominator is bounded by $500 \cdot 2^{14} \cdot 3^{15}$. Since all operations are additions and multiplications of rationals, the result is rational and the denominator divides the stated bound. $\square$

## Editorial
A frog on squares 1..500 croaks P on primes (2/3), N on non-primes (2/3). Find P(sequence PPPPNNPPPNPPNPN) as a reduced fraction. We forward pass. We then iterate over each neighbor i of j. Finally, sum over final states.

## Pseudocode

```text
Initialize alpha[0][i] = (1/500) * e(i, sequence[0])
Forward pass
for each neighbor i of j
Sum over final states
```

## Complexity Analysis

- **Time:** $O(S \cdot T)$ where $S = 500$ is the number of states and $T = 15$ is the sequence length. At each time step, we iterate over all states and their neighbors. Since each state has at most 2 neighbors, total work is $O(500 \cdot 15) = O(7500)$.
- **Space:** $O(S) = O(500)$ for the forward variable array (only two time slices needed).

## Answer

$$\boxed{199740353/29386561536000}$$
