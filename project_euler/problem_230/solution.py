"""
Problem 230: Fibonacci Words

F(1) = A (100 digits of pi), F(2) = B (next 100 digits of pi)
F(n) = F(n-2) . F(n-1) (concatenation, older part first)
Sequence: A, B, AB, BAB, ABBAB, ...

Find sum of D((127+19n)*7^n) * 10^n for n = 0 to 17,
where D(k) is the k-th digit of the infinite Fibonacci word.
"""

def solve():
    A = ("1415926535897932384626433832795028841971"
         "6939937510582097494459230781640628620899"
         "86280348253421170679")
    B = ("8214808651328230664709384460955058223172"
         "5359408128481117450284102701938521105559"
         "64462294895493038196")

    # Precompute Fibonacci lengths
    MAX_N = 200
    L = [0] * (MAX_N + 1)
    L[1] = L[2] = 100
    for i in range(3, MAX_N + 1):
        L[i] = L[i-1] + L[i-2]
        if L[i] > 10**30:
            for j in range(i+1, MAX_N + 1):
                L[j] = 10**30
            break

    def find_digit(k):
        """Find the k-th digit (1-indexed) of the infinite Fibonacci word."""
        n = 1
        while L[n] < k:
            n += 1
        # F(n) = F(n-2) . F(n-1)
        while n > 2:
            if k <= L[n-2]:
                n -= 2
            else:
                k -= L[n-2]
                n -= 1
        if n == 1:
            return int(A[k - 1])
        else:
            return int(B[k - 1])

    # Compute the answer
    answer = 0
    for n in range(18):
        k = (127 + 19 * n) * (7 ** n)
        digit = find_digit(k)
        answer += digit * (10 ** n)

    print(answer)

solve()
