"""
Problem 126: Cuboid Layers

Find the least n for which C(n) = 1000, where C(n) counts
the number of cuboids that have exactly n cubes in one of its layers.

Layer formula: f(a,b,c,k) = 2(ab+bc+ac) + 4(a+b+c)(k-1) + 4(k-1)(k-2)
"""

def solve(target=1000):
    LIMIT = 20000
    C = [0] * (LIMIT + 1)

    a = 1
    while 2 * a * a <= LIMIT:
        b = a
        while 2 * (a * b + b * b) <= LIMIT:
            c = b
            while 2 * (a * b + b * c + a * c) <= LIMIT:
                base = 2 * (a * b + b * c + a * c)
                edge = 4 * (a + b + c)
                k = 1
                while True:
                    val = base + edge * (k - 1) + 4 * (k - 1) * (k - 2)
                    if val > LIMIT:
                        break
                    C[val] += 1
                    k += 1
                c += 1
            b += 1
        a += 1

    for n in range(1, LIMIT + 1):
        if C[n] == target:
            return n
    return -1

def visualize():
    """Optional visualization of C(n) distribution."""
