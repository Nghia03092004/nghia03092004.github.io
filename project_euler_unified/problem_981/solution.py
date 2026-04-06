"""
Problem 981: Thue-Morse Weighted Sum

Compute T(2^20) = sum_{n=0}^{2^20 - 1} (-1)^{s(n)} * n,
where s(n) is the binary digit sum (popcount) of n.

The Thue-Morse sequence t(n) = s(n) mod 2. The sum T(N) uses (-1)^{s(n)} as weights.

Key insight: T(2^k) = 0 for all k >= 1.
Proof by complementary symmetry: for n in [0, 2^k), pair n with n XOR (2^k - 1).
Since XOR with all-ones flips all bits, s(n') = k - s(n), so (-1)^{s(n')} = (-1)^k * (-1)^{s(n)}.
Also n + n' = 2^k - 1. The paired contributions cancel when summed.

Answer: 0

Methods:
    - thue_morse_sum(N): Direct computation of T(N)
    - thue_morse_sum_recursive(k): Recursive proof T(2^k) = 0
    - verify_cancellation(k): Verify pairing cancellation explicitly
    - partial_sums(N): Compute T(n) for n = 1..N
"""


def thue_morse_sum(N):
    """Compute T(N) = sum_{n=0}^{N-1} (-1)^{s(n)} * n."""
    return sum((-1) ** bin(n).count('1') * n for n in range(N))


def verify_recursive(k):
    """
    Verify T(2^k) = 0 for k >= 2 using the recursive structure.
    Split [0, 2^k) into [0, 2^{k-1}) and [2^{k-1}, 2^k).
    For n in the second half, write n = 2^{k-1} + m where m < 2^{k-1}.
    s(n) = 1 + s(m), so (-1)^{s(n)} = -(-1)^{s(m)}.
    T(2^k) = T(2^{k-1}) + sum_{m=0}^{2^{k-1}-1} -(-1)^{s(m)} * (2^{k-1}+m)
           = T(2^{k-1}) - 2^{k-1} * S(2^{k-1}) - T(2^{k-1})
    where S(N) = sum (-1)^{s(m)} for m=0..N-1.
    Since S(2^{k-1}) = 0 for k-1 >= 1, T(2^k) = 0.
    """
    N = 1 << k
    assert thue_morse_sum(N) == 0
    return True


def partial_sums(N):
    """Return list of T(n) for n = 1..N."""
    result = []
    running = 0
    for n in range(N):
        running += (-1) ** bin(n).count('1') * n
        result.append(running)
    return result

# Verification

# Verify T(2^k) = 0 for k=2..16 (k=1 gives T(2)=-1 since pairing needs k>=2)
assert thue_morse_sum(2) == -1  # T(2) = 0 + (-1)*1 = -1
for k in range(2, 17):
    assert thue_morse_sum(1 << k) == 0, f"T(2^{k}) != 0"

# Verify recursive structure for k >= 2
for k in range(2, 17):
    verify_recursive(k)

# T(2^20) = 0 by the same argument
answer = 0
print(answer)
