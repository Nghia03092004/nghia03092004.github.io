"""
Problem 802: Iterated Composition
Let $f(x) = x^2 - x + 1$. Define $f_1(x)=f(x)$ and $f_{n+1}(x)=f(f_n(x))$. Let $g(n)$ be the number of real solutions to $f_n(x)=x$. Find $\sum_{n=1}^{30} g(n)$.
"""

def solve():
    """Count real solutions of f_n(x) = x for f(x) = x^2 - x + 1."""
    # For c = 3/4 in the Mandelbrot set, the dynamics have limited real periodic orbits
    # f(x) = x^2 - x + 1, fixed point x=1 (double)
    # f_2(x) = x has the same fixed point; check for 2-cycles
    # f(x) = y, f(y) = x => system gives no real 2-cycles when c > 1/4

    # For this quadratic map, all periodic orbits beyond period 1 are complex
    # g(n) = 1 for all n (only the fixed point x=1)
    total = sum(1 for n in range(1, 31))  # g(n) = 1 for each n
    return total

answer = solve()
print(answer)
