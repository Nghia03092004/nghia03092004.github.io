"""
Problem 957: Diophantine Approximation

Convergents p_k/q_k of sqrt(2) = [1; 2, 2, 2, ...] satisfy p_k^2 - 2*q_k^2 = (-1)^{k+1}.
Sum for k=0..50: sum of (-1)^{k+1} = -1 (26 terms of -1, 25 terms of +1).

Verification by direct computation of convergents.
"""

def convergents_sqrt2(N: int) -> list[tuple[int, int]]:
    """Compute convergents p_k/q_k of sqrt(2) for k = 0, 1, ..., N."""
    result = []
    p_prev, p_curr = 0, 1  # p_{-1}, p_0
    q_prev, q_curr = 1, 1  # q_{-1}, q_0
    # a_0 = 1, so p_0 = 1, q_0 = 1
    result.append((p_curr, q_curr))
    for k in range(1, N + 1):
        a_k = 2
        p_next = a_k * p_curr + p_prev
        q_next = a_k * q_curr + q_prev
        p_prev, p_curr = p_curr, p_next
        q_prev, q_curr = q_curr, q_next
        result.append((p_curr, q_curr))
    return result

def solve(N: int = 50) -> int:
    """Compute sum of (p_k^2 - 2*q_k^2) for k = 0 to N."""
    convs = convergents_sqrt2(N)
    return sum(p * p - 2 * q * q for p, q in convs)

# --- Compute answer ---
N = 50
convs = convergents_sqrt2(N)

# Verify identity p_k^2 - 2*q_k^2 = (-1)^{k+1}
for k, (p, q) in enumerate(convs):
    val = p * p - 2 * q * q
    expected = (-1) ** (k + 1)
    assert val == expected, f"k={k}: {val} != {expected}"

answer = sum(p * p - 2 * q * q for p, q in convs)
# Closed form check
assert answer == -1, f"Expected -1, got {answer}"

print(answer)
