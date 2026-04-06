"""
Problem 147: Rectangles in Cross-hatched Grids

Find the total number of rectangles (axis-aligned and diagonal) that could be
situated within cross-hatched grids of sizes w x h, for 1 <= w <= 47, 1 <= h <= 43.

The answer is the SUM over all sub-grid sizes (w, h) of the rectangle counts.
"""

def diagonal(width, height):
    """Count diagonal (tilted 45-degree) rectangles in a width x height grid.

    Uses doubled coordinates to avoid floating point.
    Two types of diagonal unit squares exist based on parity:
    - Type 0: center has integer x, half-integer y
    - Type 1: center has half-integer x, integer y
    For each starting square, scan along rotated axes to enumerate valid rectangles.
    """
    a, b = max(width, height), min(width, height)
    count = 0

    for i in range(a):
        for j in range(b):
            for parity in range(2):
                startX = 2 * i + 1 + parity
                startY = 2 * j + 2 - parity

                stop = False
                max_height = float('inf')

                cw = 0
                while not stop:
                    currentX = startX + cw
                    currentY = startY - cw
                    if currentY <= 0:
                        break

                    ch = 0
                    while ch < max_height:
                        endX = currentX + ch
                        endY = currentY + ch
                        if endX >= 2 * a or endY >= 2 * b:
                            if max_height > ch:
                                max_height = ch
                            stop = (ch == 0)
                            break
                        count += 1
                        ch += 1
                    cw += 1

    return count

def solve():
    M, N = 47, 43

    cache = {}
    total = 0

    for w in range(1, M + 1):
        for h in range(1, N + 1):
            # Axis-aligned rectangles
            axis = w * (w + 1) // 2 * (h * (h + 1) // 2)

            # Diagonal rectangles (cache with sorted key)
            key = (min(w, h), max(w, h))
            if key not in cache:
                cache[key] = diagonal(w, h)
            diag = cache[key]

            total += axis + diag

    print(total)

if __name__ == "__main__":
    solve()
