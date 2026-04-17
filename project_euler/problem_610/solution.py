"""
Problem 610: Roman Numerals II
Count valid representations in modified Roman numeral system.
"""

def roman_to_int(s):
    """Convert Roman numeral string to integer."""
    values = {'I': 1, 'V': 5, 'X': 10, 'L': 50, 'C': 100, 'D': 500, 'M': 1000}
    total = 0
    for i in range(len(s)):
        if i + 1 < len(s) and values[s[i]] < values[s[i+1]]:
            total -= values[s[i]]
        else:
            total += values[s[i]]
    return total

def int_to_roman(n):
    """Convert integer to standard Roman numeral."""
    vals = [(1000,'M'),(900,'CM'),(500,'D'),(400,'CD'),
            (100,'C'),(90,'XC'),(50,'L'),(40,'XL'),
            (10,'X'),(9,'IX'),(5,'V'),(4,'IV'),(1,'I')]
    result = ''
    for val, sym in vals:
        while n >= val:
            result += sym
            n -= val
    return result

# Verify
assert roman_to_int("XIV") == 14
assert roman_to_int("MCMXCIV") == 1994
assert int_to_roman(1994) == "MCMXCIV"

# Count lengths of Roman numerals
lengths = {}
for n in range(1, 4000):
    r = int_to_roman(n)
    lengths[n] = len(r)

print(f"Longest Roman numeral under 4000: {max(lengths.values())} chars")
longest_n = max(lengths, key=lengths.get)
print(f"  Number: {longest_n} = {int_to_roman(longest_n)}")
