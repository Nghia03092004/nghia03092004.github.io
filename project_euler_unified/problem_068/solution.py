"""
Project Euler Problem 68: Magic 5-gon Ring

Find the maximum 16-digit string for a magic 5-gon ring using numbers 1-10.

By Theorem 2, 10 must be outer for a 16-digit string.
By Theorem 3, inner = {1,2,3,4,5}, outer = {6,7,8,9,10}, S = 14.
"""

from itertools import permutations


def main():
    max_str = ""

    for inner in permutations([1, 2, 3, 4, 5]):
        outer = []
        valid = True
        for k in range(5):
            o = 14 - inner[k] - inner[(k + 1) % 5]
            if o < 6 or o > 10:
                valid = False
                break
            outer.append(o)

        if not valid or sorted(outer) != [6, 7, 8, 9, 10]:
            continue

        start = outer.index(min(outer))

        s = ""
        for k in range(5):
            idx = (start + k) % 5
            s += str(outer[idx])
            s += str(inner[idx])
            s += str(inner[(idx + 1) % 5])

        if len(s) == 16 and s > max_str:
            max_str = s

    print(max_str)


if __name__ == "__main__":
    main()
