import math

def solve():
    log_phi = math.log10((1 + math.sqrt(5)) / 2)
    h = 0.5 * math.log10(5)
    n = math.ceil((999 + h) / log_phi)

    # Verify iteratively
    a, b = 1, 1
    idx = 2
    while len(str(b)) < 1000:
        a, b = b, a + b
        idx += 1

    assert n == idx
    print(n)
