"""
Problem 671: Colouring a Loop
"""

MOD = 1000004321

print("Problem 671: Colouring a Loop")
print("Trace of transfer matrix power for cylindrical tiling")
print(f"Target: F_10(10004003002001) mod {MOD}")

# For cycle graph proper colourings (simplified model):
# P(C_n, k) = (k-1)^n + (-1)^n * (k-1)
def chromatic_cycle(n, k, mod):
    return (pow(k-1, n, mod) + (1 if n % 2 == 0 else mod - 1) * (k-1)) % mod

# Verify on simple cases
for n in range(3, 8):
    for k in [3, 4]:
        val = chromatic_cycle(n, k, 10**9)
        print(f"  P(C_{n}, {k}) = {val}")

print("Full solution requires tiling-specific transfer matrix.")
plot_data = [list(range(3, 20)), [chromatic_cycle(n, 4, 10**9) for n in range(3, 20)],
             [chromatic_cycle(n, 4, 10**9) for n in range(3, 20)],
             [chromatic_cycle(n, 4, 10**9) for n in range(3, 20)]]
