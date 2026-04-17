import urllib.request
import os

def roman_to_int(s):
    """Parse a Roman numeral string to integer."""
    val = {'I': 1, 'V': 5, 'X': 10, 'L': 50, 'C': 100, 'D': 500, 'M': 1000}
    result = 0
    for i in range(len(s)):
        if i + 1 < len(s) and val[s[i]] < val[s[i + 1]]:
            result -= val[s[i]]
        else:
            result += val[s[i]]
    return result

def int_to_roman(n):
    """Convert integer to minimal Roman numeral form."""
    values =  [1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1]
    symbols = ['M', 'CM', 'D', 'CD', 'C', 'XC', 'L', 'XL', 'X', 'IX', 'V', 'IV', 'I']
    result = []
    for v, sym in zip(values, symbols):
        while n >= v:
            result.append(sym)
            n -= v
    return ''.join(result)

def solve():
    """
    Read Roman numerals from Project Euler data file.
    For each, convert to integer and back to minimal form.
    Count total characters saved.
    """
    # Try to read from local file first
    script_dir = os.path.dirname(os.path.abspath(__file__))
    local_file = os.path.join(script_dir, "p089_roman.txt")

    if os.path.exists(local_file):
        with open(local_file) as f:
            lines = f.read().strip().split('\n')
    else:
        # Download from Project Euler
        url = "https://projecteuler.net/resources/documents/0089_roman.txt"
        try:
            response = urllib.request.urlopen(url)
            data = response.read().decode('utf-8')
            lines = data.strip().split('\n')
            # Save locally for future use
            with open(local_file, 'w') as f:
                f.write(data)
        except Exception:
            print("Error: Could not read roman numerals file.")
            print("Please download p089_roman.txt from Project Euler and place it in this directory.")
            return

    total_saved = 0
    for line in lines:
        line = line.strip()
        if not line:
            continue
        val = roman_to_int(line)
        minimal = int_to_roman(val)
        total_saved += len(line) - len(minimal)

    print(total_saved)

if __name__ == "__main__":
    solve()
