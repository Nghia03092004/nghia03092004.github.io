"""
Problem 736: Paths to Equality

r(x,y)=(x+1,2y), s(x,y)=(2x,y+1). Find shortest odd-length path from (45,90) to x=y.
"""

def find_shortest_odd_path(a, b, max_len=25):
    """BFS for shortest odd-length path from (a,b) to x=y."""
    from collections import deque

    # State: (x, y, length, path)
    # Use BFS level by level
    for target_len in range(1, max_len + 1, 2):  # odd lengths only
        # DFS/enumerate all 2^target_len paths
        stack = [(a, b, 0, [])]
        results = []
        queue = [(a, b, "")]

        # BFS approach: expand level by level
        current = [(a, b, "")]
        for step in range(target_len):
            nxt = []
            for x, y, path in current:
                # Apply r
                nx, ny = x + 1, 2 * y
                if step < target_len - 1:
                    if nx != ny:
                        nxt.append((nx, ny, path + "r"))
                else:
                    if nx == ny:
                        results.append((nx, path + "r"))
                    # Also check even if not equal (just don't add to results)
                    nxt.append((nx, ny, path + "r"))

                # Apply s
                nx, ny = 2 * x, y + 1
                if step < target_len - 1:
                    if nx != ny:
                        nxt.append((nx, ny, path + "s"))
                else:
                    if nx == ny:
                        results.append((nx, path + "s"))
                    nxt.append((nx, ny, path + "s"))

            if step < target_len - 1:
                current = nxt
            else:
                # Check results from this level
                pass

        if results:
            print(f"  Found {len(results)} path(s) of odd length {target_len}")
            for val, path in results[:5]:
                print(f"    Final value: {val}, path: {path}")
            return results[0][0], target_len

    return None, None

# Search (this may take time for large lengths)
# For demonstration, try small lengths first
val, length = find_shortest_odd_path(45, 90, max_len=15)
if val:
    print(f"Answer: final value = {val}, path length = {length}")
else:
    print("No odd-length path found within search limit")
