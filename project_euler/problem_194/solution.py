"""
Project Euler Problem 194: Coloured Configurations

Count colourings of chains made from 25 A-units and 75 B-units, modulo 10^8.
"""

from math import comb


def unit_a_count(colors):
    c = colors
    return c**5 - 9 * c**4 + 34 * c**3 - 69 * c**2 + 77 * c - 38


def unit_b_count(colors):
    c = colors
    return c**5 - 8 * c**4 + 27 * c**3 - 50 * c**2 + 52 * c - 24


def configuration_count(a_units, b_units, colors):
    return (
        colors
        * (colors - 1)
        * comb(a_units + b_units, a_units)
        * unit_a_count(colors) ** a_units
        * unit_b_count(colors) ** b_units
    )


def solve():
    mod = 10**8
    return configuration_count(25, 75, 1984) % mod


if __name__ == "__main__":
    assert configuration_count(1, 0, 3) == 24
    assert configuration_count(0, 2, 4) == 92928
    assert configuration_count(2, 2, 3) == 20736
    answer = solve()
    assert answer == 61190912
    print(answer)
