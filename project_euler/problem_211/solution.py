import math
import numpy as np

def solve():
    N = 64_000_000
    sigma2 = np.zeros(N, dtype=np.uint64)

    for d in range(1, N):
        sigma2[d::d] += np.uint64(d * d)

    answer = 0
    for n in range(1, N):
        s = int(sigma2[n])
        r = math.isqrt(s)
        if r * r == s:
            answer += n

    print(answer)

if __name__ == "__main__":
    solve()
