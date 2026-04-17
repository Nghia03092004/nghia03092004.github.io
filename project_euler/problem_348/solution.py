import math

def solve():
    LIMIT = 10**9

    def generate_palindromes(limit):
        """Generate all palindromes up to limit in sorted order."""
        pals = []
        # Single digit
        for d in range(1, 10):
            pals.append(d)

        # Multi-digit: generate by half
        for length in range(2, 11):
            half_len = (length + 1) // 2
            start = 10 ** (half_len - 1)
            end = 10 ** half_len
            for h in range(start, end):
                s = str(h)
                if length % 2 == 0:
                    p = int(s + s[::-1])
                else:
                    p = int(s + s[-2::-1])
                if p >= limit:
                    break
                pals.append(p)

        pals.sort()
        return pals

    palindromes = generate_palindromes(LIMIT)

    results = []
    for pal in palindromes:
        if pal < 2:
            continue
        count = 0
        b = 1
        while b ** 3 < pal:
            rem = pal - b ** 3
            a = int(math.isqrt(rem))
            if a >= 1 and a * a == rem:
                count += 1
            b += 1
        if count == 4:
            results.append(pal)
            if len(results) == 5:
                break

    print(sum(results))

if __name__ == "__main__":
    solve()
