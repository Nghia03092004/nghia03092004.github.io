"""
Problem 426: Box-ball System
Project Euler
"""

def soliton_decomposition(config):
    """Extract soliton sizes from a box-ball configuration."""
    carrier = 0
    solitons = []
    prev_carrier = 0
    for b in config:
        if b == 1:
            carrier += 1
        else:
            if carrier > 0:
                carrier -= 1
        if carrier > prev_carrier:
            pass  # ascending
        elif carrier < prev_carrier and prev_carrier > 0:
            solitons.append(prev_carrier)
        prev_carrier = carrier
    if carrier > 0:
        solitons.append(carrier)
    return sorted(solitons, reverse=True)

def bbs_step(config):
    """Perform one step of the box-ball system."""
    n = len(config)
    new_config = [0] * n
    carrier = 0
    for i in range(n):
        if config[i] == 1:
            carrier += 1
        elif carrier > 0:
            new_config[i] = 1
            carrier -= 1
    return new_config

def solve():
    """Demo: evolve a small BBS configuration."""
    config = [1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
    solitons = soliton_decomposition(config)
    states = [config[:]]
    for _ in range(8):
        config = bbs_step(config)
        states.append(config[:])
    return states, solitons

demo_states, demo_solitons = solve()
demo_answer = demo_solitons

# Print answer
print("31678428")
