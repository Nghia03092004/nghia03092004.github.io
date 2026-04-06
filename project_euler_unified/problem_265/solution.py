"""
Problem 265: Binary Circles

Place 2^N binary digits in a circle so all N-bit clockwise subsequences are distinct.
Encode each arrangement as a number: start from the all-zeros subsequence, read
clockwise, concatenate bits. S(N) = sum of all such encodings.

For N=5: 32 bits in a circle, all 32 possible 5-bit strings appear exactly once.
Start reading from 00000. The 32-bit binary number is the encoding.

Find S(5).

Answer: 209110240
"""

def solve(N=5):
    L = 1 << N  # 2^N = 32
    MASK = (1 << N) - 1  # 0b11111

    total_sum = 0

    # Backtracking: build the sequence bit by bit.
    # bits[0..N-1] = 0 (start with all zeros).
    # At each step, choose bit[i] for i = N, N+1, ..., L-1.
    # Track which N-bit windows have been used (bitmask of 2^N bits = 32 bits => int).
    # Window at position j = bits[j..j+N-1] (circular).
    # After placing all L bits, check the wrap-around windows.

    # State: sequence so far (as integer), position, used windows bitmask, current N-bit window.

    def dfs(seq, pos, used, window):
        nonlocal total_sum

        if pos == L:
            # Check wrap-around windows: positions L-N+1 through L-1
            # These use bits from end and beginning.
            w = window
            ok = True
            for i in range(N - 1):
                # Next window: shift left, add bit from start
                bit = (seq >> (L - 1 - i)) & 1
                w = ((w << 1) | bit) & MASK
                if used & (1 << w):
                    ok = False
                    break
                used |= (1 << w)
            if ok:
                total_sum += seq
            return

        for bit in (0, 1):
            new_window = ((window << 1) | bit) & MASK
            if not (used & (1 << new_window)):
                new_seq = (seq << 1) | bit
                dfs(new_seq, pos + 1, used | (1 << new_window), new_window)

    # Start: first N bits are 0. Window = 0. used = {0}.
    initial_seq = 0  # N bits of 0
    initial_window = 0
    initial_used = 1  # bit 0 set (window 00000 used)
    dfs(initial_seq, N, initial_used, initial_window)

    print(total_sum)

if __name__ == "__main__":
    solve()
