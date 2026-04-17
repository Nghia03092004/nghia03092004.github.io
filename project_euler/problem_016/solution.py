"""Project Euler Problem 16: Power Digit Sum"""

def main():
    print(sum(int(d) for d in str(2 ** 1000)))
