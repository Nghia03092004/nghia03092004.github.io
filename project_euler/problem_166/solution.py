"""
Problem 166: Criss Cross

Count 4x4 grids of digits 0-9 where all rows, columns, and both diagonals
have the same sum S.

Meet-in-the-middle: enumerate top 2 rows and bottom 2 rows separately,
then match on column sums and diagonal partial sums.
"""

from collections import defaultdict

def solve():
    total = 0

    for S in range(37):
        # Generate all 4-tuples of digits summing to S
        tuples = []
        for a in range(min(10, S+1)):
            for b in range(min(10, S-a+1)):
                for c in range(min(10, S-a-b+1)):
                    d = S - a - b - c
                    if 0 <= d <= 9:
                        tuples.append((a, b, c, d))

        # Top half: rows 1 and 2 (a,b,c,d) and (e,f,g,h)
        # Track: col sums (a+e, b+f, c+g, d+h), diag partials (a+f, d+g)
        top_map = defaultdict(int)
        for t1 in tuples:
            for t2 in tuples:
                key = (
                    t1[0]+t2[0], t1[1]+t2[1], t1[2]+t2[2], t1[3]+t2[3],
                    t1[0]+t2[1],  # a + f
                    t1[3]+t2[2],  # d + g
                )
                top_map[key] += 1

        # Bottom half: rows 3 and 4 (i,j,k,l) and (m,n,o,p)
        # Need: col sums i+m = S - top_c0, etc.
        # Diag: k+p = S - (a+f), j+m = S - (d+g)
        for t3 in tuples:
            for t4 in tuples:
                bc0 = t3[0]+t4[0]
                bc1 = t3[1]+t4[1]
                bc2 = t3[2]+t4[2]
                bc3 = t3[3]+t4[3]
                bd0 = t3[2]+t4[3]  # k + p
                bd1 = t3[1]+t4[0]  # j + m

                needed = (S-bc0, S-bc1, S-bc2, S-bc3, S-bd0, S-bd1)
                if any(v < 0 or v > 18 for v in needed):
                    continue
                total += top_map.get(needed, 0)

    print(total)

solve()
