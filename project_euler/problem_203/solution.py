"""
Problem 203: Squarefree Binomial Coefficients

Find the sum of all distinct squarefree numbers in the first 51 rows
(rows 0 through 50) of Pascal's triangle.

By Kummer's theorem, for n <= 50, primes >= 11 can appear at most once
in any binomial coefficient. So squarefreeness reduces to checking
divisibility by 4, 9, 25, and 49.
"""

def solve():
    N = 50
    vals = set()

    # Build Pascal's triangle
    row = [1]
    vals.add(1)
    for n in range(1, N + 1):
        new_row = [1]
        for k in range(1, n):
            new_row.append(row[k-1] + row[k])
        new_row.append(1)
        row = new_row
        vals.update(row)

    # Check squarefreeness (only need to test p^2 for p in {2,3,5,7})
    squares = [4, 9, 25, 49]
    ans = sum(v for v in vals if all(v % sq != 0 for sq in squares))
    print(ans)

if __name__ == "__main__":
    solve()
