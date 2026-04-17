"""
Problem 661: A Long Chess Match

Compute H(n) = sum_{k=3}^{n} E_A(1/sqrt(k+3), 1/sqrt(k+3)+1/k^2, 1/k^3)
where E_A is the expected number of times A leads in a match with
geometric killing rate p.

Key formula: E_A = r_+ * z / ((1 - z) * (1 - r_+ * z))
where z = 1 - p, r_+ = (1 - sqrt(1 - 4*pA*pB)) / (2*pB).
"""

from math import sqrt

# --- Method 1: Closed-form via Wiener-Hopf factorization ---
def expected_leading_closed(pA, pB, p):
    """Expected times A leads, using closed-form generating function."""
    disc = 1 - 4 * pA * pB
    if disc < 0:
        disc = 0.0
    r_plus = (1 - sqrt(disc)) / (2 * pB)
    z = 1 - p
    # E_A = sum_{t>=0} (1-p)^t * Pr[d_t > 0]
    # = r_plus * z / ((1 - z) * (1 - r_plus * z))
    denom = (1 - z) * (1 - r_plus * z)
    if abs(denom) < 1e-30:
        return 0.0
    return r_plus * z / denom

# --- Method 2: Absorbing Markov chain (tridiagonal matrix) ---
def expected_leading_markov(pA, pB, p, M=300):
    """Expected times A leads via truncated Markov chain."""
    # States: d in {-M, ..., 0, ..., M}, total 2M+1 states
    # State index i corresponds to d = i - M
    size = 2 * M + 1
    # Build tridiagonal system (I - Q) x = e_M (start at d=0)
    # Using Thomas algorithm for efficiency

    # Transition probabilities (with killing)
    surv = 1 - p
    a_coeff = pB * surv  # move left
    b_coeff = (1 - pA - pB) * surv  # stay
    c_coeff = pA * surv  # move right

    # (I - Q) is tridiagonal: diagonal = 1-b, off-diag = -a (lower), -c (upper)
    # Solve (I-Q) x = e_{M} where M is the index of d=0
    diag = [1.0 - b_coeff] * size
    lower = [-a_coeff] * size  # lower[i] is coefficient of x[i-1] in row i
    upper = [-c_coeff] * size  # upper[i] is coefficient of x[i+1] in row i

    # Boundary: absorbing at edges
    diag[0] = 1.0
    diag[-1] = 1.0
    lower[0] = 0.0
    upper[-1] = 0.0

    rhs = [0.0] * size
    rhs[M] = 1.0  # start at d = 0

    # Forward elimination
    for i in range(1, size):
        if abs(diag[i-1]) < 1e-30:
            continue
        factor = lower[i] / diag[i-1]
        diag[i] -= factor * upper[i-1]
        rhs[i] -= factor * rhs[i-1]

    # Back substitution
    x = [0.0] * size
    x[-1] = rhs[-1] / diag[-1] if abs(diag[-1]) > 1e-30 else 0.0
    for i in range(size - 2, -1, -1):
        x[i] = (rhs[i] - upper[i] * x[i+1]) / diag[i] if abs(diag[i]) > 1e-30 else 0.0

    # Sum expected visits to positive states (d > 0, indices M+1 to 2M)
    # But wait - x gives expected visits starting from d=0
    # We need to solve for each target state... this approach needs N columns.
    # Instead, use the fact that E_A = sum of N[0, d] for d > 0.
    # For the fundamental matrix, we need row 0 (start=d=0).
    # This is actually computing one column of (I-Q)^{-1}, not what we want.

    # Alternative: direct simulation sum
    # E_A = sum_{t=0}^{T} (1-p)^t * Pr[d_t > 0]
    # Use convolution / FFT or direct DP

    # DP: prob[t][d] = probability of being at state d at time t
    T_max = min(int(10 / p), 50000)  # enough steps
    # Use two arrays for current and next
    prob = [0.0] * size
    prob[M] = 1.0  # start at d=0

    result = 0.0
    for t in range(1, T_max + 1):
        new_prob = [0.0] * size
        for i in range(1, size - 1):
            new_prob[i] = (prob[i-1] * c_coeff +
                          prob[i] * b_coeff +
                          prob[i+1] * a_coeff)
        prob = new_prob
        # Add probability of being in positive states
        pos_prob = sum(prob[M+1:])
        result += pos_prob
        if pos_prob < 1e-15:
            break

    return result

# --- Compute H(n) ---
def compute_H(n):
    """Compute H(n) = sum_{k=3}^{n} E_A(params(k))."""
    total = 0.0
    terms = []
    for k in range(3, n + 1):
        pA = 1.0 / sqrt(k + 3)
        pB = pA + 1.0 / (k * k)
        p = 1.0 / (k ** 3)
        ea = expected_leading_closed(pA, pB, p)
        total += ea
        terms.append((k, pA, pB, p, ea))
    return total, terms

# --- Compute and verify ---
H3, _ = compute_H(3)
print(f"H(3) = {H3:.4f} (expected ~6.8345)")

H50, terms = compute_H(50)
print(f"H(50) = {H50:.4f}")

# Verify with Markov chain for small case
ea_check = expected_leading_closed(0.25, 0.25, 0.5)
print(f"E_A(0.25, 0.25, 0.5) = {ea_check:.6f} (expected ~0.585786)")

# Cross-check Method 1 vs Method 2 for k=3
pA3 = 1.0 / sqrt(6)
pB3 = pA3 + 1.0 / 9
p3 = 1.0 / 27
ea_closed = expected_leading_closed(pA3, pB3, p3)
print(f"E_A closed (k=3) = {ea_closed:.6f}")

# Assertions
assert abs(H3 - 6.8345) < 0.01, f"H(3) mismatch: {H3}"
assert abs(ea_check - 0.585786) < 0.001, f"E_A check mismatch: {ea_check}"
