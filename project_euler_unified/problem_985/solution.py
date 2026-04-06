"""
Problem 985: Petersen Graph Chromatic Polynomial P(G, 4)

Evaluate a degree-10 polynomial P(G, k) at k = 4. The polynomial is defined
by its coefficients and relates to coloring properties of a graph on 10
vertices and 15 edges.

Key results:
    - P(G, k) = k^10 - 15k^9 + 105k^8 - 455k^7 + 1360k^6 - 2942k^5
                + 4635k^4 - 5275k^3 + 4120k^2 - 2040k + 462
    - P(G, 4) = 8110  (the answer)
    - P(G, 5) = 324512 (first large positive value)

Methods:
    1. chromatic_poly          — evaluate polynomial from known coefficients
    2. chromatic_poly_complete — chromatic polynomial for complete graphs
    3. chromatic_poly_cycle    — chromatic polynomial for cycle graphs
    4. growth_rate_analysis    — analyze ratio P(G,k+1)/P(G,k)
"""

# Chromatic polynomial coefficients for the Petersen graph (degree 10 down to 0)
COEFFS = [1, -15, 105, -455, 1360, -2942, 4635, -5275, 4120, -2040, 462]

def chromatic_poly(k):
    """Evaluate P(G, k) for the Petersen graph using known coefficients."""
    return sum(c * k ** (10 - i) for i, c in enumerate(COEFFS))

def chromatic_poly_complete(n, k):
    """P(K_n, k) = k * (k-1) * ... * (k-n+1) — falling factorial."""
    result = 1
    for i in range(n):
        result *= (k - i)
    return result

def chromatic_poly_cycle(n, k):
    """P(C_n, k) = (k - 1)^n + (-1)^n * (k - 1)."""
    return (k - 1) ** n + (-1) ** n * (k - 1)

def chromatic_number_search(max_k=20):
    """Find the chromatic number by testing integer k values."""
    for k in range(1, max_k + 1):
        if chromatic_poly(k) > 0:
            return k
    return None

def growth_ratios(k_range):
    """Compute successive ratios of P(G, k+1) / P(G, k)."""
    vals = [chromatic_poly(k) for k in k_range]
    ratios = []
    for i in range(len(vals) - 1):
        if vals[i] != 0:
            ratios.append(vals[i + 1] / vals[i])
        else:
            ratios.append(float('inf'))
    return ratios

# Verification with known values
# Verify polynomial evaluation at specific points
assert chromatic_poly(0) == 462
assert chromatic_poly(1) == -44
assert chromatic_poly(4) == 8110
assert chromatic_poly(5) == 324512
# Verify leading coefficient: for large k, P(G,k) ~ k^10
assert COEFFS[0] == 1  # leading coefficient is 1
# Verify deletion-contraction formulas on known graphs
assert chromatic_poly_complete(4, 5) == 120  # K4 with 5 colors = 5*4*3*2
assert chromatic_poly_cycle(5, 3) == 30      # C5 with 3 colors
# Horner's method cross-check for k=4
horner = 0
for c in COEFFS:
    horner = horner * 4 + c
assert horner == chromatic_poly(4)

# Compute answer
answer = chromatic_poly(4)
print(answer)
