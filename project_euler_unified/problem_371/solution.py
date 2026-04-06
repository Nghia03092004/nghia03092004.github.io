"""
Problem 371: Licence Plates
Expected number of plates to observe before seeing a pair summing to 1000.
"""

def solve():
    # E(k, s): expected additional plates from state (k, s)
    # k = number of active pairs, s = whether 500 has been seen
    # 499 complementary pairs: (1,999), (2,998), ..., (499,501)
    # 500 is special: 500+500=1000
    # 000 is irrelevant (no plate 1000)

    E0 = [0.0] * 500  # s=0
    E1 = [0.0] * 500  # s=1

    E1[499] = 1000.0 / 500.0
    E0[499] = (1000.0 + E1[499]) / 500.0

    for k in range(498, -1, -1):
        E1[k] = (1000.0 + 2.0 * (499 - k) * E1[k + 1]) / (999.0 - k)
        E0[k] = (1000.0 + E1[k] + 2.0 * (499 - k) * E0[k + 1]) / (999.0 - k)

    print(f"{E0[0]:.8f}")

if __name__ == "__main__":
    solve()
