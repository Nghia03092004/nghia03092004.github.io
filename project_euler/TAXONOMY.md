# Project Euler Taxonomy

This archive uses a closed, controlled taxonomy for Project Euler tags.

The machine-readable source of truth lives in `project_euler/taxonomy.json`.

## Rules

- Tags must come only from the controlled list below.
- Tags must use `snake_case`.
- Do not invent ad hoc tags.
- Do not duplicate tags on a single problem.
- Prefer `1` to `3` tags per problem.
- Never exceed `5` tags for one problem.

## Allowed tags

- `algebra` — polynomial structure, equations, symbolic manipulation.
- `analytic_math` — asymptotics, series, generating functions, numerical analysis.
- `arithmetic` — elementary integer manipulation, sums, fractions, direct numeric structure.
- `brute_force` — exhaustive search, broad enumeration, straightforward trial computation.
- `combinatorics` — counting arguments, permutations, combinations, subsets.
- `constructive` — explicit construction or arrangement of objects.
- `digit_dp` — digit-state dynamic programming or digit-constrained counting.
- `dynamic_programming` — memoization, recurrence optimization, state transitions.
- `game_theory` — winning states, impartial games, strategy analysis.
- `geometry` — planar or spatial geometry, distance, area, angle, polygon arguments.
- `graph` — vertices, edges, paths, trees, graph traversal or graph structure.
- `greedy` — locally optimal selection with a global correctness argument.
- `linear_algebra` — matrices, determinants, vectors, eigenstructure.
- `modular_arithmetic` — congruences, residues, modular computation.
- `number_theory` — primes, divisors, totients, Diophantine structure.
- `optimization` — minimizing, maximizing, or selecting an extremal quantity.
- `probability` — random processes, expected value, stochastic arguments.
- `recursion` — recursive decomposition or recursive implementation structure.
- `search` — DFS, BFS, binary search, backtracking, or guided state exploration.
- `sequence` — Fibonacci-type behavior, recurrences, integer sequences.
- `simulation` — direct process simulation or system evolution over steps.

## Metadata usage

- Per-problem tags live in `project_euler/metadata.json`.
- The current metadata file is generated heuristically and restricted to this taxonomy only.
- Manual refinement is allowed later, but it must stay inside the controlled vocabulary.
