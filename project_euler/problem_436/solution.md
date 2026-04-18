# Problem 436: Unfair Wager

## Problem Statement

Julie proposes a wager to her sister Louise. They use a generator of independent random numbers uniformly distributed on $(0, 1)$. Starting with $S = 0$:

- **Louise** adds random numbers to $S$ until $S > 1$, and records her last drawn number as $x$.
- **Julie** continues adding random numbers to $S$ until $S > 2$, and records her last drawn number as $y$.
- Julie wins if $y > x$.

Find $P(y > x)$ rounded to 10 decimal places.

## Mathematical Foundation

**Theorem 1 (Irwin-Hall Distribution).** Let $U_1, U_2, \ldots$ be i.i.d. $\mathrm{Uniform}(0,1)$ and $S_n = \sum_{i=1}^n U_i$. The CDF of $S_n$ on $[0, n]$ is
$$P(S_n \leq t) = \frac{1}{n!} \sum_{k=0}^{\lfloor t \rfloor} (-1)^k \binom{n}{k} (t - k)^n.$$

**Proof.** By induction on $n$. For $n = 1$, $P(U_1 \leq t) = t$ for $t \in [0,1]$, which matches the formula. The inductive step follows from the convolution $f_{S_{n+1}}(t) = \int_0^1 f_{S_n}(t - u)\,du$ and termwise integration of the polynomial expression. $\square$

**Lemma 1 (First Passage Probability).** $P(S_n \leq 1) = 1/n!$ for all $n \geq 0$.

**Proof.** Setting $t = 1$ in Theorem 1: for $0 \leq t \leq 1$, the only term in the sum is $k = 0$ (since $\lfloor 1 \rfloor = 1$ but $k = 1$ contributes $(1-1)^n = 0$ for $n \geq 1$), giving $P(S_n \leq 1) = 1 \cdot 1^n / n! = 1/n!$. $\square$

**Theorem 2 (Stopping Time Distribution).** Let $N_1 = \min\{n : S_n > 1\}$. Then
$$P(N_1 = n) = \frac{1}{(n-1)!} - \frac{1}{n!} = \frac{n-1}{n!}$$
for $n \geq 2$, and $P(N_1 = 1) = 0$ (since $U_1 \leq 1$ a.s. when $U_1 \sim \mathrm{Uniform}(0,1)$; here we use the open interval, so $P(N_1 = 1) = 0$).

**Proof.** $P(N_1 = n) = P(S_{n-1} \leq 1) - P(S_n \leq 1) = 1/(n-1)! - 1/n! = (n-1)/n!$. $\square$

**Theorem 3 (Joint Density).** Conditional on $N_1 = n$, the sum $S_{n-1}$ is uniformly distributed on $[0, 1]$ (after rescaling by the conditional probability), and the overshoot $x = U_{N_1}$ satisfies $x > 1 - S_{n-1}$. The joint density of $(S_{N_1}, x)$ can be expressed as:
$$f_{S_{N_1}, x}(s, x) = \sum_{n=2}^{\infty} f_{S_n, U_n}(s, x \mid N_1 = n) \cdot P(N_1 = n),$$
where $s = S_{N_1} \in (1, 2)$ and $x \in (0, 1)$ with $x > s - 1$.

**Proof.** For fixed $n$, the density of $S_{n-1} = s - x$ conditional on $S_{n-1} \leq 1$ and $S_{n-1} + x > 1$ (with $x \sim \mathrm{Uniform}(0,1)$ independent of $S_{n-1}$) is obtained from the Irwin-Hall density restricted to $[0,1]$. Marginalizing over $n$ gives the stated formula. $\square$

**Theorem 4 (Winning Probability).** The probability $P(y > x)$ is computed by integrating over the joint distribution of Louise's state $(S_{N_1}, x)$ and then computing, for each such state, the probability that Julie's last draw $y$ exceeds $x$, given that Julie starts from $S_{N_1}$ and stops when $S > 2$.

**Proof.** By the law of total probability:
$$P(y > x) = \int P(y > x \mid S_{N_1} = s, \, x = u) \, f_{S_{N_1}, x}(s, u) \, ds\, du.$$
The inner probability depends on the distribution of the stopping value for the sum starting at $s$ exceeding 2, which is itself an Irwin-Hall stopping problem shifted by $s$. The integral is evaluated numerically by truncating the series at sufficiently large $N_{\max}$ and using high-precision quadrature. $\square$

## Editorial
Julie proposes a wager: they add uniform random numbers from [0,1]. Louise adds until sum > 1, records last number x. Julie continues adding until sum > 2, records last number y. Julie wins if y > x. Find P(Julie wins). We use Monte Carlo simulation for verification and numerical integration for the exact answer. We iterate over each configuration (n1, n2) where Louise uses n1 draws, Julie uses n2 - n1. Finally, where S_{n1-1} <= 1, S_{n1} > 1, S_{n2-1} <= 2, S_{n2} > 2.

## Pseudocode

```text
Precompute Irwin-Hall densities for n = 1 to N_max
For each configuration (n1, n2) where Louise uses n1 draws, Julie uses n2 - n1:
Integrate over all valid (u_1, ..., u_{n2}) configurations
where S_{n1-1} <= 1, S_{n1} > 1, S_{n2-1} <= 2, S_{n2} > 2
and u_{n2} > u_{n1}
```

## Complexity Analysis

- **Time:** $O(N_{\max}^2 \cdot Q)$ where $Q$ is the cost of each numerical quadrature evaluation. With $N_{\max} = O(1)$ (truncation) and adaptive quadrature, this is effectively $O(1)$ with a large constant depending on precision.
- **Space:** $O(1)$.

## Answer

$$\boxed{0.5276662759}$$
