def can_split(s, pos, target, has_split):
    """Check if s[pos:] can be split into parts summing to target."""
    if pos == len(s):
        return target == 0 and has_split
    if target < 0:
        return False

    val = 0
    for i in range(pos, len(s)):
        val = val * 10 + int(s[i])
        if val > target:
            break
        # Need at least 2 parts total
        if i == len(s) - 1 and not has_split:
            continue
        if can_split(s, i + 1, target - val, True):
            return True
    return False

def T(N):
    limit = int(N ** 0.5)
    total = 0
    for k in range(2, limit + 1):
        # Mod 9 pruning
        r = k % 9
        if r != 0 and r != 1:
            continue
        n = k * k
        s = str(n)
        if can_split(s, 0, k, False):
            total += n
    return total

# Verify with small case
assert T(10**4) == 41333, f"T(10^4) = {T(10**4)}, expected 41333"
print(f"T(10^4) = {T(10**4)}")

# Compute the answer
result = T(10**12)
print(f"T(10^12) = {result}")
