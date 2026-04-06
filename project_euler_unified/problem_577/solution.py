def solve():
    N = 12345
    H = [0] * (N + 1)
    # Initial values from OEIS A011779
    H[3] = 1; H[4] = 3; H[5] = 6; H[6] = 12; H[7] = 21
    H[8] = 33; H[9] = 51; H[10] = 75; H[11] = 105

    # Linear recurrence: signature (3,-3,3,-6,6,-3,3,-3,1)
    for n in range(12, N + 1):
        H[n] = (3*H[n-1] - 3*H[n-2] + 3*H[n-3] - 6*H[n-4]
              + 6*H[n-5] - 3*H[n-6] + 3*H[n-7] - 3*H[n-8] + H[n-9])

    total = sum(H[3:N+1])
    print(total)

solve()
