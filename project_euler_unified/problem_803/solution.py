"""
Problem 803: Pseudorandom Sequence

Rand48 PRNG: $a_n = (25214903917 a_{n-1} + 11) \bmod 2^{48}$. Extract character: $b_n = \lfloor a_n/2^{16}\rfloor \bmod 52$ mapped to a-zA-Z. Given starting string 'PuzzleOne...', find position of 'Lu
"""

print("Problem 803: Pseudorandom Sequence")
# Implementation sketch - see solution.md for full analysis
