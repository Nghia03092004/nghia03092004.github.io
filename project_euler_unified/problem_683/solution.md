# Problem 683: The Chase II

## Problem Statement

$n$ players sit around a circular table. Two dice are placed randomly with two distinct players. Each turn, both die-holders simultaneously roll a fair die: outcomes 1--2 pass the die left, 3--4 keep it, 5--6 pass right. A round ends when one player holds both dice; that player is eliminated and pays $s^2$ into the pot, where $s$ is the number of turns in the round. The game continues (with dice re-placed randomly among remaining players) until one player remains with the pot.

Find the expected total pot for $n = 100$ players, rounded to 10 decimal places.

## Mathematical Foundation

**Theorem 1 (Gap Reduction).** By circular symmetry, the state of a round with $m$ players is fully described by the gap $g = (d_2 - d_1) \bmod m \in \{1, 2, \ldots, m-1\}$ between the two die positions. The gap evolves as

$$g_{t+1} \equiv g_t + \Delta \pmod{m}, \quad \Delta = \delta_2 - \delta_1,$$

where each $\delta_i \in \{-1, 0, +1\}$ independently with probability $1/3$ each.

**Proof.** Player positions are labeled $0, 1, \ldots, m-1$ modulo $m$. Die 1 at position $d_1$ moves to $d_1 + \delta_1 \pmod{m}$ and die 2 to $d_2 + \delta_2 \pmod{m}$. The gap $(d_2 - d_1) \bmod m$ changes by $\delta_2 - \delta_1 \pmod{m}$. The initial positions are irrelevant; only the gap matters. $\square$

**Lemma 1 (Distribution of $\Delta$).** The difference $\Delta = \delta_2 - \delta_1$ has distribution:

| $\Delta$ | $-2$ | $-1$ | $0$ | $+1$ | $+2$ |
|----------|------|------|-----|------|------|
| $\Pr$    | $1/9$ | $2/9$ | $3/9$ | $2/9$ | $1/9$ |

**Proof.** Direct convolution of two uniform $\{-1,0,1\}$ distributions. $\square$

**Theorem 2 (Circulant Eigenvalues).** The transition matrix $Q$ on the non-absorbing states $\{1, \ldots, m-1\}$ is circulant. Its eigenvalues are

$$\lambda_k = \frac{1}{9}\bigl(3 + 4\cos(2\pi k/m) + 2\cos(4\pi k/m)\bigr), \quad k = 1, \ldots, m-1.$$

**Proof.** The transition probability from gap $g$ to gap $g'$ depends only on $(g'-g) \bmod m$, so $Q$ is circulant. For a circulant matrix with first-row entries $c_j$ (where $c_j = \Pr[\Delta \equiv j \pmod{m}]$), the eigenvalues are $\lambda_k = \sum_{j} c_j \, \omega^{jk}$ where $\omega = e^{2\pi i/m}$. Substituting the $\Delta$-distribution:

$$\lambda_k = \frac{1}{9}\bigl(e^{-2\cdot 2\pi ik/m} + 2e^{-2\pi ik/m} + 3 + 2e^{2\pi ik/m} + e^{2\cdot 2\pi ik/m}\bigr) = \frac{1}{9}\bigl(3 + 4\cos(2\pi k/m) + 2\cos(4\pi k/m)\bigr).$$

$\square$

**Theorem 3 (Expected $s^2$ via Fundamental Matrix).** Let $N = (I - Q)^{-1}$ be the fundamental matrix. Then:
- $\mathbf{E}[s \mid g] = (N\mathbf{1})_g$,
- $\mathbf{E}[s^2 \mid g] = ((2N - I)N\mathbf{1})_g$.

For a circulant $Q$, these reduce to spectral formulae:

$$\mathbf{E}[s^2 \mid g] = \frac{1}{m-1}\sum_{k=1}^{m-1} \frac{1+\lambda_k}{(1-\lambda_k)^2}\, e^{2\pi i k g/m}.$$

**Proof.** Standard absorbing Markov chain theory: if $T$ is the absorption time, $E[T] = N\mathbf{1}$ and $E[T^2] = (2N-I)N\mathbf{1}$. The spectral decomposition of the circulant $N = (I-Q)^{-1}$ diagonalizes via DFT: $N$ has eigenvalues $1/(1-\lambda_k)$, and $(2N-I)N$ has eigenvalues $(1+\lambda_k)/(1-\lambda_k)^2$. The formula follows from the inverse DFT. $\square$

**Theorem 4 (Expected Round Cost).** In a round with $m$ players and dice placed uniformly at random among distinct pairs, the expected cost is

$$C(m) = \frac{1}{m-1}\sum_{g=1}^{m-1} \mathbf{E}[s^2 \mid g] = \frac{1}{(m-1)^2}\sum_{k=1}^{m-1}\frac{1+\lambda_k}{(1-\lambda_k)^2} \cdot \sum_{g=1}^{m-1} e^{2\pi i kg/m}.$$

Since $\sum_{g=1}^{m-1} e^{2\pi i kg/m} = -1$ for $k \ne 0 \pmod{m}$, this simplifies to

$$C(m) = \frac{-1}{(m-1)^2}\sum_{k=1}^{m-1}\frac{1+\lambda_k}{(1-\lambda_k)^2}.$$

**Proof.** The initial gap is uniform on $\{1,\ldots,m-1\}$. Averaging $E[s^2|g]$ and using the geometric sum formula for roots of unity yields the result. $\square$

**Corollary.** The expected total pot is

$$E[\text{Pot}] = \sum_{m=2}^{n} C(m).$$

## Algorithm

```
function expected_pot(n):
    total = 0
    for m = 2 to n:
        cost = 0
        for k = 1 to m - 1:
            lam = (3 + 4*cos(2*pi*k/m) + 2*cos(4*pi*k/m)) / 9
            cost += (1 + lam) / (1 - lam)^2
        cost = -cost / (m - 1)^2
        total += cost
    return total
```

## Complexity Analysis

- **Time:** $O\!\left(\sum_{m=2}^{n}(m-1)\right) = O(n^2)$.
- **Space:** $O(1)$ (no matrices stored; eigenvalues computed on-the-fly).

## Answer

$$\boxed{2.38955315e11}$$
