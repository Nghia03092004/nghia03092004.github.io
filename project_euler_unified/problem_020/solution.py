"""Project Euler Problem 20: Factorial Digit Sum"""

import math

def main():
    print(sum(int(d) for d in str(math.factorial(100))))
