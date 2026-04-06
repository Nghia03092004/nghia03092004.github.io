def solve():
    N = 10**18

    # Langton's Ant simulation
    # Directions: 0=up, 1=right, 2=down, 3=left
    dx = [0, 1, 0, -1]
    dy = [1, 0, -1, 0]

    black = set()
    x, y, d = 0, 0, 0

    sim_steps = 20000
    black_count = [0] * (sim_steps + 1)

    for step in range(sim_steps):
        pos = (x, y)
        if pos in black:
            # On black: flip to white, turn left, move forward
            black.remove(pos)
            d = (d + 3) % 4
        else:
            # On white: flip to black, turn right, move forward
            black.add(pos)
            d = (d + 1) % 4
        x += dx[d]
        y += dy[d]
        black_count[step + 1] = len(black)

    # Highway: period 104, gain 12 black squares per period
    period = 104
    gain = 12

    L = sim_steps
    remaining = N - L
    full_cycles = remaining // period
    leftover = remaining % period

    answer = black_count[L] + full_cycles * gain

    # For leftover steps, use the periodic pattern
    delta = black_count[L - period + leftover] - black_count[L - period]
    answer += delta

    print(answer)

if __name__ == "__main__":
    solve()
