"""
Problem 198: Ambiguous Numbers

A best approximation to x for denominator bound d is r/s (fully reduced, s<=d)
such that |x - r/s| < |x - p/q| for any p/q with q <= d and q != s.

x is ambiguous if for some d, there are two or more best approximations.

Key insight: x = p/q (in lowest terms) is ambiguous iff there exist Farey neighbours
a/b, c/d with bc-ad=1 such that x is equidistant from them:
|x - a/b| = |x - c/d|, i.e., x = (a/b + c/d) / 2 = (ad+bc)/(2bd).

We need p/q = (ad+bc)/(2bd) in lowest terms, q <= 10^8, 0 < p/q < 1/100.

Since bc - ad = 1: ad + bc = 2ad + 1 (because bc = ad + 1).
So x = (2ad+1)/(2bd), and since 2ad+1 is odd and gcd(2ad+1, 2bd) needs computation:
gcd(2ad+1, 2bd): since 2ad+1 is odd, = gcd(2ad+1, bd).
gcd(2ad+1, b): since ad ≡ -1 (mod b), 2ad+1 ≡ -2+1 = -1 (mod b), so gcd = gcd(1,b) = 1.
gcd(2ad+1, d): since bc-ad=1, bc ≡ 1 (mod d) (if d > 1), so ad = bc-1 ≡ -1 (mod d).
  2ad+1 ≡ -2+1 = -1 (mod d). So gcd(2ad+1, d) = gcd(1, d) = 1. Wait: -1 mod d.
  gcd(-1, d) = gcd(d-1, d) = 1 for d > 1. For d = 1: gcd(anything, 1) = 1. So yes, gcd = 1.

So gcd(2ad+1, bd) = 1, meaning the fraction (2ad+1)/(2bd) is already in lowest terms.
Denominator = 2bd. Need 2bd <= N = 10^8, i.e., bd <= 5*10^7.

So the problem reduces to:
Count pairs (b, d) with b, d >= 1, gcd(b,d) = 1, bd <= N/2 = 5*10^7,
and there exists a with 0 <= a, bc-ad=1 (c=(1+ad)/b),
such that 0 < (2ad+1)/(2bd) < 1/100.

For the existence of a: since gcd(b,d)=1, the equation bc-ad=1 always has solutions.
The solution a is unique mod b: a ≡ -d^{-1} (mod b), a in [0, b).
(But different choices of a give midpoints differing by integers, so only one is in [0,1).)

Midpoint = (2ad+1)/(2bd).
Condition 0 < midpoint: always true since 2ad+1 >= 1.
Condition midpoint < 1/100: 100(2ad+1) < 2bd => 100a*d + 50 < bd => a < (bd-50)/(100d) = b/100 - 1/(2d).

For the canonical a in [0, b): a ≡ -d^{-1} (mod b).
We need a < b/100 - 1/(2d).

Special cases:
- b = 1: a = 0 always. Condition: 0 < 1/100 - 1/(2d) => d > 50.
  Count: number of d with d > 50, d <= N/2, gcd(1,d)=1 = N/2 - 50.

- b = 2: a ≡ -d^{-1} (mod 2). d must be odd (gcd(2,d)=1). d^{-1} mod 2 = 1. a = (-1) mod 2 = 1.
  Need a < 2/100 = 0.02. But a = 1 >= 0.02. No solutions.

- b = 3,...,99: a >= 1 (since a=0 only if b=1). a < b/100 < 1. Impossible.

- b >= 100: a ranges from 0 to b-1, and we need a < b/100 (roughly).
  Since a ≡ -d^{-1} (mod b), we need to count d with d <= N/(2b), gcd(d,b)=1,
  and (-d^{-1} mod b) < b/100 - 1/(2d).

Now the pair (b,d) is unordered in the sense that (b,d) and (d,b) give different
midpoints (since a depends on the specific b,d). So we count all ordered pairs.

Actually wait - I need to reconsider. The Farey neighbors a/b < c/d give
a specific midpoint. But (a/b, c/d) and (c/d, a/b) don't make sense since
we need a/b < c/d. However, for a pair of denominators (b, d), the Farey
neighbors can have b as the left or right denominator. So we should count
both (b, d, a) and (d, b, a') as potentially giving different midpoints.

Hmm, actually each unordered coprime pair {b, d} gives rise to TWO ordered
pairs of Farey neighbors: one with a/b < c/d and one with c'/d < a'/b.
Wait, no. For Farey neighbors a/b, c/d with bc-ad=1, b and d are the
denominators. If we swap: a'/d, c'/b with b*a' - d*c' = 1... this is a
different pair giving a different midpoint.

So we should count all ORDERED coprime pairs (b, d) with b*d <= N/2.

Let me verify with the b=1 case.
- (b,d) = (1, d): midpoint 1/(2d), condition d > 50. Count: N/2 - 50.
- (d, b) = (d, 1): a ≡ -1^{-1} (mod d) = d-1. Midpoint = (2(d-1)+1)/(2d) = (2d-1)/(2d).
  Need (2d-1)/(2d) < 1/100: 100(2d-1) < 2d => 200d - 100 < 2d => 198d < 100.
  d = 0 (invalid) or d < 100/198 < 1. No solutions.

So ordered pairs are correct and (d, 1) gives midpoints near 1, not near 0.

So the answer = (N/2 - 50) + sum_{b=100}^{N/2} C(b)

where C(b) = #{d : 1 <= d <= N/(2b), gcd(d,b)=1, a(b,d) < b/100 - 1/(2d)}
and a(b,d) = (-d^{-1}) mod b.

For the sum: iterate b from 100 to N/2, and for each b compute C(b).

Approach for computing C(b):
- For each valid a (with gcd(a,b)=1 and 0 <= a < b/100 - 1/(2*1)):
  - d ≡ -a^{-1} (mod b) (since a*d ≡ -1 mod b => d ≡ -a^{-1} mod b)
  - d in [d_min(a), N/(2b)] where d_min ensures a < b/100 - 1/(2d)
    i.e., 1/(2d) < b/100 - a, i.e., d > 1/(2(b/100 - a)) = 50/(b - 100a)
  - d ≡ -a^{-1} (mod b)
  - gcd(d, b) = 1 (automatic since d * a ≡ -1 mod b and gcd(a,b)=1)

For each (b, a), the count of valid d is:
  d_0 = (-a^{-1}) mod b (in [1, b])
  d_min = ceil(50/(b - 100*a)) + 1 if 50/(b-100a) is integer, else ceil(50/(b-100a))
  Actually: d > 50/(b - 100*a), so d >= floor(50/(b-100*a)) + 1.
  d_max = N // (2*b)

  Count = max(0, (d_max - first_valid_d) // b + 1)
  where first_valid_d is the smallest d >= d_min with d ≡ d_0 (mod b).

The total work: sum_{b=100}^{N/2} (number of valid a for b) ≈ sum_{b=100}^{N/2} phi(b)/100
≈ (1/100) * sum phi(b) ≈ (1/100) * 3*(N/2)^2/pi^2 ≈ 1.5 * 10^12. Too slow!

We need to split into small-b and large-b.

For b <= B (B = sqrt(N/2) ≈ 7071):
  d_max = N/(2b) >= B. Many d per (b,a). Iterate a, compute count.
  Work per b: b/100 (number of a values). Total: sum_{b=100}^{B} b/100 ≈ B^2/200 ≈ 250000.

For b > B:
  d_max = N/(2b) < B. Few d values.
  Swap: iterate d from 1 to B, then for each d iterate b.
  For each d, b ranges over [max(B+1, 100), N/(2d)].
  For each (d, b), check a(b,d) < b/100 - 1/(2d).

  But b ranges up to N/(2d) which can be up to 5*10^7. Too many.

  Better: for each d, b must be coprime to d and have a specific a.
  a = (-d^{-1}) mod b depends on b, so we can't simply batch.

  Alternative for large b: use the fact that the valid a values are few.
  For b > B, a < b/100. But a ≡ -d^{-1} (mod b). So we need to find,
  for each (residue class mod b), how many d in [1, d_max] satisfy it.
  Since d_max < B < b, there's at most 1 valid d per a.

  So for b > B: d = ((-a^{-1}) mod b), and check 1 <= d <= d_max and d > 50/(b-100a).
  Total work: sum_{b=B+1}^{N/2} (b/100) ... still too much.

  Hmm. For b > B and d < B: swap iteration to d outer, b inner.
  For each d from 1 to B:
    For each b in [B+1, N/(2d)] with gcd(b,d)=1:
      a = (-d^{-1}) mod b
      Check a < b/100 - 1/(2d)

  But this iterates over all b, which is up to N/(2d).
  Total: sum_{d=1}^{B} N/(2d) ≈ N/2 * ln(B) ≈ 5*10^7 * 9 ≈ 4.5*10^8. Borderline.

  Can optimize: for fixed d, the condition a < b/100 - 1/(2d) where a = (-d^{-1}) mod b.
  Note: a = b - (d^{-1} mod b) (if d^{-1} mod b > 0).

  d^{-1} mod b: this is the unique r in [1, b-1] with d*r ≡ 1 (mod b).
  a = b - r = b - (d^{-1} mod b).
  Condition: b - r < b/100 - 1/(2d) => b(1 - 1/100) < r + ... => r > 99b/100 + 1/(2d).
  i.e., r > 99b/100 (approximately). So r/b > 99/100, meaning d^{-1}/b is close to 1,
  i.e., d^{-1} ≡ r (mod b) with r > 99b/100. Equivalently d*r ≡ 1 (mod b) and r > 99b/100.
  This means d*r = 1 + kb for some k, and r > 99b/100.
  So 1 + kb = d*r > d*99b/100 => k > 99d/100 - 1/b. For d >= 2: k >= 1.
  Also r = (1+kb)/d <= b-1 => 1+kb <= d(b-1) => k <= (d(b-1)-1)/b < d.
  And k > 99d/100 - 1/b. For large d, k is near d, so k in [ceil(99d/100), d-1].

  For each valid k: b = (d*r - 1)/k. And r = (1+kb)/d. Hmm, circular.

  Let me try: d*r - kb = 1. So for each k, r = (1 + kb)/d must be integer.
  1 + kb ≡ 0 (mod d) => kb ≡ -1 (mod d) => b ≡ -k^{-1} (mod d) (when gcd(k,d)=1).

  This is getting too complicated. Let me just implement the straightforward O(N^(1+eps))
  algorithm and accept that it might take some time.

  For the problem, I'll use a combined approach:
  1) b = 1: counted analytically
  2) small b (100 to B): iterate a for each b
  3) large b (B+1 to N/2): iterate d from 1 to B, b per d
"""

