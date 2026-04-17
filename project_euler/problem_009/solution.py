s = 1000
for a in range(1, s // 3):
    num = s * s // 2 - s * a
    den = s - a
    if num % den == 0:
        b = num // den
        c = s - a - b
        if a < b < c:
            print(a * b * c)
            break
