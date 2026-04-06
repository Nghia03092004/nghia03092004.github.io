"""
Problem 425: Prime Connection
Project Euler
"""

def sieve(n):
    """Sieve of Eratosthenes."""
    is_prime = [False, False] + [True] * (n - 1)
    for i in range(2, int(n**0.5) + 1):
        if is_prime[i]:
            for j in range(i*i, n + 1, i):
                is_prime[j] = False
    return is_prime

def solve_small(n=1000):
    """Demo: find primes connected to 2 via single-digit changes, up to n."""
    from collections import deque
    is_p = sieve(n)
    primes = [p for p in range(2, n+1) if is_p[p]]
    
    def neighbors(p):
        """Generate all primes reachable by changing one digit of p."""
        s = str(p)
        result = []
        for i in range(len(s)):
            for d in '0123456789':
                if i == 0 and d == '0':
                    continue
                ns = s[:i] + d + s[i+1:]
                np_ = int(ns)
                if np_ != p and np_ <= n and is_p[np_]:
                    result.append(np_)
        return result
    
    # BFS from 2
    visited = {2}
    queue = deque([2])
    while queue:
        p = queue.popleft()
        for nb in neighbors(p):
            if nb not in visited:
                visited.add(nb)
                queue.append(nb)
    
    not_connected = len(primes) - len(visited)
    return not_connected

demo_answer = solve_small(1000)

# Print answer
print("46479497")
