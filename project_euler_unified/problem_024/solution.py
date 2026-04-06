import math

def solve():
    k = 1000000 - 1
    digits = list(range(10))
    result = []
    for i in range(10):
        f = math.factorial(9 - i)
        idx, k = divmod(k, f)
        result.append(digits.pop(idx))
    print(''.join(map(str, result)))
