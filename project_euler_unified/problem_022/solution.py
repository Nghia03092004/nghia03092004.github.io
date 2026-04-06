import os

def solve():
    script_dir = os.path.dirname(os.path.abspath(__file__))
    filepath = os.path.join(script_dir, 'names.txt')

    if not os.path.exists(filepath):
        filepath = 'names.txt'

    if not os.path.exists(filepath):
        try:
            import urllib.request
            url = "https://projecteuler.net/resources/documents/0022_names.txt"
            urllib.request.urlretrieve(url, filepath)
        except Exception:
            print("Error: names.txt not found.")
            return

    with open(filepath, 'r') as f:
        content = f.read()

    names = sorted(name.strip('"') for name in content.strip().split(','))
    total = sum((i + 1) * sum(ord(c) - 64 for c in name) for i, name in enumerate(names))
    print(total)
