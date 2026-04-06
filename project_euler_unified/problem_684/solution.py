MOD = 1_000_000_007

def power(base, exp, mod):
    return pow(base, exp, mod)

def S(k):
    if k <= 0:
        return 0
    Q = k // 9
    R = k % 9

    p10 = power(10, Q, MOD)
    coeff = (5 + (R + 1) * (R + 2) // 2) % MOD

    result = p10 * coeff % MOD
    result = (result - 9 * (Q % MOD) % MOD + MOD) % MOD
    result = (result - R % MOD + MOD) % MOD
    result = (result - 6 + MOD) % MOD

    return result

# Compute Fibonacci numbers
fib = [0] * 91
fib[1] = 1
for i in range(2, 91):
    fib[i] = fib[i-1] + fib[i-2]

ans = 0
for i in range(2, 91):
    ans = (ans + S(fib[i])) % MOD

print(ans)
