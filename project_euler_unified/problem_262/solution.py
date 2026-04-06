"""
Problem 262: Mountain Range

h(x,y) = (5000 - 0.005*(x^2+y^2+x*y) + 12.5*(x+y))
         * exp(-abs(0.000001*(x^2+y^2) - 0.0015*(x+y) + 0.7))

A mosquito flies from A(200,200) to B(1400,1400) within 0<=x,y<=1600.
It ascends to elevation f, flies horizontally at f, then descends.

Find f_min (minimum elevation allowing such trip), then the shortest
horizontal path at f_min, rounded to 3 decimal places.

Solution approach:
1. f_min = minimax elevation = 10396.462193 (peak on y=0 boundary at x=895.483).
2. The barrier {h > f_min} is an annular (ring-shaped) region.
   It has a "pinch point" at (895.483, 0) where the barrier width is zero.
   By symmetry h(x,y)=h(y,x), another pinch exists at (0, 895.483).
3. The shortest path from A to B goes:
   A -> PA -> arc(PA, pinch) -> arc(pinch, Q) -> Q -> B
   where PA is tangent from A to the outer contour (left branch),
   and Q is tangent from B to the outer contour (right branch).
   The arc goes through the pinch point at (895.483, 0).
4. PA = (624.650, 48.254), Q = (1536.043, 873.038).
5. Path = 450.948 + 276.452 + 1259.565 + 544.239 = 2531.205.

Answer: 2531.205
"""

import math
from scipy.optimize import brentq, minimize_scalar

def h(x, y):
    t1 = x*x + y*y + x*y
    t2 = x*x + y*y
    base = 5000.0 - 0.005*t1 + 12.5*(x+y)
    ex = abs(0.000001*t2 - 0.0015*(x+y) + 0.7)
    return base * math.exp(-ex)

def dh_dx(x, y, eps=1e-7):
    return (h(x+eps, y) - h(x-eps, y)) / (2*eps)

def dh_dy(x, y, eps=1e-7):
    return (h(x, y+eps) - h(x, y-eps)) / (2*eps)

def outer_left_x(y, f_min):
    """First (leftmost) crossing of h=f_min at given y."""
    prev = h(0, y) - f_min
    for xi in range(1, 1601):
        cur = h(xi, y) - f_min
        if prev * cur < 0:
            return brentq(lambda x: h(x, y) - f_min, xi-1, xi)
        prev = cur
    return None

def outer_right_x(y, f_min):
    """Last (rightmost) crossing of h=f_min at given y."""
    prev = h(1600, y) - f_min
    for xi in range(1599, -1, -1):
        cur = h(xi, y) - f_min
        if prev * cur < 0:
            return brentq(lambda x: h(x, y) - f_min, xi, xi+1)
        prev = cur
    return None

def trace_arc(x0, y0, x1, y1, f_target, step=0.05, max_steps=200000):
    """Trace contour h=f_target from (x0,y0) toward (x1,y1), return arc length."""
    x, y = x0, y0
    total = 0.0
    for _ in range(max_steps):
        gx = dh_dx(x, y)
        gy = dh_dy(x, y)
        t1x, t1y = -gy, gx
        t2x, t2y = gy, -gx
        dx_t, dy_t = x1-x, y1-y
        if t1x*dx_t + t1y*dy_t > t2x*dx_t + t2y*dy_t:
            tx, ty = t1x, t1y
        else:
            tx, ty = t2x, t2y
        tn = math.sqrt(tx*tx + ty*ty)
        if tn < 1e-12: break
        tx /= tn; ty /= tn
        nx = x + step*tx
        ny = y + step*ty
        for _ in range(5):
            hv = h(nx, ny)
            gx2 = dh_dx(nx, ny)
            gy2 = dh_dy(nx, ny)
            g2 = gx2*gx2 + gy2*gy2
            if g2 < 1e-20: break
            corr = (hv - f_target) / g2
            nx -= corr * gx2
            ny -= corr * gy2
        total += math.sqrt((nx-x)**2 + (ny-y)**2)
        x, y = nx, ny
        if math.sqrt((x-x1)**2 + (y-y1)**2) < step * 2:
            total += math.sqrt((x-x1)**2 + (y-y1)**2)
            break
    return total

# Step 1: Find f_min (max of h on y=0 axis)
res = minimize_scalar(lambda x: -h(x, 0), bounds=(800, 1000), method='bounded')
f_min = -res.fun
pinch = (res.x, 0.0)
print(f"f_min = {f_min:.6f}")
print(f"Pinch point: ({pinch[0]:.3f}, {pinch[1]:.3f})")

# Step 2: Find tangent point PA from A(200,200) to left branch
def tangent_residual_A(y):
    xl = outer_left_x(y, f_min)
    if xl is None: return 1e10
    gx = dh_dx(xl, y)
    gy = dh_dy(xl, y)
    return gx*(xl-200) + gy*(y-200)

y_PA = brentq(tangent_residual_A, 40, 50)
x_PA = outer_left_x(y_PA, f_min)
PA = (x_PA, y_PA)
print(f"PA = ({PA[0]:.6f}, {PA[1]:.6f})")

# Step 3: Find tangent point Q from B(1400,1400) to right branch
def tangent_residual_B(y):
    xr = outer_right_x(y, f_min)
    if xr is None: return 1e10
    gx = dh_dx(xr, y)
    gy = dh_dy(xr, y)
    return gx*(xr-1400) + gy*(y-1400)

y_Q = brentq(tangent_residual_B, 870, 875)
x_Q = outer_right_x(y_Q, f_min)
Q = (x_Q, y_Q)
print(f"Q = ({Q[0]:.6f}, {Q[1]:.6f})")

# Step 4: Compute path lengths
d_APA = math.sqrt((200-PA[0])**2 + (200-PA[1])**2)
arc1 = trace_arc(PA[0], PA[1], pinch[0], pinch[1], f_min)
arc2 = trace_arc(pinch[0], pinch[1], Q[0], Q[1], f_min)
d_QB = math.sqrt((Q[0]-1400)**2 + (Q[1]-1400)**2)

total = d_APA + arc1 + arc2 + d_QB
print(f"\nPath breakdown:")
print(f"  d(A, PA) = {d_APA:.3f}")
print(f"  arc(PA, pinch) = {arc1:.3f}")
print(f"  arc(pinch, Q) = {arc2:.3f}")
print(f"  d(Q, B) = {d_QB:.3f}")
print(f"  Total = {total:.3f}")
print(f"\n{total:.3f}")
