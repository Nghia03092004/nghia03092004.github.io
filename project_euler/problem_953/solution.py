"""
Problem 953: Sorting Network Verification

Count binary inputs with balanced intermediate states in a 16-wire sorting
network. Specifically, for an odd-even merge sort network on 16 wires, count
how many of the C(16,8) balanced (8-zero, 8-one) inputs reach the sorted
target state [0]*8 + [1]*8 at some point during or after the network
application.

Key results:
    - Network has 63 comparators across multiple stages
    - Only balanced (8 zeros, 8 ones) inputs are considered
    - Answer is the count of such inputs that hit the sorted target

Methods:
    1. odd_even_merge_sort_network  — generate comparators recursively
    2. simulate_network             — run input through comparators, track states
    3. count_reaching_target        — brute-force all C(16,8) inputs
    4. stage_analysis               — determine at which stage target is first reached
"""

from math import comb

def odd_even_merge_sort_network(n):
    """Generate comparators for odd-even merge sort on n wires."""
    comps = []

    def merge(lo, hi, step):
        if step >= hi - lo:
            return
        if hi - lo == step:
            comps.append((lo, hi))
            return
        merge(lo, hi - step, step * 2)
        merge(lo + step, hi, step * 2)
        for i in range(lo + step, hi - step + 1, step * 2):
            comps.append((i, i + step))

    def sort_net(lo, hi):
        if hi - lo < 1:
            return
        mid = lo + (hi - lo) // 2
        sort_net(lo, mid)
        sort_net(mid + 1, hi)
        merge(lo, hi, 1)

    sort_net(0, n - 1)
    return comps

def simulate_network(bits, comps):
    """Run bits through comparators, return list of states after each step."""
    state = list(bits)
    states = [tuple(state)]
    for (i, j) in comps:
        if state[i] > state[j]:
            state[i], state[j] = state[j], state[i]
        states.append(tuple(state))
    return states

def count_reaching_target(n, comps, target):
    """Count balanced inputs that reach target at any intermediate step."""
    count = 0
    first_hit_stage = []
    for inp in range(2 ** n):
        bits = [(inp >> i) & 1 for i in range(n)]
        if sum(bits) != n // 2:
            continue
        states = simulate_network(bits, comps)
        for stage_idx, st in enumerate(states):
            if st == target:
                count += 1
                first_hit_stage.append(stage_idx)
                break
    return count, first_hit_stage

def stage_depth_analysis(comps, n):
    """Assign each comparator to a depth/stage based on wire availability."""
    wire_depth = [0] * n
    depths = []
    for (i, j) in comps:
        d = max(wire_depth[i], wire_depth[j]) + 1
        depths.append(d)
        wire_depth[i] = d
        wire_depth[j] = d
    return depths

# Verification with small cases
# A 4-wire network should sort all 2^4 inputs
comps_4 = odd_even_merge_sort_network(4)
for inp in range(16):
    bits = [(inp >> i) & 1 for i in range(4)]
    final = simulate_network(bits, comps_4)[-1]
    assert list(final) == sorted(bits), f"4-wire sort failed on {bits}"

# Verify C(16,8) total balanced inputs
assert comb(16, 8) == 12870

# Compute answer
n = 16
comps = odd_even_merge_sort_network(n)
target = tuple([0] * 8 + [1] * 8)
answer, first_hit_stages = count_reaching_target(n, comps, target)

print(f"Comparators: {len(comps)}")
print(f"C(16,8) = {comb(16, 8)}")
print(answer)
