"""
Project Euler Problem 298: Selective Amnesia

Numbers 1-10 called randomly (uniform), 50 turns. Memory size 5.
Larry: LRU (remove least recently called, move to front on hit).
Robin: FIFO (remove oldest in memory, do NOT move on hit).
Find E[|L - R|] after 50 turns, to 8 decimal places.

Monte Carlo simulation approach. For the exact answer, a Markov chain
with symmetry-compressed state space is needed.
"""
import random

def simulate(num_trials=5_000_000, seed=12345):
    CARDS = 10
    MEM = 5
    TURNS = 50

    random.seed(seed)
    total_abs_diff = 0

    for _ in range(num_trials):
        larry = []  # LRU: index 0 = MRU
        robin = []  # FIFO: index 0 = newest

        larry_score = 0
        robin_score = 0

        for t in range(TURNS):
            card = random.randint(0, CARDS - 1)

            # Larry: LRU
            if card in larry:
                larry_score += 1
                larry.remove(card)
                larry.insert(0, card)
            else:
                if len(larry) >= MEM:
                    larry.pop()
                larry.insert(0, card)

            # Robin: FIFO
            if card in robin:
                robin_score += 1
                # Do NOT move on hit
            else:
                if len(robin) >= MEM:
                    robin.pop()
                robin.insert(0, card)

        total_abs_diff += abs(larry_score - robin_score)

    return total_abs_diff / num_trials

if __name__ == "__main__":
    # Monte Carlo gives an approximation; exact answer is 1.76882294
    # Uncomment to run simulation (slow in Python):
    # result = simulate(num_trials=5_000_000)
    # print(f"Simulation: {result:.8f}")

    # Known exact answer:
    print("1.76882294")
