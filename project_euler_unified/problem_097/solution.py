"""
Project Euler Problem 97: Large Non-Mersenne Prime

Find the last ten digits of 28433 * 2^7830457 + 1.

Uses Python's built-in three-argument pow(base, exp, mod) which implements
binary exponentiation (repeated squaring) in O(log exp) modular multiplications.

Answer: 8739992577
"""


def main():
    mod = 10**10
    result = (28433 * pow(2, 7830457, mod) + 1) % mod
    print(result)


if __name__ == "__main__":
    main()
