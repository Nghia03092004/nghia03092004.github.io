def sieve_sum(limit):
    is_prime = bytearray(b'\x01') * limit
    is_prime[0] = is_prime[1] = 0
    for i in range(2, int(limit**0.5) + 1):
        if is_prime[i]:
            is_prime[i*i::i] = bytearray(len(is_prime[i*i::i]))
    return sum(i for i in range(2, limit) if is_prime[i])

print(sieve_sum(2_000_000))
