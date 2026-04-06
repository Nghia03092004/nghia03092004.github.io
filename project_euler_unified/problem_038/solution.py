"""Project Euler Problem 38: Pandigital Multiples"""

def is_pandigital(s):
    return len(s) == 9 and set(s) == set("123456789")

best = 0
for x in range(1, 10000):
    concat = ""
    for n in range(1, 10):
        concat += str(x * n)
        if len(concat) > 9:
            break
        if n >= 2 and len(concat) == 9 and is_pandigital(concat):
            best = max(best, int(concat))

print(best)
