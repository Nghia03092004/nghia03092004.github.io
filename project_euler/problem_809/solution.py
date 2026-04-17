"""
Problem 809: Rational Recurrence

$f(x)$: if $x$ integer, $f(x)=x$; if $x<1$, $f(x)=f(1/(1-x))$; else $f(x)=f(1/(\lceil x\rceil - x) - 1 + f(x-1))$. Given $f(3/2)=3$, $f(1/6)=65533$, $f(13/10)=7625597484985$. Find $f(22/7) \bmod 10^{1
"""

print("Problem 809: Rational Recurrence")
# Implementation sketch - see solution.md for full analysis
