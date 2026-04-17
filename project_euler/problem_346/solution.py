def solve():
    LIMIT = 10**12
    repunits = set()
    repunits.add(1)

    # Enumerate repunits with k >= 3 digits in base b >= 2
    # Each such number is also "11" in base (n-1), making it a strong repunit
    for k in range(3, 41):
        b = 2
        while True:
            # Compute 1 + b + b^2 + ... + b^(k-1)
            val = (b**k - 1) // (b - 1)
            if val >= LIMIT:
                break
            repunits.add(val)
            b += 1

    print(sum(repunits))

if __name__ == "__main__":
    solve()
