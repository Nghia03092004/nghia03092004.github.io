"""Project Euler Problem 39: Integer Right Triangles"""

def solve():
    LIMIT = 1000
    best_p, best_count = 0, 0
    for p in range(2, LIMIT + 1, 2):
        count = 0
        for a in range(1, p // 3):
            num = p * (p - 2 * a)
            den = 2 * (p - a)
            if num % den == 0 and num // den >= a:
                count += 1
        if count > best_count:
            best_count = count
            best_p = p
    return best_p

answer = solve()
assert answer == 840
print(answer)
