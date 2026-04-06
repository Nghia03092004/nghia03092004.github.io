"""
Problem 102: Triangle Containment

Count how many of 1000 triangles contain the origin (0,0).
Uses the cross-product sign test.
"""
import os
import urllib.request


def contains_origin(x1, y1, x2, y2, x3, y3):
    d1 = x1 * y2 - x2 * y1
    d2 = x2 * y3 - x3 * y2
    d3 = x3 * y1 - x1 * y3
    has_neg = (d1 < 0) or (d2 < 0) or (d3 < 0)
    has_pos = (d1 > 0) or (d2 > 0) or (d3 > 0)
    return not (has_neg and has_pos)


def get_data():
    local_file = os.path.join(os.path.dirname(os.path.abspath(__file__)),
                              "p102_triangles.txt")
    if os.path.exists(local_file):
        with open(local_file) as f:
            return f.read()
    url = "https://projecteuler.net/resources/documents/0102_triangles.txt"
    response = urllib.request.urlopen(url)
    data = response.read().decode("utf-8")
    with open(local_file, "w") as f:
        f.write(data)
    return data


def solve():
    data = get_data()
    count = 0
    for line in data.strip().split("\n"):
        x1, y1, x2, y2, x3, y3 = map(int, line.strip().split(","))
        if contains_origin(x1, y1, x2, y2, x3, y3):
            count += 1
    return count


if __name__ == "__main__":
    answer = solve()
    print(answer)
    assert answer == 228
