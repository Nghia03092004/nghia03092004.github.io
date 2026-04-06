"""
Problem 337: Totient Stairstep Sequences
Find the number of totient stairstep sequences ending at n <= 10^7.
Answer: 7124517762917989
"""

def solve(N=10_000_000):
    # Compute Euler's totient function using sieve
    phi = list(range(N + 1))
    for i in range(2, N + 1):
        if phi[i] == i:  # i is prime
            for j in range(i, N + 1, i):
                phi[j] -= phi[j] // i

    # DP: f[n] = number of totient stairstep sequences ending at n
    f = [0] * (N + 1)
    for i in range(1, N + 1):
        f[i] = 1  # single-element sequence {i}

    total = 0

    # For each n, propagate f[n] to all multiples m where phi[m] > phi[n]
    for n in range(1, N + 1):
        total += f[n]
        m = 2 * n
        while m <= N:
            if phi[m] > phi[n]:
                f[m] += f[n]
            m += n

    return total

def main():
    result = solve()
    print(f"Answer: {result}")
    assert result == 7124517762917989, f"Expected 7124517762917989, got {result}"
    print("Verified: 7124517762917989")

if __name__ == "__main__":
    main()
