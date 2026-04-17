# Problem 198: Ambiguous Numbers

## Problem Statement

A **best rational approximation** to a real number $x$ for denominator bound $d$ is a fraction $r/s$ (fully reduced, $s \le d$) such that $|x - r/s| < |x - p/q|$ for every $p/q$ with $q \le d$ and $q \ne s$.

A real number $x$ is **ambiguous** if for some denominator bound $d$, there are two or more best rational approximations.

Count ambiguous numbers $x = p/q$ (in lowest terms, $q \le 10^8$) in $(0, 1/100)$.

## Mathematical Foundation

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

## Algorithm

```
function count_ambiguous(N):
    half_N = N / 2
    count = 0

    # Case 1: b = 1, a = 0, x = 1/(2d), need d > 50
    count += half_N - 50

    # Case 2: b >= 2, small b (b <= sqrt(half_N))
    B = floor(sqrt(half_N))
    for b from 2 to B:
        max_a = floor(b/100 - epsilon)  # largest a < b/100
        if b < 100: max_a = 0
        for a from 0 to max_a:
            if a == 0:
                # d must be coprime to b, d <= half_N / b, d > 50/b (if a=0)
                count += count_valid_d(b, a=0, half_N)
            else:
                if gcd(a, b) != 1: continue  # need gcd(b,d)=1 but also Farey condition
                # d ≡ -a^{-1} (mod b), d <= half_N / b
                count += count_arithmetic_progression(b, a, half_N)

    # Case 3: large b (b > sqrt(half_N))
    # Swap iteration: for each d from 1 to B, iterate over valid b
    for d from 1 to B:
        # For each valid a, count large b in arithmetic progression mod d
        count += count_large_b_case(d, half_N, B)

    return count
```

## Complexity Analysis

- **Time:** Cases 2 and 3 each involve $O(\sqrt{N/2})$ outer iterations, with inner work proportional to $O(\sqrt{N})$ total. Overall: $O(N)$ arithmetic operations.
- **Space:** $O(1)$ beyond loop variables and small lookup tables for modular inverses.

## Answer

$$\boxed{52374425}$$
