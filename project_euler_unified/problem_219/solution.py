"""
Problem 219: Skew-cost Coding

Build a prefix-free code for N = 10^9 symbols.
Bit '0' costs 1, bit '1' costs 4.
Minimize total cost of all codewords.

Greedy approach: always split the cheapest codeword.
Splitting a codeword of cost c removes it and creates c+1 and c+4.
Net cost increase per split of cost c: c + 5.

Use histogram (sorted dict) for bulk processing.

Answer: 64564225042
"""

def solve_simple():
    """Alternative using plain dict with manual min tracking."""
    N = 10**9

    freq = {0: 1}
    total_cost = 0
    num_codes = 1
    min_cost = 0

    while num_codes < N:
        # Find minimum cost
        while min_cost not in freq or freq[min_cost] == 0:
            if min_cost in freq and freq[min_cost] == 0:
                del freq[min_cost]
            min_cost += 1

        cost = min_cost
        count = freq[cost]
        del freq[cost]

        need = N - num_codes
        splits = min(count, need)

        total_cost += splits * (cost + 5)
        num_codes += splits

        freq[cost + 1] = freq.get(cost + 1, 0) + splits
        freq[cost + 4] = freq.get(cost + 4, 0) + splits

        if splits < count:
            freq[cost] = freq.get(cost, 0) + (count - splits)

    print(total_cost)

if __name__ == "__main__":
    solve_simple()
