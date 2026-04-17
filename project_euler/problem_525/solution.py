"""
Problem 525: Rolling Ellipse
An ellipse rolls along the x-axis. Track the center's path (elliptic trochoid).
"""

import numpy as np
from scipy import integrate

def ellipse_arc_length(a: float, b: float, theta: float) -> float:
    """Arc length of ellipse from 0 to theta."""
    result, _ = integrate.quad(
        lambda t: np.sqrt(a**2 * np.sin(t)**2 + b**2 * np.cos(t)**2),
        0, theta
    )
    return result

def ellipse_circumference(a: float, b: float) -> float:
    """Full circumference of ellipse."""
    return ellipse_arc_length(a, b, 2 * np.pi)

def rolling_ellipse_center(a: float, b: float, n_points: int = 1000):
    """
    Compute the path of the center of an ellipse rolling on the x-axis.

    When the contact point is at parameter theta on the ellipse:
    - Distance along x-axis = arc length s(theta)
    - Height of center = ab / sqrt(a^2 sin^2(theta) + b^2 cos^2(theta))
    - Horizontal offset of center from contact = center projects to contact

    The center position:
    X_c = s(theta) - (a cos(theta) * b sin(theta) - b sin(theta) * a cos(theta)) ...
    Actually, using the rolling constraint more carefully:

    At contact parameter theta:
    - tangent direction at theta: (-a sin(theta), b cos(theta)) (normalized)
    - normal direction (inward): (-b cos(theta), -a sin(theta)) (normalized)
    - distance from center to tangent line: h = ab / sqrt(a^2 sin^2 + b^2 cos^2)

    Center position:
    X_c(theta) = s(theta) + offset along x
    Y_c(theta) = h(theta)
    """
    thetas = np.linspace(0, 2 * np.pi, n_points)

    X_c = np.zeros(n_points)
    Y_c = np.zeros(n_points)

    for i, theta in enumerate(thetas):
        # Arc length up to theta
        s = ellipse_arc_length(a, b, theta)

        # Height of center above x-axis
        denom = np.sqrt(a**2 * np.sin(theta)**2 + b**2 * np.cos(theta)**2)
        h = a * b / denom

        # The tangent to the ellipse at theta makes angle alpha with x-axis
        tx = -a * np.sin(theta)
        ty = b * np.cos(theta)
        tangent_len = np.sqrt(tx**2 + ty**2)

        # Normal from contact point to center
        # Center relative to contact point on ellipse: (-a*cos(theta), -b*sin(theta)) relative
        # rotated so tangent aligns with x-axis
        # The contact is at bottom, tangent is horizontal

        # Angle of tangent with horizontal
        alpha = np.arctan2(ty, tx)

        # Center offset from contact point (in ellipse frame): (-a cos theta, -b sin theta)
        # from contact point which is at (a cos theta, b sin theta) to center (0,0)
        dx_ell = -a * np.cos(theta)
        dy_ell = -b * np.sin(theta)

        # Rotate by -(alpha) to align tangent with x-axis, then by pi (since rolling on bottom)
        # Actually the tangent at the contact should be horizontal (aligned with x-axis)
        # Rotation angle to make tangent horizontal: -alpha (or pi - alpha)
        rot_angle = -alpha + np.pi / 2  # align tangent with x-axis

        cos_r = np.cos(rot_angle)
        sin_r = np.sin(rot_angle)

        dx_world = dx_ell * cos_r - dy_ell * sin_r
        dy_world = dx_ell * sin_r + dy_ell * cos_r

        X_c[i] = s + dx_world
        Y_c[i] = dy_world

    # Ensure the center starts at the correct height
    # At theta=0, contact at (a, 0), center should be at height b
    # Adjust if needed
    Y_c = np.abs(Y_c)  # center should be above x-axis

    return thetas, X_c, Y_c

# Parameters
a, b = 2.0, 1.0
circ = ellipse_circumference(a, b)
print(f"Ellipse a={a}, b={b}")
print(f"Circumference = {circ:.6f}")

thetas, X_c, Y_c = rolling_ellipse_center(a, b, n_points=500)

# Compute arc length of center path
dX = np.diff(X_c)
dY = np.diff(Y_c)
center_arc_length = np.sum(np.sqrt(dX**2 + dY**2))
print(f"Center path arc length (one revolution) = {center_arc_length:.6f}")
