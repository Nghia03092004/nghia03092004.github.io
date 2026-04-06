"""Project Euler Problem 19: Counting Sundays"""

def main():
    days = [31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31]
    is_leap = lambda y: (y % 4 == 0 and y % 100 != 0) or y % 400 == 0

    w, count = 1, 0  # 1 Jan 1900 = Monday (1); Sunday = 0
    for y in range(1900, 2001):
        for m in range(12):
            if y >= 1901 and w == 0:
                count += 1
            d = 29 if m == 1 and is_leap(y) else days[m]
            w = (w + d) % 7
    print(count)
