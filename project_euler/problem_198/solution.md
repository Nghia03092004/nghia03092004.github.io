# Problem 198: Ambiguous Numbers

## Problem Statement

A **best rational approximation** to a real number $x$ for denominator bound $d$ is a fraction $r/s$ (fully reduced, $s \le d$) such that $|x - r/s| < |x - p/q|$ for every $p/q$ with $q \le d$ and $q \ne s$.

A real number $x$ is **ambiguous** if for some denominator bound $d$, there are two or more best rational approximations.

Count ambiguous numbers $x = p/q$ (in lowest terms, $q \le 10^8$) in $(0, 1/100)$.

## Mathematical Development

**Theorem 1.** *(Midpoint Characterization.) A rational number $x$ is ambiguous if and only if $x$ is the midpoint of two Farey neighbours. That is, $x = \frac{a/b + c/d}{2}$ where $a/b < c/d$ are adjacent fractions in some Farey sequence $F_D$ (meaning $bc - ad = 1$).*

**Proof.** ($\Rightarrow$) If $x$ is ambiguous, there exists a bound $D$ such that two distinct fractions $r_1/s_1$ and $r_2/s_2$ (both with denominator $\leq D$) are equidistant from $x$ and closer than all others. By the properties of best approximations, these two fractions must be adjacent in $F_D$ (no fraction with denominator $\leq D$ lies between them), and $x$ is their midpoint.

($\Leftarrow$) If $x = (a/b + c/d)/2$ with $bc - ad = 1$, then for $D = \max(b, d) - 1$ (or more precisely, for the largest $D$ such that no Farey fraction lies strictly between $a/b$ and $c/d$), both $a/b$ and $c/d$ are equidistant from $x$, making $x$ ambiguous. $\square$

**Theorem 2.** *(Lowest-Terms Form of Midpoints.) For Farey neighbours $a/b < c/d$ with $bc - ad = 1$, their midpoint is*
$$x = \frac{ad + bc}{2bd} = \frac{2ad + 1}{2bd},$$
*and this fraction is already in lowest terms, i.e., $\gcd(2ad + 1, 2bd) = 1$, so the denominator in lowest terms is $q = 2bd$.*

**Proof.** Since $bc - ad = 1$, we have $ad + bc = ad + (ad + 1) = 2ad + 1$, which is odd. Thus $\gcd(2ad + 1, 2) = 1$.

For $\gcd(2ad + 1, b)$: since $ad \equiv -1 \pmod{b}$ (from $bc - ad = 1$ and $bc \equiv 0 \pmod{b}$), we get $2ad + 1 \equiv -2 + 1 = -1 \pmod{b}$, so $\gcd(2ad + 1, b) = \gcd(-1, b) = 1$.

For $\gcd(2ad + 1, d)$: since $bc \equiv 1 \pmod{d}$, we get $ad + bc \equiv 0 + 1 = 1 \pmod{d}$, so $\gcd(2ad + 1, d) = \gcd(1, d) = 1$.

Therefore $\gcd(2ad + 1, bd) = 1$, and since $\gcd(2ad + 1, 2) = 1$, we have $\gcd(2ad + 1, 2bd) = 1$. $\square$

**Lemma 1.** *(Counting Reduction.) The condition $q \leq N = 10^8$ becomes $bd \leq N/2$. For each coprime pair $(b, d)$ with $b \geq 1$, $d \geq 1$, $bd \leq N/2$, there is a unique $a \in \{0, 1, \ldots, b-1\}$ satisfying $ad \equiv -1 \pmod{b}$ (namely $a = b - d^{-1} \bmod b$). This gives a midpoint $x \in [0, 1)$.*

**Proof.** Since $\gcd(b, d) = 1$, the inverse $d^{-1} \bmod b$ exists and is unique. The value $a = (-d^{-1}) \bmod b$ is the unique solution in $\{0, \ldots, b-1\}$. $\square$

**Lemma 2.** *(Range Constraint.) The midpoint $x = (2ad+1)/(2bd)$ lies in $(0, 1/100)$ if and only if*
$$0 < a < \frac{b}{100} - \frac{1}{2d}.$$
*For $b < 100$, the only possibility is $a = 0$, which gives $x = 1/(2bd) < 1/100$ iff $bd > 50$.*

**Proof.** $x > 0$ requires $a \geq 0$ (with $a = 0$ giving $x = 1/(2bd)$). $x < 1/100$ requires $2ad + 1 < 2bd/100$, i.e., $a < b/100 - 1/(2d)$. For $b < 100$, the bound $b/100 < 1$ forces $a = 0$. $\square$

## Editorial

An ambiguous rational appears exactly at the midpoint of two Farey neighbours, so the search space is much smaller than all fractions with denominator up to `10^8`. The midpoint formula shows that the reduced denominator is always `2bd`, which immediately turns the denominator cap into the product bound `bd <= 5 * 10^7`. After that, the remaining test is whether the unique residue class for `a` places the midpoint below `1/100`.

The implementation splits the count at `B = floor(sqrt(N/2))`. For small `b`, there are many valid `d` values, so it is efficient to fix `b`, enumerate the feasible `a` values, and count matching `d` values in one arithmetic progression determined by a modular inverse. For large `b`, the product bound forces `d` to be small, so the loops are reversed: the code fixes `d`, works with the near-terminal values of the auxiliary parameter in `dr-kb=1`, and counts compatible `b` values in residue classes modulo `d`. Those two passes cover the whole search space without ever scanning every denominator up to `5 * 10^7`.

## Pseudocode

```text
Set HALF = 10^8 / 2 and B = floor(sqrt(HALF)).
Initialize the answer with the special case b = 1, namely all d > 50.

For each small denominator b from 100 up to B:
    compute the largest possible d from HALF / b;
    iterate over the feasible values of a below b / 100;
    skip a when gcd(a, b) is not 1;
    use the modular inverse of a modulo b to determine the residue class for d;
    count how many d in that arithmetic progression satisfy both the range bound
    and the midpoint inequality.

For each small d from 1 up to B:
    compute the largest compatible b from HALF / d;
    iterate over the admissible near-terminal values of the auxiliary parameter m;
    skip m when gcd(m, d) is not 1;
    use the modular inverse of m modulo d to recover the residue class for b;
    count the valid b values in that progression.

Return the combined total.
```

## Complexity Analysis

- **Time:** Cases 2 and 3 each involve $O(\sqrt{N/2})$ outer iterations, with inner work proportional to $O(\sqrt{N})$ total. Overall: $O(N)$ arithmetic operations.
- **Space:** $O(1)$ beyond loop variables and small lookup tables for modular inverses.

## Answer

$$\boxed{52374425}$$
