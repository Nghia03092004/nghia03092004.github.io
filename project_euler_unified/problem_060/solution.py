"""
Project Euler Problem 60: Prime Pair Sets

Find the lowest sum for a set of five primes for which any two
primes concatenate (in either order) to produce another prime.

Approach: build the prime-pair graph and search for minimum-weight
5-cliques via depth-first enumeration with adjacency-intersection
pruning and branch-and-bound.

Answer: 26033
"""
from sympy import isprime, primerange

def solve():
    LIMIT = 10000
    primes = list(primerange(2, LIMIT))

    pair_cache = {}

    def is_pair(a, b):
        key = (a, b)
        if key not in pair_cache:
            pair_cache[key] = (
                isprime(int(str(a) + str(b))) and
                isprime(int(str(b) + str(a)))
            )
        return pair_cache[key]

    # Build adjacency lists
    adj = {p: [] for p in primes}
    for i in range(len(primes)):
        for j in range(i + 1, len(primes)):
            if is_pair(primes[i], primes[j]):
                adj[primes[i]].append(primes[j])

    best = float('inf')

    # Incremental 5-clique search
    for p1 in primes:
        if p1 * 5 >= best:
            break
        for p2 in adj[p1]:
            if p1 + p2 * 4 >= best:
                break
            friends2 = [p for p in adj[p1] if p > p2 and is_pair(p2, p)]
            for p3 in friends2:
                if p1 + p2 + p3 * 3 >= best:
                    break
                friends3 = [p for p in friends2 if p > p3 and is_pair(p3, p)]
                for p4 in friends3:
                    if p1 + p2 + p3 + p4 * 2 >= best:
                        break
                    friends4 = [p for p in friends3 if p > p4 and is_pair(p4, p)]
                    for p5 in friends4:
                        s = p1 + p2 + p3 + p4 + p5
                        if s < best:
                            best = s

    return best

if __name__ == "__main__":
    answer = solve()
assert answer == 26033
print(answer)
