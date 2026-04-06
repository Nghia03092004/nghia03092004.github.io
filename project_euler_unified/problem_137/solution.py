"""
Problem 137: Fibonacci Golden Nuggets

The k-th golden nugget is F(2k) * F(2k+1), where F is the Fibonacci sequence.
Find the 15th golden nugget.
"""

def solve():
    # Compute Fibonacci numbers
    fib = [0] * 32
    fib[1] = 1
    fib[2] = 1
    for i in range(3, 32):
        fib[i] = fib[i-1] + fib[i-2]

    # k-th golden nugget = F(2k) * F(2k+1)
    k = 15
    answer = fib[2*k] * fib[2*k + 1]
    return answer

if __name__ == "__main__":
    answer = solve()
assert answer == 1120149658760
print(answer)
