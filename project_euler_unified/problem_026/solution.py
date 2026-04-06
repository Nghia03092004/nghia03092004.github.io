def cycle_length(d):
    seen = {}
    r, pos = 1, 0
    while r != 0:
        if r in seen:
            return pos - seen[r]
        seen[r] = pos
        r = (r * 10) % d
        pos += 1
    return 0

def solve():
    best_d, best_len = 0, 0
    for d in range(2, 1000):
        cl = cycle_length(d)
        if cl > best_len:
            best_len = cl
            best_d = d
    print(best_d)

if __name__ == "__main__":
    solve()
