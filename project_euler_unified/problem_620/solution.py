"""
Problem 620: Planetary Gears
Analyze gear ratios in planetary gear systems.
"""

def planetary_gear_ratio(sun_teeth, ring_teeth):
    """Gear ratio of a planetary gear system.
    With fixed ring: ratio = 1 + ring/sun
    With fixed carrier: ratio = -ring/sun
    With fixed sun: ratio = 1 + sun/ring ... but inverted
    """
    return 1 + ring_teeth / sun_teeth

def find_integer_ratios(max_teeth=100):
    """Find sun/ring combinations giving integer gear ratios."""
    results = []
    for s in range(1, max_teeth + 1):
        for r in range(s + 1, max_teeth + 1):
            # Planet teeth = (r - s) / 2, must be positive integer
            if (r - s) % 2 != 0:
                continue
            p = (r - s) // 2
            if p < 1:
                continue
            ratio = 1 + r / s
            if abs(ratio - round(ratio)) < 1e-10:
                results.append((s, p, r, int(round(ratio))))
    return results

results = find_integer_ratios(100)
print(f"Found {len(results)} integer-ratio planetary gear configs")
for s, p, r, ratio in results[:15]:
    print(f"  Sun={s}, Planet={p}, Ring={r}, Ratio={ratio}")
