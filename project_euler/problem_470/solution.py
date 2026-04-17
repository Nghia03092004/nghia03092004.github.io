"""
Project Euler Problem 470: Super Ramvok

A die-rolling game with optimal strategy. Compute F(20) = sum over d=4..20,
c=0..20 of S(d, c), rounded to nearest integer.

R(d,c) = expected profit of optimally-played Ramvok with d-sided die, cost c per turn.
S(d,c) = expected total profit of Super Ramvok.
"""

import numpy as np
from itertools import combinations
from functools import lru_cache

def compute_R(visible_values, c):
    """
    Compute R(die, c) = max_t (E_t - c*t) for a die with given visible values.

    visible_values: list of face values that are not blank
    c: cost per turn

    Returns optimal expected profit.
    """
    if not visible_values:
        return 0.0  # no visible faces, can't play

    vals = sorted(visible_values)
    k = len(vals)
    mu = sum(vals) / k

    # Compute E_t for increasing t until E_t - c*t starts decreasing
    E_prev = mu  # E_1
    best_profit = E_prev - c * 1

    for t in range(2, 1000):  # upper bound on turns
        # E_t = (1/k) * (sum of max(v, E_{t-1}) for v in vals)
        E_t = 0.0
        for v in vals:
            E_t += max(v, E_prev)
        E_t /= k

        profit = E_t - c * t
        if profit > best_profit:
            best_profit = profit
        elif E_t - E_prev < c:
            # Marginal gain < cost, won't improve further
            break
        E_prev = E_t

    # Also consider t=0 (not playing): profit = -0 = 0
    # Actually, player MUST choose t >= 1? The problem says "t is chosen by the player"
    # If the player can choose not to play (t=0, cost=0, no prize), profit=0
    # But the problem says "before the game begins, t is chosen"
    # In Super Ramvok, a game of Ramvok CAN begin -- the player chooses t optimally
    # If all profits are negative, player should choose t=0 (not play)?
    # Or must play at least once?
    # The problem says "another game of Ramvok can begin" -- suggesting it's optional.
    # For safety, allow t=0:
    best_profit = max(best_profit, 0.0)

    return best_profit

