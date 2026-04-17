# Problem 776: Digit Sum Division

## Problem Statement

For a positive integer `n`, let `d(n)` be the sum of its decimal digits. Define

$$
F(N)=\sum_{n=1}^{N}\frac{n}{d(n)}.
$$

The given checks are:

$$
F(10)=19,\qquad
F(123)\approx 1.187764610390\mathrm e3,\qquad
F(12345)\approx 4.855801996238\mathrm e6.
$$

We must compute `F(1234567890123456789)` and print it in scientific notation with
12 digits after the decimal point.

## 1. Group by the Digit Sum

For each `s >= 1`, define

$$
T_s(N)=\sum_{\substack{1 \le n \le N \\ d(n)=s}} n.
$$

Then

$$
F(N)=\sum_{s \ge 1}\frac{T_s(N)}{s}.
$$

If `N` has `L` decimal digits, then `d(n) <= 9L`. For our target,

$$
L=19,\qquad 9L=171.
$$

So it is enough to know `T_s(N)` for `1 <= s <= 171`.

## 2. Suffix DP

Fix a nonnegative integer `r`.

- Let `C[r][s]` be the number of `r`-digit strings (leading zeros allowed) whose digits sum to `s`.
- Let `S[r][s]` be the sum of the numeric values of those `r`-digit strings.

For the empty suffix:

$$
C[0][0]=1,\qquad S[0][0]=0.
$$

For `r >= 1`, choose the first digit `x` of the suffix. The remaining `r-1` digits must sum to `s-x`, so:

$$
C[r][s]=\sum_{x=0}^{9} C[r-1][s-x],
$$

where terms with negative index are omitted.

If a suffix starts with digit `x`, then its numeric value is

$$
x \cdot 10^{r-1} + u,
$$

where `u` is the value of the remaining `r-1` digits. Therefore

$$
S[r][s]
=
\sum_{x=0}^{9}
\left(
x \cdot 10^{r-1}\, C[r-1][s-x] + S[r-1][s-x]
\right).
$$

These tables are computed in `O(L^2)` states and a factor `10` for the choice of `x`.

## 3. Scan the Prefix of `N`

Write the decimal expansion of `N` as

$$
a_0 a_1 \dots a_{L-1}.
$$

Process the digits from left to right. Suppose we have already fixed a prefix:

- its numeric value is `P`,
- its digit sum is `\sigma`.

Now consider position `i`, with current limit digit `a_i`, and let `r=L-1-i` be the number of remaining digits.

For each smaller digit `x < a_i`, every number formed by choosing `x` here and then any legal suffix `u` of length `r` is

$$
(10P+x)\,10^r + u.
$$

If the suffix has digit sum `t`, then the full number has digit sum `\sigma + x + t`, and the contribution of all such numbers to `T_{\sigma+x+t}(N)` is

$$
\bigl((10P+x)10^r\bigr)\, C[r][t] + S[r][t].
$$

After summing these contributions for every position and every `x < a_i`, we continue with the actual digit `a_i`, updating

$$
P \leftarrow 10P + a_i,\qquad
\sigma \leftarrow \sigma + a_i.
$$

At the end, the exact number `N` itself is added to `T_{d(N)}(N)`.

This gives every integer in `[1,N]` exactly once: it is counted at the first position where it is smaller than `N`, or as the final exact endpoint `N`.

## 4. Correctness

**Lemma 1.** `C[r][s]` equals the number of `r`-digit suffixes with digit sum `s`, and `S[r][s]` equals the sum of their numeric values.

*Proof.* The statement is true for `r=0`. For `r>=1`, partition the suffixes by their first digit `x`. The remaining `r-1` digits then contribute exactly the recurrences above for both the count and the sum of values. By induction on `r`, the claim follows. $\square$

**Lemma 2.** During the prefix scan, the formula

$$
\bigl((10P+x)10^r\bigr)\, C[r][t] + S[r][t]
$$

is exactly the sum of all numbers whose processed prefix becomes `10P+x` and whose remaining `r` digits have digit sum `t`.

*Proof.* There are `C[r][t]` such suffixes, and each contributes the common prefix term `(10P+x)10^r`. The suffix values themselves sum to `S[r][t]` by Lemma 1. Adding these two parts gives the stated formula. $\square$

**Lemma 3.** For every `s >= 1`, the algorithm computes `T_s(N)` exactly.

*Proof.* Every `1 <= n < N` has a first position where its digit is smaller than the corresponding digit of `N`; at that position the algorithm counts it in exactly one branch `x < a_i`. The final endpoint `N` is added separately. By Lemma 2, each branch contributes the correct sum to the correct digit-sum bucket, so each `T_s(N)` is exact. $\square$

**Theorem.** The algorithm returns the correct value of `F(N)`.

*Proof.* By Lemma 3, the algorithm computes all values `T_s(N)` exactly. Since

$$
F(N)=\sum_{s \ge 1}\frac{T_s(N)}{s},
$$

summing these exact buckets divided by `s` gives the exact value of `F(N)`. $\square$

## 5. Complexity Analysis

Let `L` be the number of decimal digits of `N`.

- The DP tables contain `O(L \cdot 9L) = O(L^2)` states, each with at most `10` transitions.
- The prefix scan also visits `O(L^2)` digit-sum states.

Hence:

- **Time:** `O(L^2)`.
- **Space:** `O(L^2)`.

For `L=19`, this is tiny.

## Answer

The computation gives

$$
\boxed{9.627509725002e33}
$$
