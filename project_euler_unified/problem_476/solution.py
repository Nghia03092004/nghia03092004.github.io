import math

def triangle_area(a, b, c):
    s = (a + b + c) / 2.0
    val = s * (s - a) * (s - b) * (s - c)
    if val <= 0:
        return 0.0
    return math.sqrt(val)

def triangle_angles(a, b, c):
    A = math.acos(max(-1, min(1, (b*b + c*c - a*a) / (2.0*b*c))))
    B = math.acos(max(-1, min(1, (a*a + c*c - b*b) / (2.0*a*c))))
    C = math.pi - A - B
    return A, B, C

def malfatti_area(a, b, c):
    """Compute total area of Malfatti circles in triangle with sides a,b,c."""
    s = (a + b + c) / 2.0
    area = triangle_area(a, b, c)
    if area <= 0:
        return 0.0
    r = area / s
    A, B, C = triangle_angles(a, b, c)

    sA, sB, sC = math.sin(A/2), math.sin(B/2), math.sin(C/2)
    cA, cB, cC = math.cos(A/2), math.cos(B/2), math.cos(C/2)

    # Malfatti circle radii
    r1 = r / (2*cA*cA) * (cA + sB + sC - 1)
    r2 = r / (2*cB*cB) * (cB + sA + sC - 1)
    r3 = r / (2*cC*cC) * (cC + sA + sB - 1)

    return math.pi * (r1*r1 + r2*r2 + r3*r3)

def greedy_area(a, b, c):
    """Greedy packing: incircle + 2 largest corner circles."""
    s = (a + b + c) / 2.0
    area = triangle_area(a, b, c)
    if area <= 0:
        return 0.0
    r = area / s
    A, B, C = triangle_angles(a, b, c)

    # Corner circle radii
    def corner_r(angle):
        t = math.tan(angle / 2)
        si = math.sin(angle / 2)
        if si == 0:
            return 0.0
        return r * t / (1.0 + 1.0 / si)

    corners = sorted([corner_r(A), corner_r(B), corner_r(C)], reverse=True)
    return math.pi * (r*r + corners[0]**2 + corners[1]**2)

def solve():
    N = 1803
    total_R = 0.0
    count = 0

    for a in range(1, N + 1):
        for b in range(a, N + 1):
            if a + b > N:
                break
            cmax = min(a + b - 1, N)
            for c in range(b, cmax + 1):
                m = malfatti_area(a, b, c)
                g = greedy_area(a, b, c)
                total_R += max(m, g)
                count += 1

    S = total_R / count
    print(f"{S:.5f}")

if __name__ == "__main__":
    solve()