import math
import sys

def solve():
    N = 10**8
    HALF = N // 2
    B = int(math.isqrt(HALF))  # ~7071

    count = 0

    # b = 1: a = 0, d > 50, d <= HALF
    count += HALF - 50

    # Small b: 100 to B
    for b in range(100, B + 1):
        d_max = HALF // b
        if d_max < 1:
            break

        # a < b/100 - 1/(2*d) for d >= 1
        # Maximum possible a: floor(b/100 - 1/2) for d=1, floor(b/100 - epsilon) for d=inf
        # Conservative upper bound on a: b // 100
        a_upper = b // 100
        if b % 100 == 0:
            a_upper = b // 100 - 1  # a < b/100 strictly

        for a in range(0, a_upper + 1):
            if a == 0:
                if b != 1:
                    continue  # a=0 only valid for b=1, already handled
            if math.gcd(a, b) != 1:
                continue

            # d ≡ -a^{-1} (mod b)
            a_inv = pow(a, -1, b)
            d_res = (-a_inv) % b
            if d_res == 0:
                d_res = b  # smallest positive d in this residue class

            # d > 50 / (b - 100*a)
            gap = b - 100 * a
            if gap <= 0:
                continue

            # d_min: smallest integer > 50/gap
            if 50 % gap == 0:
                d_min = 50 // gap + 1
            else:
                d_min = 50 // gap + 1  # ceil(50/gap) works since we need strict >
                # Actually: d > 50/gap. 50/gap might not be integer.
                # d_min = floor(50/gap) + 1 always works for strict inequality.

            # First d >= d_min with d ≡ d_res (mod b)
            if d_res >= d_min:
                first_d = d_res
            else:
                k = (d_min - d_res + b - 1) // b
                first_d = d_res + k * b

            if first_d > d_max:
                continue

            cnt = (d_max - first_d) // b + 1
            count += cnt

    # Large b: B+1 to HALF
    # For each d from 1 to B, iterate b from B+1 to HALF//d
    # For each (d, b) coprime: a = (-d^{-1}) mod b, check a > 0 and a < b/100 - 1/(2d)

    for d in range(1, B + 1):
        b_max = HALF // d
        if b_max <= B:
            continue

        # For each b in [B+1, b_max] with gcd(b, d) = 1:
        # a = (-d^{-1}) mod b, check 1 <= a < b/100 - 1/(2d)
        # (a = 0 only if b = 1, not in this range)

        # The threshold: a < b/100 - 1/(2d) ≈ b/100.
        # And a = b - (d^{-1} mod b). So d^{-1} mod b > b - b/100 = 99b/100 + 1/(2d).
        # Let r = d^{-1} mod b. Then r > 99b/100 (approximately).
        # d*r ≡ 1 (mod b) and r in [1, b-1].
        # d*r = 1 + m*b for some m >= 0.
        # r = (1 + m*b) / d, must be integer.
        # r > 99b/100: (1+mb)/d > 99b/100 => 100(1+mb) > 99bd => 100 + 100mb > 99bd
        # => m > (99bd - 100)/(100b) = 99d/100 - 1/b.
        # Also r < b: (1+mb)/d < b => m < (bd-1)/b = d - 1/b. So m <= d-1.
        # And r >= 1: m >= 0. Also r > 99b/100 + 1/(2d) for the exact condition.

        # But we also need the EXACT condition: a < b/100 - 1/(2d).
        # a = b - r. So b - r < b/100 - 1/(2d) => r > 99b/100 + 1/(2d).
        # r = (1+mb)/d => (1+mb)/d > 99b/100 + 1/(2d).
        # (1+mb)/d - 1/(2d) > 99b/100.
        # (2(1+mb) - 1)/(2d) > 99b/100.
        # (2mb + 1)/(2d) > 99b/100.
        # 100(2mb+1) > 99b*2d = 198bd.
        # 200mb + 100 > 198bd.
        # m > (198bd - 100)/(200b) = 99d/100 - 1/(2b).

        # So m >= ceil(99d/100 - 1/(2b) + epsilon) = ceil(99d/100) when 1/(2b) is tiny.
        # For d divisible by 100: m >= 99d/100 (since 99d/100 is integer and we need strict >).
        # Wait: m > 99d/100 - 1/(2b). If 99d/100 is integer: m > integer - tiny => m >= 99d/100.
        # If 99d/100 not integer: m >= ceil(99d/100).

        m_min_exact_num = 198 * d  # m > (198*b*d - 100) / (200*b) for each b
        # But m_min depends on b... we need m > (198bd-100)/(200b) = 99d/100 - 1/(2b)
        # This is essentially m >= ceil(99d/100) for all b > B > 7000 (since 1/(2b) is tiny).

        if d * 99 % 100 == 0:
            m_min = 99 * d // 100  # m > 99d/100 - tiny, m is integer, so m >= 99d/100
        else:
            m_min = 99 * d // 100 + 1  # ceil(99d/100)

        # WAIT: we also need m_min to satisfy the original condition more carefully.
        # But since 1/(2b) < 1/(2*7071) < 0.0001, and m is integer, the adjustment is negligible.
        # Let's just use m_min = ceil(99d/100 - 1/(2*B)) which equals ceil(99d/100) for most d.

        # Also m <= d-1 (since r < b).
        m_max = d - 1

        for m in range(m_min, m_max + 1):
            # r = (1 + m*b) / d must be integer for some b in [B+1, b_max].
            # 1 + m*b ≡ 0 (mod d) => m*b ≡ -1 (mod d) => b ≡ -m^{-1} (mod d) [if gcd(m,d)=1]
            if math.gcd(m, d) != 1:
                # 1 + m*b ≡ 0 (mod d): m*b ≡ -1 (mod d). Since gcd(m,d) > 1,
                # and gcd(m,d) must divide -1, which means gcd(m,d) = 1. Contradiction.
                # So if gcd(m,d) > 1, no solution. Skip.
                continue

            b_res = (-pow(m, -1, d)) % d
            if b_res == 0:
                b_res = d

            # b in [B+1, b_max] with b ≡ b_res (mod d)
            # First b >= B+1 with b ≡ b_res (mod d)
            if b_res >= B + 1:
                first_b = b_res
            else:
                k = (B + 1 - b_res + d - 1) // d
                first_b = b_res + k * d

            if first_b > b_max:
                continue

            # For each valid b, check:
            # 1. gcd(b, d) = 1 (since b ≡ b_res mod d and gcd computation...)
            #    Actually b ≡ -m^{-1} (mod d), and we need gcd(b,d)=1.
            #    gcd(-m^{-1}, d): since m^{-1} exists mod d, gcd(m^{-1}, d)=1, so gcd(b,d)=gcd(b_res,d).
            #    Let's check: gcd(b_res, d). We have m*b_res ≡ -1 (mod d), so gcd(b_res,d)|gcd(-1,d)=1.
            #    So gcd(b_res, d) = 1. And since b ≡ b_res (mod d), gcd(b,d) = gcd(b_res,d) = 1. Good.

            # 2. Need to verify exact condition: a = b - r, a < b/100 - 1/(2d).
            #    r = (1 + m*b)/d. a = b - (1+m*b)/d = (bd - 1 - mb)/(d) = (b(d-m) - 1)/d.
            #    Since d > m (m <= d-1): a = (b(d-m)-1)/d.
            #    Condition: (b(d-m)-1)/d < b/100 - 1/(2d).
            #    Multiply by d: b(d-m) - 1 < bd/100 - 1/2.
            #    b(d-m) - bd/100 < 1 - 1/2 = 1/2.
            #    b(d - m - d/100) < 1/2.
            #    b * d * (1 - m/d - 1/100) < 1/2.
            #    Hmm this should be < 0 for valid m (m > 99d/100 roughly).
            #    b(d-m) - bd/100 = b(d-m-d/100) = b*d*(1 - m/d - 1/100).
            #    For m >= ceil(99d/100): 1 - m/d <= 1 - 99/100 = 1/100.
            #    So b*d*(1/100 - ...) which could be > 0 for m = ceil(99d/100).
            #    We need b*d*(1 - m/d - 1/100) < 1/2.
            #    I.e., b < 1/(2*d*(1 - m/d - 1/100)).

            #    For m = ceil(99d/100): 1 - m/d - 1/100. If d=100: m=99, val = 1-99/100-1/100 = 0.
            #    Boundary case! Need b*0 < 1/2: always true (0 < 1/2). So all b valid.
            #    For d=200: m >= 198. m=198: 1-198/200-1/100 = 1-0.99-0.01 = 0. All b valid.
            #    For d=101: m >= 100. m=100: 1-100/101-1/100 = (10100-10000-101)/10100 = -1/10100 < 0. All b valid.
            #    For d=99: m >= 99*99/100 = 98.01, m >= 99. m=99: but m <= d-1 = 98. No valid m!

            # So for d <= 99: m_min >= ceil(99*d/100). For d=99: m_min >= 99. But m_max = 98. No solutions.
            # For d=100: m_min = 99, m_max = 99. One m value.
            # For d=101: m_min = 100, m_max = 100. One m value.
            # For d >= 100: number of valid m ≈ d - 99d/100 = d/100. So O(d/100) per d.

            # Since the exact condition is always satisfied when m > 99d/100 (the expression is <= 0),
            # we don't need per-b checking for most cases. Only the boundary m = ceil(99d/100)
            # might need checking, but as shown, it's usually fine.

            # Count b values in [first_b, b_max] with step d
            cnt_b = (b_max - first_b) // d + 1
            count += cnt_b

    print(count)

if __name__ == "__main__":
    solve()
