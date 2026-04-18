# Problem 33: Digit Cancelling Fractions

## Problem Statement

The fraction $49/98$ is a curious fraction, because an inexperienced mathematician might incorrectly simplify it by cancelling the 9s, obtaining $49/98 = 4/8$, which happens to be correct.

We shall consider fractions like $30/50 = 3/5$ as trivial examples. There are exactly four non-trivial examples of this type of fraction, less than one in value, containing two digits in the numerator and denominator. If the product of these four fractions is given in its lowest common terms, find the value of the denominator.

## Mathematical Development

**Definition 1.** Let $n = 10a + b$ and $d = 10c + e$ be two-digit positive integers with $a, c \in \{1, \ldots, 9\}$ and $b, e \in \{0, \ldots, 9\}$. The fraction $n/d$ (with $n < d$) is a *non-trivial digit-cancelling fraction* if there exist a shared non-zero digit between $n$ and $d$ such that removing one occurrence of this digit from both the numerator and the denominator yields a single-digit fraction equal to $n/d$.

**Definition 2.** For a fraction $n/d$ with $n = 10a + b$ and $d = 10c + e$, the four *cancellation patterns* are:

| Pattern | Shared digit | Cancelled pair | Reduced fraction |
|---------|-------------|----------------|------------------|
| P1 | $b = c$ | units of $n$, tens of $d$ | $a/e$ |
| P2 | $a = e$ | tens of $n$, units of $d$ | $b/c$ |
| P3 | $a = c$ | tens of both | $b/e$ |
| P4 | $b = e$ | units of both | $a/c$ |

**Theorem 1 (Classification of Pattern P1).** *Under pattern P1 ($b = c \ne 0$, $e \ne 0$), the equation $\frac{10a + b}{10b + e} = \frac{a}{e}$ holds if and only if*
$$e = \frac{10ab}{9a + b}.$$
*The complete set of solutions with $a, b \in \{1, \ldots, 9\}$, $e \in \{1, \ldots, 9\}$, and $10a + b < 10b + e$ is:*
$$\{(a, b, e)\} = \{(1, 6, 4),\; (1, 9, 5),\; (2, 6, 5),\; (4, 9, 8)\}.$$

**Proof.** Cross-multiplying the equation $\frac{10a+b}{10b+e} = \frac{a}{e}$ yields
$$e(10a + b) = a(10b + e) \implies 10ae + be = 10ab + ae \implies 9ae = b(10a - e).$$
Solving for $e$: $9ae + be = 10ab$, so $e(9a + b) = 10ab$, giving $e = \frac{10ab}{9a + b}$.

We require: (i) $e \in \mathbb{Z}$, i.e., $(9a + b) \mid 10ab$; (ii) $1 \le e \le 9$; (iii) $10a + b < 10b + e$, i.e., the fraction is less than 1.

Exhaustive verification over all $81$ pairs $(a, b) \in \{1, \ldots, 9\}^2$:
- $(1, 6)$: $e = 60/15 = 4$. Check: $16/64 = 1/4 = 0.25$. Condition $16 < 64$. Valid.
- $(1, 9)$: $e = 90/18 = 5$. Check: $19/95 = 1/5 = 0.2$. Condition $19 < 95$. Valid.
- $(2, 6)$: $e = 120/24 = 5$. Check: $26/65 = 2/5 = 0.4$. Condition $26 < 65$. Valid.
- $(4, 9)$: $e = 360/45 = 8$. Check: $49/98 = 4/8 = 1/2$. Condition $49 < 98$. Valid.

All other pairs either fail the integrality condition, or produce $e \notin [1, 9]$, or violate $n < d$. $\square$

**Theorem 2 (Completeness).** *Patterns P2, P3, and P4 yield no additional non-trivial solutions beyond those found in Theorem 1.*

**Proof.** For each pattern, derive the analogous Diophantine condition:

*Pattern P2* ($a = e \ne 0$, $c \ne 0$): $\frac{10a + b}{10c + a} = \frac{b}{c}$ yields $c(10a + b) = b(10c + a)$, giving $10ac + bc = 10bc + ab$, hence $a(10c - b) = 9bc$, so $a = \frac{9bc}{10c - b}$. Exhaustive check over $(b, c) \in \{1, \ldots, 9\}^2$: no solution satisfies $a \in \{1, \ldots, 9\}$ (integer) with $10a + b < 10c + a$ that is not already equivalent to a P1 solution under the mapping $(n, d) \mapsto (n, d)$.

*Pattern P3* ($a = c$, $e \ne 0$): $\frac{10a + b}{10a + e} = \frac{b}{e}$ yields $e(10a + b) = b(10a + e)$, giving $10ae = 10ab$, hence $e = b$. But $e = b$ and $a = c$ implies $n = d$, contradicting $n < d$.

*Pattern P4* ($b = e$, $c \ne 0$): $\frac{10a + b}{10c + b} = \frac{a}{c}$ yields $c(10a + b) = a(10c + b)$, giving $cb = ab$, hence $c = a$ (for $b \ne 0$) or $b = 0$ (trivial case excluded). If $c = a$, then $n = d$, a contradiction. If $b = 0$, the cancellation is trivial.

Thus no new solutions arise from patterns P2--P4. $\square$

**Theorem 3 (Product computation).** *The product of the four digit-cancelling fractions, reduced to lowest terms, equals $1/100$.*

**Proof.** Using the simplified forms from Theorem 1:
$$\frac{16}{64} \times \frac{19}{95} \times \frac{26}{65} \times \frac{49}{98} = \frac{1}{4} \times \frac{1}{5} \times \frac{2}{5} \times \frac{1}{2} = \frac{1 \cdot 1 \cdot 2 \cdot 1}{4 \cdot 5 \cdot 5 \cdot 2} = \frac{2}{200} = \frac{1}{100}.$$
Since $\gcd(1, 100) = 1$, the fraction is already in lowest terms. The denominator is $100$. $\square$

## Editorial

We use the algebraic relation derived for non-trivial digit cancellation to avoid checking all two-digit fractions directly. The search runs over the nonzero digits $a$ and $b$, computes the candidate cancelled denominator digit $e$ from the divisibility condition, and retains only those cases that produce a proper fraction. The valid fractions are then multiplied together in cancelled form, and the denominator of the reduced product is returned.

## Pseudocode

```text
Algorithm: Non-trivial Digit-cancelling Fractions
Require: The nonzero decimal digits.
Ensure: The denominator of the product of all non-trivial digit-cancelling fractions, written in lowest terms.
1: Initialize N ← 1 and D ← 1.
2: For each ordered pair of nonzero digits (a, b), compute e ← 10ab / (9a + b) whenever this quotient is an integer digit.
3: If e is a digit and 10a + b < 10b + e, record the fraction (10a + b) / (10b + e) = a / e by updating N ← N · a and D ← D · e.
4: Reduce the fraction N / D to lowest terms.
5: Return the reduced denominator.
```

## Complexity Analysis

**Proposition.** *The algorithm runs in $O(1)$ time and $O(1)$ space.*

**Proof.** The search iterates over exactly $81$ pairs $(a, b) \in \{1, \ldots, 9\}^2$, with $O(1)$ arithmetic per pair. The final GCD computation operates on bounded integers. All quantities are independent of any input parameter. $\square$

## Answer

$$\boxed{100}$$
