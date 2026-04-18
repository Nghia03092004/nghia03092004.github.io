# Problem 781: Feynman Path Integral

## Problem Statement

Consider a lattice path on $\mathbb{Z}^2$ from $(0,0)$ to $(n,n)$ using steps $(1,0)$ and $(0,1)$. Define the **area** under a path as the number of unit squares below it. Let $Z(n) = \sum_{\gamma} e^{i\pi \cdot \text{area}(\gamma) / n^2}$ where the sum runs over all $\binom{2n}{n}$ lattice paths $\gamma$. Find $\lfloor |Z(10^6)| \rfloor$.

## Mathematical Foundation

**Lemma 1.** *The number of lattice paths from $(0,0)$ to $(n,n)$ with area exactly $k$ below the path equals the Gaussian binomial coefficient entry, i.e., $|\{\gamma : \text{area}(\gamma) = k\}| = [q^k]\binom{2n}{n}_q$ evaluated as a polynomial in $q$.*

**Proof.** Each lattice path from $(0,0)$ to $(n,n)$ using $n$ east steps and $n$ north steps can be encoded as a binary string of length $2n$ with $n$ ones. The area below such a path equals the number of inversions: if the east steps occur at positions $1 \le a_1 < a_2 < \cdots < a_n \le 2n$ and north steps at the remaining positions, the area equals $\sum_{i=1}^{n}(a_i - i)$. This is precisely the $q$-analog weight whose generating function is the Gaussian binomial coefficient $\binom{2n}{n}_q = \frac{[2n]_q!}{[n]_q!^2}$. $\square$

**Theorem 1.** *Let $\omega = e^{i\pi/n^2}$. Then*
$$Z(n) = \binom{2n}{n}_{q=\omega} = \prod_{k=1}^{n} \frac{1 - \omega^{n+k}}{1 - \omega^k}.$$

**Proof.** By Lemma 1, $Z(n) = \sum_k |\{\gamma : \text{area}(\gamma) = k\}| \cdot \omega^k = \binom{2n}{n}_{q=\omega}$. The product formula for the Gaussian binomial coefficient gives
$$\binom{2n}{n}_q = \prod_{k=1}^{n}\frac{1 - q^{n+k}}{1 - q^k},$$
valid as a polynomial identity and hence for any complex $q$, provided the denominator factors are nonzero. Since $\omega = e^{i\pi/n^2}$ has order $2n^2$ and $k \le n < 2n^2$, we have $\omega^k \ne 1$ for $1 \le k \le n$. $\square$

**Lemma 2.** *For $|Z(n)|$ we have*
$$|Z(n)| = \prod_{k=1}^{n} \frac{|\sin(\pi(n+k)/(2n^2))|}{|\sin(\pi k/(2n^2))|}.$$

**Proof.** Using $|1 - e^{i\theta}| = 2|\sin(\theta/2)|$, we get $|1 - \omega^m| = 2|\sin(m\pi/(2n^2))|$. Substituting into the product formula from Theorem 1 yields the result. $\square$

## Editorial
We enumerate the admissible parameter range, discard candidates that violate the derived bounds or arithmetic constraints, and update the final set or total whenever a candidate passes the acceptance test.

## Pseudocode

```text
    log_mag = 0
    For k from 1 to n:
        num_angle = pi * (n + k) / (2 * n * n)
        den_angle = pi * k / (2 * n * n)
        log_mag += log(|sin(num_angle)|) - log(|sin(den_angle)|)
    Return floor(exp(log_mag))
```

## Complexity Analysis

- **Time:** $O(n)$ -- a single loop of $n$ iterations, each computing a constant number of trigonometric and logarithmic operations.
- **Space:** $O(1)$ -- only a running accumulator for the log-magnitude.

## Answer

$$\boxed{162450870}$$
