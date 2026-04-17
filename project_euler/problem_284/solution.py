"""
Project Euler Problem 284: Steady Squares

A steady square in base 14 is a number n whose square ends with the same digits
as n (i.e., n^2 ≡ n mod 14^k where k = number of digits of n in base 14).

Find the sum of the digits of all n-digit steady squares in base 14 for 1 <= n <= 10000,
and give the answer in base 14.

Approach:
- Solutions to x^2 ≡ x mod 14^k are: 0, 1, s_k, e_k
  where s_k extends from 7 and e_k extends from 8.
- Use fast doubling: n' = (3n^2 - 2n^3) mod 14^(2k) to double digit count.
- e_k = (14^k + 1 - s_k) mod 14^k.
- Sum digits of all valid k-digit steady squares for k=1..10000.
"""

MAXD = 10000
BASE = 14

def mul_low(a, b, keep):
    """Multiply a * b in base 14, keep only first 'keep' digits."""
    res = [0] * keep
    na, nb = len(a), len(b)
    for i in range(min(na, keep)):
        if a[i] == 0:
            continue
        carry = 0
        jmax = min(nb, keep - i)
        for j in range(jmax):
            carry += res[i + j] + a[i] * b[j]
            res[i + j] = carry % BASE
            carry //= BASE
        j2 = jmax
        while i + j2 < keep and carry > 0:
            carry += res[i + j2]
            res[i + j2] = carry % BASE
            carry //= BASE
            j2 += 1
    return res

def mul_scalar(a, s, keep):
    """Multiply a by scalar s, keep first 'keep' digits."""
    res = [0] * keep
    carry = 0
    for i in range(keep):
        if i < len(a):
            carry += a[i] * s
        res[i] = carry % BASE
        carry //= BASE
    return res

def sub_mod(a, b, keep):
    """(a - b) mod 14^keep."""
    res = [0] * keep
    borrow = 0
    for i in range(keep):
        diff = a[i] - b[i] - borrow
        if diff < 0:
            diff += BASE
            borrow = 1
        else:
            borrow = 0
        res[i] = diff
    return res

# Build 'seven': the automorphic number starting with 7
seven = [0] * MAXD
seven[0] = 7
cur = 1

while cur < MAXD:
    target = min(cur * 2, MAXD)
    n2 = mul_low(seven[:cur], seven[:cur], target)
    n3 = mul_low(n2, seven[:cur], target)
    t1 = mul_scalar(n2, 3, target)
    t2 = mul_scalar(n3, 2, target)
    seven_new = sub_mod(t1, t2, target)
    seven[:target] = seven_new
    cur = target

# Build 'eight': (1 - seven) mod 14^MAXD
eight = [0] * MAXD
borrow = 0
for i in range(MAXD):
    val = (1 if i == 0 else 0) - seven[i] - borrow
    if val < 0:
        val += BASE
        borrow = 1
    else:
        borrow = 0
    eight[i] = val

assert seven[0] == 7
assert eight[0] == 8

# Sum digits of all valid k-digit steady squares for k=1..MAXD
digit_sum = 1  # for the number "1" (1-digit steady square)
running_s = 0
running_e = 0

for k in range(1, MAXD + 1):
    running_s += seven[k - 1]
    running_e += eight[k - 1]
    if seven[k - 1] != 0:
        digit_sum += running_s
    if eight[k - 1] != 0:
        digit_sum += running_e

# Convert to base 14
digits_b14 = "0123456789abcd"
result = ""
tmp = digit_sum
if tmp == 0:
    result = "0"
while tmp > 0:
    result = digits_b14[tmp % 14] + result
    tmp //= 14

print(result)
