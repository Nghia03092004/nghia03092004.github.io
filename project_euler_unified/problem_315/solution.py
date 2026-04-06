"""
Problem 315: Digital Root Clocks

For primes p in (10^7, 2*10^7), compute the total difference
Sam(p) - Max(p) where:
- Sam turns off all segments then turns on new ones at each step
- Max only changes segments that differ

The difference at each transition = 2 * (common segments).

Seven-segment display:
    _a_
   |   |
   f   b
   |_g_|
   |   |
   e   c
   |_d_|
"""

# Segment bitmasks: a=0(top), b=1(top-right), c=2(bot-right), d=3(bot), e=4(bot-left), f=5(top-left), g=6(mid)
# NOTE: In the Project Euler display, digit 7 has 4 segments (includes top-left f).
SEGMENTS = {
    0: 0x3f,  # a,b,c,d,e,f     = 6 segments
    1: 0x06,  # b,c             = 2 segments
    2: 0x5b,  # a,b,d,e,g       = 5 segments
    3: 0x4f,  # a,b,c,d,g       = 5 segments
    4: 0x66,  # b,c,f,g         = 4 segments
    5: 0x6d,  # a,c,d,f,g       = 5 segments
    6: 0x7d,  # a,c,d,e,f,g     = 6 segments
    7: 0x27,  # a,b,c,f         = 4 segments
    8: 0x7f,  # a,b,c,d,e,f,g   = 7 segments
    9: 0x6f,  # a,b,c,d,f,g     = 6 segments
}

SEG_COUNT = {d: bin(SEGMENTS[d]).count('1') for d in range(10)}

def popcount(x):
    """Count set bits."""
    return bin(x).count('1')

def common_segments(a, b):
    """Count common segments between displays of numbers a and b."""
    total = 0
    while a > 0 and b > 0:
        total += popcount(SEGMENTS[a % 10] & SEGMENTS[b % 10])
        a //= 10
        b //= 10
    return total

def digit_sum(n):
    """Sum of digits of n."""
    s = 0
    while n > 0:
        s += n % 10
        n //= 10
    return s

def solve():
    LO = 10_000_000
    HI = 20_000_000

    # Sieve of Eratosthenes
    is_prime = bytearray([1]) * HI
    is_prime[0] = is_prime[1] = 0
    for i in range(2, int(HI**0.5) + 1):
        if is_prime[i]:
            is_prime[i*i::i] = bytearray(len(is_prime[i*i::i]))

    answer = 0
    for p in range(LO + 1, HI):
        if not is_prime[p]:
            continue

        current = p
        while current >= 10:
            nxt = digit_sum(current)
            answer += 2 * common_segments(current, nxt)
            current = nxt

    print(answer)
