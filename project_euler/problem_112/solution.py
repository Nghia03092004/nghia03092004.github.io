def is_bouncy(n):
    digits = []
    while n > 0:
        digits.append(n % 10)
        n //= 10
    digits.reverse()
    inc = all(digits[i] <= digits[i + 1] for i in range(len(digits) - 1))
    dec = all(digits[i] >= digits[i + 1] for i in range(len(digits) - 1))
    return not inc and not dec


def solve():
    bouncy = 0
    n = 0
    while True:
        n += 1
        if is_bouncy(n):
            bouncy += 1
        if 100 * bouncy == 99 * n:
            print(n)
            return


solve()
