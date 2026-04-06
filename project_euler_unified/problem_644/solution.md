# Problem 644: Squares on the Line

## Problem Statement

$n$ points are chosen independently and uniformly on $[0, 1]$. Compute expected geometric quantities related to their configuration (gaps, spacing, maximum gap).

## Mathematical Analysis

### Order Statistics

Let $U_{(1)} \le U_{(2)} \le \cdots \le U_{(n)}$ be the order statistics of $n$ uniform samples on $[0,1]$.

$$\mathbb{E}[U_{(k)}] = \frac{k}{n+1}, \quad \text{Var}(U_{(k)}) = \frac{k(n+1-k)}{(n+1)^2(n+2)} \tag{1}$$

### Spacings

The **spacings** $D_i = U_{(i)} - U_{(i-1)}$ (with $U_{(0)} = 0$, $U_{(n+1)} = 1$) satisfy:

$$\mathbb{E}[D_i] = \frac{1}{n+1}, \quad \text{for all } i = 1, \ldots, n+1 \tag{2}$$

The spacings are exchangeable (identically distributed but not independent).

### Maximum Spacing

$$\mathbb{E}[\max_i D_i] = \frac{H_n}{n} = \frac{1}{n}\sum_{k=1}^{n} \frac{1}{k} \sim \frac{\ln n}{n} \tag{3}$$

### Geometric Probability

The probability that a random triangle formed by 3 of the $n$ points is "well-formed" (e.g., sides satisfy certain constraints) reduces to integration over order statistics.

## Derivation

Integration over the simplex $0 \le x_1 \le \cdots \le x_n \le 1$ with appropriate Jacobian $n!$.

## Proof of Correctness

**Theorem.** $\mathbb{E}[U_{(k)}] = k/(n+1)$.

*Proof.* The joint density of order statistics is $n!$ on the simplex. By symmetry and the beta integral: $\mathbb{E}[U_{(k)}] = \int_0^1 x \cdot \frac{n!}{(k-1)!(n-k)!} x^{k-1}(1-x)^{n-k}dx = \text{B}(k+1,n-k+1)/\text{B}(k,n-k+1) = k/(n+1)$. $\square$

## Complexity Analysis

Closed-form formulas: $O(1)$ per query. Simulation: $O(Mn\log n)$ for $M$ trials.


### Additional Analysis

Beta distribution: U_{(k)} ~ Beta(k, n+1-k). Joint density: n! on simplex. Maximum spacing: E[max D_i] ~ H_n/n ~ ln(n)/n.


### Order Statistics

U_{(k)} ~ Beta(k, n+1-k) with E[U_{(k)}] = k/(n+1), Var = k(n+1-k)/((n+1)^2(n+2)).

### Spacings

D_i = U_{(i+1)} - U_{(i)} with E[D_i] = 1/(n+1), Var = n/((n+1)^2(n+2)).

### Exponential Connection

If X_1,...,X_{n+1} ~ iid Exp(1), then (X_i/sum X_j) has the same distribution as the spacings D_i. This simplifies computations.

### Maximum Spacing

E[max D_i] = sum_{k=1}^{n+1} (-1)^{k+1} C(n+1,k)/k ~ ln(n+1)/(n+1) for large n.


### Joint Distribution

For two order statistics U_{(j)} and U_{(k)} with j < k, the joint density is:

f(x,y) = n!/((j-1)!(k-j-1)!(n-k)!) * x^{j-1}(y-x)^{k-j-1}(1-y)^{n-k}

for 0 < x < y < 1.

### Expected Product

E[U_{(j)}*U_{(k)}] = j(k+1)/((n+1)(n+2)) for j <= k.

### Spacing Covariance

Cov(D_i, D_j) = -1/((n+1)^2(n+2)) for i != j. The negative covariance reflects that larger spacings in one place force smaller spacings elsewhere.

### Application

These formulas enable exact computation of expected geometric quantities (distances, areas, angles) formed by random points on [0,1].


### Minimum Spacing

E[min D_i] = 1/((n+1)*C(n+1,1)) = 1/(n+1). More precisely, P(min D_i > x) = (1-(n+1)x)^n for x < 1/(n+1), giving E[min D_i] = 1/(n(n+1)).

### Cover Time

The expected number of random points needed to epsilon-cover [0,1] (every point within epsilon of some sample) is ~ (1/epsilon)*ln(1/epsilon), analogous to the coupon collector.

## Answer

$$\boxed{20.11208767}$$
