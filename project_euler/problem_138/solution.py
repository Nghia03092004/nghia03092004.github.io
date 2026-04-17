"""
Problem 138: Special Isosceles Triangles

The L values satisfy the recurrence L_{n+1} = 18*L_n - L_{n-1}
with L_1 = 17, L_2 = 305.
Sum the first 12 values.
"""

def solve():
    L_prev, L_curr = 17, 305
    total = L_prev + L_curr
    for _ in range(10):  # 10 more to get 12 total
        L_prev, L_curr = L_curr, 18 * L_curr - L_prev
        total += L_curr
    return total

if __name__ == "__main__":
    answer = solve()
assert answer == 1118049290473932
print(answer)
