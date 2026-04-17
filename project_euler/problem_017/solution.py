"""Project Euler Problem 17: Number Letter Counts"""

def letter_count(n):
    ones = [0, 3, 3, 5, 4, 4, 3, 5, 5, 4]
    teens = [3, 6, 6, 8, 8, 7, 7, 9, 8, 8]
    tens = [0, 0, 6, 6, 5, 5, 5, 7, 6, 6]
    if n == 1000:
        return 11
    c = 0
    if n >= 100:
        c += ones[n // 100] + 7
        n %= 100
        if n > 0:
            c += 3
    if n >= 20:
        c += tens[n // 10]
        n %= 10
    if 10 <= n <= 19:
        c += teens[n - 10]
        n = 0
    if 1 <= n <= 9:
        c += ones[n]
    return c

def main():
    print(sum(letter_count(n) for n in range(1, 1001)))
