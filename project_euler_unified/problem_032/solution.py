"""Project Euler Problem 32: Pandigital Products"""

def is_pandigital(a, b, p):
    s = str(a) + str(b) + str(p)
    return len(s) == 9 and set(s) == set('123456789')

def main():
    products = set()
    for a in range(1, 10):
        for b in range(1234, 9877):
            p = a * b
            if 1000 <= p <= 9999 and is_pandigital(a, b, p):
                products.add(p)
    for a in range(12, 99):
        for b in range(123, 988):
            p = a * b
            if 1000 <= p <= 9999 and is_pandigital(a, b, p):
                products.add(p)
    print(sum(products))  # 45228

if __name__ == "__main__":
    main()
