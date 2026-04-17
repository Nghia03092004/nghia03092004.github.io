"""
Project Euler Problem 389: Platonic Dice
=========================================

Find the variance of I, where the chain is:
    T ~ d4
    C = sum of T d6
    O = sum of C d8
    D = sum of O d12
    I = sum of D d20

Uses the law of total variance applied recursively:
    E[sum]   = E[Y] * (n+1)/2
    Var[sum] = E[Y] * (n^2-1)/12 + Var[Y] * ((n+1)/2)^2

Answer: 2406376.3623
"""

from __future__ import annotations

def die_mean(n: int) -> float:
    """Expected value of a single fair n-sided die."""
    return (n + 1) / 2

def die_var(n: int) -> float:
    """Variance of a single fair n-sided die."""
    return (n ** 2 - 1) / 12

def propagate(e_y: float, v_y: float, n: int):
    """
    Given Y with mean e_y and variance v_y, compute the mean and variance
    of the sum of Y independent fair n-sided dice.

    Parameters
    ----------
    e_y : float
        E[Y], the expected number of dice.
    v_y : float
        Var(Y), the variance of the number of dice.
    n : int
        Number of sides on each die.

    Returns
    -------
    (E[sum], Var(sum))
    """
    mu = die_mean(n)
    sigma2 = die_var(n)
    e_sum = e_y * mu
    v_sum = e_y * sigma2 + v_y * mu ** 2
    return e_sum, v_sum

def solve() -> float:
    """Compute Var(I) through the five Platonic dice layers."""
    dice_sides = [4, 6, 8, 12, 20]

    # Layer 0: single d4
    e = die_mean(dice_sides[0])
    v = die_var(dice_sides[0])

    # Layers 1-4: each layer rolls previous-sum dice of the next type
    for n in dice_sides[1:]:
        e, v = propagate(e, v, n)

    return round(v, 4)

def collect_layer_data():
    """Collect mean and variance at each layer for visualization."""
    dice_sides = [4, 6, 8, 12, 20]
    labels = ["T (d4)", "C (d6)", "O (d8)", "D (d12)", "I (d20)"]

    e = die_mean(dice_sides[0])
    v = die_var(dice_sides[0])
    layers = [{"label": labels[0], "mean": e, "variance": v}]

    for i, n in enumerate(dice_sides[1:], start=1):
        e, v = propagate(e, v, n)
        layers.append({"label": labels[i], "mean": e, "variance": v})

    return layers

def visualize(output_path: str) -> None:
    """Create a visualization of how mean and variance grow across layers."""
    layers = collect_layer_data()
    labels = [d["label"] for d in layers]
    means = [d["mean"] for d in layers]
    variances = [d["variance"] for d in layers]
