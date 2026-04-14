import math


def cutoff_index_regular_polygon(sides: int) -> int:
    theta = math.pi / sides
    tangent = math.tan(theta)

    def threshold_function(k: int) -> float:
        angle = k * theta
        return math.sin(angle) - (k + sides) * tangent * math.cos(angle)

    k = 0
    while threshold_function(k) < 0.0:
        k += 1
    return k - 1


def critical_speed_regular_polygon(sides: int) -> float:
    theta = math.pi / sides
    tangent = math.tan(theta)
    k = cutoff_index_regular_polygon(sides)

    angle = k * theta
    # Theorem 2 from the PDF gives V_n = sec(alpha).
    correction = math.acos(
        2.0 * math.sin(angle) / ((k + sides) * tangent) - math.cos(angle)
    )
    alpha = (angle + correction) / 2.0
    return 1.0 / math.cos(alpha)


def solve() -> str:
    return f"{critical_speed_regular_polygon(6):.8f}"


if __name__ == "__main__":
    print(solve())
