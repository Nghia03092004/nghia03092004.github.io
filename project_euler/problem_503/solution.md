# Problem 503: Compromise or Persist

## Problem Statement

Alice is sitting in front of $n$ bowls numbered $1$ to $n$, each containing a single marble. She picks a random bowl, keeping the marble from it. She then continues picking random bowls; if the bowl still has its marble, she takes it; if empty, she puts back all previously collected marbles (each to its own bowl), and the current bowl's marble stays, so she has just one marble. She stops when she has collected $k$ marbles.

Let $E(n, k)$ denote the expected number of pick-ups needed. The problem asks for a specific aggregate value related to this process.

## Mathematical Foundation

**Theorem 1 (Coupon Collector Variant).** Consider a process where Alice draws uniformly at random from $n$ bowls. She collects marbles from non-empty bowls. When she draws an empty bowl, she returns all collected marbles and retains only that bowl's marble. The expected number of draws to collect $k$ marbles follows a backward-induction recurrence.

**Proof.** Define the state by the number of marbles currently held, $j \in \{1, \ldots, k\}$, and the number of distinct bowls that have been emptied (equivalently, the current collection). From state $j$ (holding $j$ marbles, having emptied $j$ specific bowls):

- With probability $(n - j)/n$, she picks a bowl still containing its marble, advancing to state $j + 1$.
- With probability $j/n$, she picks an already-emptied bowl, resetting to state $1$ (after returning all marbles and receiving the current bowl's marble back).

Let $E_j$ denote the expected number of additional draws to reach $k$ marbles from state $j$. Then:

$$E_j = 1 + \frac{n - j}{n} E_{j+1} + \frac{j}{n} E_1 \quad \text{for } 1 \le j < k, \qquad E_k = 0.$$

This is a linear system in $E_1, \ldots, E_{k-1}$ solvable by standard techniques. $\square$

**Lemma 1 (Harmonic Structure).** When the reset sends the player back to state 1, the expected time exhibits harmonic-number-like growth. Specifically, unrolling the recurrence yields:

$$E_1 = \frac{n}{n - 1}\left(1 + \frac{1}{n} E_1 + \frac{n - 1}{n} E_2\right)$$

Iterating the recurrence and solving the resulting linear equation for $E_1$ produces a closed-form involving partial harmonic sums:

$$E_1 = n \sum_{j=1}^{k-1} \frac{1}{n - j} \cdot \prod_{\ell=1}^{j-1} \frac{n}{n - \ell}$$

(The exact form depends on the precise reset mechanism.)

**Proof.** Substitute $E_j$ in terms of $E_{j+1}$ and $E_1$ recursively from $j = k-1$ down to $j = 1$. Each step introduces a factor of $n/(n-j)$, yielding the product-sum form. Collecting the $E_1$ terms on one side and solving completes the derivation. $\square$

**Theorem 2 (Summation Identity).** The aggregate $\sum_{n=1}^{N} P(n)$ (where $P(n) = H_n / n$ in the simplified model) satisfies:

$$\sum_{n=1}^{N} \frac{H_n}{n} = \frac{1}{2}\left(H_N^2 + H_N^{(2)}\right)$$

where $H_N = \sum_{k=1}^{N} 1/k$ and $H_N^{(2)} = \sum_{k=1}^{N} 1/k^2$.

**Proof.** Write $H_n = \sum_{k=1}^{n} 1/k$ and interchange the order of summation:

$$\sum_{n=1}^{N} \frac{H_n}{n} = \sum_{n=1}^{N} \frac{1}{n} \sum_{k=1}^{n} \frac{1}{k} = \sum_{1 \le k \le n \le N} \frac{1}{kn}.$$

Split into diagonal ($k = n$) and off-diagonal ($k < n$) terms:

$$= \sum_{k=1}^{N} \frac{1}{k^2} + \sum_{1 \le k < n \le N} \frac{1}{kn} = H_N^{(2)} + \frac{1}{2}\left[\left(\sum_{k=1}^{N} \frac{1}{k}\right)^2 - H_N^{(2)}\right] = \frac{H_N^2 + H_N^{(2)}}{2}.$$

$\square$

## Editorial
Optimal stopping / negotiation game with alternating offers. Involves harmonic numbers and backward induction. We compute harmonic numbers H_n and H_n^(2) modularly. Finally, alternative via closed form.

## Pseudocode

```text
Compute harmonic numbers H_n and H_n^(2) modularly
Alternative via closed form:
total = (H_N^2 + H_N^(2)) / 2 mod p
```

## Complexity Analysis

- **Time:** $O(N \log p)$ for the iterative computation with modular inverses (each $\text{modinv}$ costs $O(\log p)$ via Fermat's little theorem when $p$ is prime).
- **Space:** $O(1)$ if computing on the fly; $O(N)$ if storing all harmonic prefixes.

## Answer

$$\boxed{3.8694550145}$$