def solve_super_ramvok(d, c):
    """
    Solve Super Ramvok for a d-sided die with cost c.

    State: bitmask of visible faces (face i has value i+1).
    Transitions: toggle a random face (each with prob 1/d).
    Game ends when all faces blank (state 0).

    V(S) = R(S, c) + (1/d) * sum_{i=0}^{d-1} V(S XOR (1<<i))
    V(0) = 0

    Returns S(d, c) = V(full_mask).
    """
    full = (1 << d) - 1
    n_states = 1 << d

    # Precompute R(S, c) for all non-empty S
    R = np.zeros(n_states)
    for mask in range(1, n_states):
        visible = [i + 1 for i in range(d) if mask & (1 << i)]
        R[mask] = compute_R(visible, c)

    # Solve the linear system using iterative method (Gauss-Seidel)
    # V(S) = R(S) + (1/d) * sum_i V(S ^ (1<<i))
    # Rearranging: V(S) - (1/d) * sum_i V(S^(1<<i)) = R(S)
    #
    # The diagonal coefficient of V(S) in the equation for V(S) is 1.
    # But V(S^(1<<i)) might include V(S) if toggling i and toggling i again
    # gives back S -- no, S^(1<<i) != S for any i.
    # So V(S) = R(S) + (1/d) * sum_i V(S^(1<<i))
    #
    # This is V = R + (1/d) * A * V where A is the adjacency of the hypercube.
    # (I - (1/d)*A) * V = R
    #
    # The eigenvalues of (1/d)*A on the hypercube are (d-2k)/d for k=0..d,
    # so eigenvalues of I - (1/d)*A are 2k/d for k=0..d.
    # The smallest eigenvalue is 0 (for k=0, the all-ones vector).
    # But we have V(0) = 0 boundary condition, which breaks that eigenvector.
    # So the system should be solvable.

    # Use iterative relaxation
    V = np.zeros(n_states)
    V[0] = 0.0

    for iteration in range(10000):
        max_change = 0.0
        for S in range(1, n_states):
            neighbor_sum = 0.0
            for i in range(d):
                neighbor_sum += V[S ^ (1 << i)]
            new_V = R[S] + neighbor_sum / d
            change = abs(new_V - V[S])
            if change > max_change:
                max_change = change
            V[S] = new_V

        if max_change < 1e-12:
            break

    # The answer is V[full]
    # But wait -- this iteration might diverge!
    # The spectral radius of (1/d)*A restricted to non-zero states needs to be < 1.
    # On the d-dimensional hypercube, eigenvalues of A are d-2k for k=0,...,d.
    # So (1/d)*A has eigenvalues (d-2k)/d.
    # For k=0: eigenvalue = 1. This means spectral radius = 1 and simple iteration diverges.
    #
    # We need a different approach. The system (I - (1/d)*A)V = R has a unique solution
    # if we remove the V(0)=0 constraint properly.
    #
    # Actually, since V(0) = 0 is fixed, we solve for the (2^d - 1) unknowns V(S) for S != 0.
    # The matrix is (I - (1/d)*A') where A' is the adjacency restricted to non-zero states
    # (transitions to state 0 contribute 0 to the neighbor sum).
    #
    # The eigenvalue 1 of (1/d)*A corresponds to the all-ones vector. With V(0)=0 boundary,
    # this eigenvector is broken. So (I - (1/d)*A') should be invertible.
    #
    # But the iteration V <- R + (1/d)*A'*V still has spectral radius close to 1.
    # Use a direct solver instead for small d.

    if d <= 15:
        # Direct solve using numpy for small d
        # Build system: for each S in 1..2^d-1:
        # V(S) - (1/d) * sum_i V(S^(1<<i)) = R(S)
        # where V(0) = 0 (so if S^(1<<i) = 0, that term is 0)
        n = n_states - 1  # number of unknowns (excluding state 0)
        # Map: state S -> index (S-1) for S >= 1
        A_mat = np.eye(n)
        b_vec = np.zeros(n)

        for S in range(1, n_states):
            idx = S - 1
            b_vec[idx] = R[S]
            for i in range(d):
                neighbor = S ^ (1 << i)
                if neighbor == 0:
                    continue  # V(0) = 0
                n_idx = neighbor - 1
                A_mat[idx][n_idx] -= 1.0 / d

        V_sol = np.linalg.solve(A_mat, b_vec)
        return V_sol[full - 1]  # V[full] = V_sol[full-1]
    else:
        # For larger d, use sparse iterative solver
        from scipy.sparse import lil_matrix
        from scipy.sparse.linalg import spsolve

        n = n_states - 1
        A_mat = lil_matrix((n, n))
        b_vec = np.zeros(n)

        for S in range(1, n_states):
            idx = S - 1
            A_mat[idx, idx] = 1.0
            b_vec[idx] = R[S]
            for i in range(d):
                neighbor = S ^ (1 << i)
                if neighbor == 0:
                    continue
                n_idx = neighbor - 1
                A_mat[idx, n_idx] -= 1.0 / d

        A_csr = A_mat.tocsr()
        V_sol = spsolve(A_csr, b_vec)
        return V_sol[full - 1]

def compute_F(n_max):
    """Compute F(n) = sum_{d=4}^{n} sum_{c=0}^{n} S(d, c)."""
    total = 0.0
    results = {}

    for d in range(4, n_max + 1):
        for c in range(0, n_max + 1):
            s = solve_super_ramvok(d, c)
            results[(d, c)] = s
            total += s
            print(f"  S({d}, {c}) = {s:.4f}")
        print(f"  Subtotal after d={d}: {total:.4f}")

    return total, results

def verify_examples():
    """Verify the given examples."""
    print("=== Verification ===")

    # R(4, 0.2) = 2.65
    r4 = compute_R([1, 2, 3, 4], 0.2)
    print(f"R(4, 0.2) = {r4:.4f} (expected 2.65)")

    # S(6, 1) = 208.3
    print("Computing S(6, 1)...")
    s6 = solve_super_ramvok(6, 1)
    print(f"S(6, 1) = {s6:.1f} (expected 208.3)")

    return r4, s6

def create_visualization(results=None):
    """Create visualization."""
