"""
Problem 844: k-Markov Sequences

Count sequences of length n over alphabet of size sigma where each symbol
depends on the previous k symbols, via transfer matrix exponentiation.
"""

MOD = 10**9 + 7

# --- Method 1: Matrix exponentiation ---
def mat_mul(A, B, mod):
    """Multiply two matrices modulo mod."""
    n = len(A)
    m = len(B[0])
    k = len(B)
    C = [[0]*m for _ in range(n)]
    for i in range(n):
        for j in range(m):
            s = 0
            for l in range(k):
                s += A[i][l] * B[l][j]
            C[i][j] = s % mod
    return C

def mat_pow(M, p, mod):
    """Compute M^p mod mod via binary exponentiation."""
    n = len(M)
    result = [[1 if i == j else 0 for j in range(n)] for i in range(n)]
    while p > 0:
        if p & 1:
            result = mat_mul(result, M, mod)
        M = mat_mul(M, M, mod)
        p >>= 1
    return result

def count_sequences_matrix(sigma, k, n, forbidden=None, mod=MOD):
    """Count valid k-Markov sequences of length n.
    forbidden: set of (k+1)-grams that are not allowed.
    """
    if forbidden is None:
        forbidden = set()

    states = sigma ** k
    # Build transfer matrix
    M = [[0]*states for _ in range(states)]
    for s in range(states):
        # Decode state s as a k-gram
        digits_s = []
        val = s
        for _ in range(k):
            digits_s.append(val % sigma)
            val //= sigma
        digits_s.reverse()

        for c in range(sigma):
            # New state: last (k-1) digits of s + c
            gram = tuple(digits_s) + (c,)
            if gram in forbidden:
                continue
            t = 0
            for d in digits_s[1:]:
                t = t * sigma + d
            t = t * sigma + c
            M[s][t] = 1

    if n < k:
        return sigma ** n

    # Total sequences = sum of all entries in M^(n-k) * initial vector
    Mp = mat_pow(M, n - k, mod)
    total = 0
    for i in range(states):
        for j in range(states):
            total = (total + Mp[i][j]) % mod
    return total

# --- Method 2: Direct enumeration (brute force for small n) ---
def count_sequences_brute(sigma, k, n, forbidden=None):
    """Brute force enumeration."""
    if forbidden is None:
        forbidden = set()
    count = 0
    def backtrack(seq):
        nonlocal count
        if len(seq) == n:
            count += 1
            return
        for c in range(sigma):
            if len(seq) >= k:
                gram = tuple(seq[-(k):]) + (c,)
                if gram in forbidden:
                    continue
            seq.append(c)
            backtrack(seq)
            seq.pop()
    backtrack([])
    return count

# --- Verify ---
# No constraints
for nn in range(1, 8):
    mat_ans = count_sequences_matrix(2, 1, nn, mod=10**18)
    assert mat_ans == 2**nn, f"n={nn}: got {mat_ans}"

# With forbidden "00" for k=1, sigma=2 (Fibonacci-like)
forbidden_00 = {(0, 0)}
for nn in range(1, 10):
    mat_ans = count_sequences_matrix(2, 1, nn, forbidden_00, mod=10**18)
    brute_ans = count_sequences_brute(2, 1, nn, forbidden_00)
    assert mat_ans == brute_ans, f"n={nn}: mat={mat_ans}, brute={brute_ans}"

print("All verification passed!")
print(f"Example: sigma=2, k=2, n=100, no '000': {count_sequences_matrix(2, 2, 100, {(0,0,0)})}")
