import heapq
import math

def solve():
    MOD = 500500507
    TARGET = 500500

    # Sieve primes
    LIMIT = 7800000
    is_prime = bytearray(b'\x01') * (LIMIT + 1)
    is_prime[0] = is_prime[1] = 0
    for i in range(2, int(LIMIT**0.5) + 1):
        if is_prime[i]:
            is_prime[i*i::i] = bytearray(len(is_prime[i*i::i]))

    primes = [i for i in range(2, LIMIT + 1) if is_prime[i]]

    # Min-heap: (log_value, prime_index, level)
    # multiplier = prime^(2^level), log_value = 2^level * log(prime)
    heap = []
    for i in range(min(len(primes), TARGET + 100)):
        heapq.heappush(heap, (math.log(primes[i]), i, 0))

    answer = 1
    for _ in range(TARGET):
        log_val, idx, level = heapq.heappop(heap)

        # Multiply by primes[idx]^(2^level) mod MOD
        mult = pow(primes[idx], 1 << level, MOD)
        answer = answer * mult % MOD

        # Push next level
        heapq.heappush(heap, (log_val * 2, idx, level + 1))

    print(answer)

solve()
