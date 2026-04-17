# Problem 19: Counting Sundays

## Problem Statement

Given that 1 January 1900 was a Monday, determine the number of Sundays that fell on the first of the month during the period 1 January 1901 to 31 December 2000 (inclusive).

## Mathematical Development

**Definition 1 (Day-of-week encoding).** Encode days of the week as elements of $\mathbb{Z}/7\mathbb{Z}$: Sunday $\equiv 0$, Monday $\equiv 1$, Tuesday $\equiv 2$, ..., Saturday $\equiv 6$.

**Definition 2 (Leap year).** A year $y$ in the Gregorian calendar is a *leap year* if and only if

$$L(y) \iff \bigl(4 \mid y\bigr) \wedge \bigl(100 \nmid y \;\lor\; 400 \mid y\bigr).$$

**Lemma 1 (Month lengths).** Let $D(m, y)$ denote the number of days in month $m \in \{1, \ldots, 12\}$ of year $y$. Then:

$$D(m, y) = \begin{cases}
31 & m \in \{1, 3, 5, 7, 8, 10, 12\}, \\
30 & m \in \{4, 6, 9, 11\}, \\
28 + [L(y)] & m = 2,
\end{cases}$$

where $[\cdot]$ is the Iverson bracket.

*Proof.* This is the defining rule of the Gregorian calendar. $\square$

**Theorem 1 (Day-of-week advancement).** If the 1st of month $m$ in year $y$ falls on day $w \in \mathbb{Z}/7\mathbb{Z}$, then the 1st of the subsequent month falls on day

$$w' \equiv w + D(m, y) \pmod{7}.$$

*Proof.* The first day of the next month is exactly $D(m, y)$ days later. Since the day-of-week function is periodic with period 7, $w' = w + D(m, y) \bmod 7$. $\square$

**Definition 3 (Day-of-week residues modulo 7).** The residues $D(m, y) \bmod 7$ for a non-leap year are:

| Month | Jan | Feb | Mar | Apr | May | Jun | Jul | Aug | Sep | Oct | Nov | Dec |
|-------|-----|-----|-----|-----|-----|-----|-----|-----|-----|-----|-----|-----|
| $D \bmod 7$ | 3 | 0 | 3 | 2 | 3 | 2 | 3 | 3 | 2 | 3 | 2 | 3 |

In a leap year, only February changes: $D(2, y) \bmod 7 = 1$.

**Theorem 2 (Gregorian cycle).** The Gregorian calendar has a period of exactly 400 years, comprising $146097 = 20871 \times 7$ days.

*Proof.* In 400 years there are $400 \times 365 + 97 = 146097$ days, where $97 = 100 - 4 + 1$ is the number of leap years (every 4th year except centuries, plus every 400th). Since $146097 = 20871 \times 7$, the cycle is an exact multiple of weeks, so the day-of-week pattern repeats exactly every 400 years. $\square$

**Corollary 1 (Expected count heuristic).** The period 1901--2000 contains $100 \times 12 = 1200$ first-of-month dates. If these were uniformly distributed over the 7 weekdays, we would expect $1200/7 \approx 171.4$ Sundays. The exact answer 171 is consistent with near-uniformity.

**Theorem 3 (Sequential computation).** Starting from the known anchor $w_0 = 1$ (Monday, 1 January 1900), iteratively applying Theorem 1 through all months from January 1900 to December 2000, and counting those months in the range 1901--2000 where $w \equiv 0 \pmod{7}$, yields the exact count of first-of-month Sundays.

*Proof.* The iteration is a faithful simulation of the Gregorian calendar. Each application of Theorem 1 is exact (no approximation). The count records precisely those first-of-month days falling on Sunday within the specified range. $\square$

## Algorithm

We simulate the calendar month by month, keeping track of the weekday of the first day of the current month. Beginning from the known fact that 1 January 1900 was a Monday, we iterate through all months up to the ending year, count the months in the target interval whose first day is Sunday, and advance the weekday by the month length modulo 7. This is sufficient because every month in the range is visited exactly once.

## Pseudocode

```text
Algorithm: Counting Sundays on the First of the Month
Require: The date range from 1 January 1901 through 31 December 2000, together with the fact that 1 January 1900 was a Monday.
Ensure: The number of months in the target range whose first day is a Sunday.
1: Initialize the weekday of the first day of the current month and set count ← 0.
2: For each month from January 1900 through December 2000 do:
3:     If the current year lies in 1901, ..., 2000 and the current first weekday is Sunday, increment count.
4:     Advance the weekday by the length of the current month, using the leap-year rule for February.
5: Return count.
```

## Complexity Analysis

**Proposition 1.** The algorithm runs in $\Theta(Y)$ time and $\Theta(1)$ space, where $Y = y_{\text{end}} - 1900 + 1$.

*Proof.* The outer loop runs $Y$ iterations, each with 12 inner iterations of $O(1)$ work (one comparison, one modular addition). The total is $12Y = O(Y)$ operations. Only a constant number of integer variables are maintained. $\square$

## Answer

$$\boxed{171}$$
