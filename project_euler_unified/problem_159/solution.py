LIMIT = 1000000

def dr(n):
    """Digital root of n."""
    if n == 0:
        return 0
    r = n % 9
    return 9 if r == 0 else r

# Initialize mdrs[n] = dr(n)
mdrs = [0] * LIMIT
for n in range(2, LIMIT):
    mdrs[n] = dr(n)

# Sieve: for each factor a >= 2, update all multiples a*j where j >= a
a = 2
while a * a < LIMIT:
    j = a
    while a * j < LIMIT:
        n = a * j
        val = dr(a) + mdrs[j]
        if val > mdrs[n]:
            mdrs[n] = val
        j += 1
    a += 1

total = sum(mdrs[2:])
print(total)
