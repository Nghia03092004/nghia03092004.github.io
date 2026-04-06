# Problem 714: Duodigits

## Problem Statement

A positive integer is a *duodigit* if its decimal representation uses at most 2 distinct digits. For example, $12$, $110$, $33333$ are duodigits, but $123$ is not. Define $d(n)$ as the smallest positive duodigit that is a multiple of $n$. Let

$$D(k) = \sum_{n=1}^{k} d(n).$$

Given $D(110) = 11\,047$ and $D(150) = 53\,312$, find $D(50\,000)$ expressed in scientific notation to 13 significant digits.

## Mathematical Foundation

**Definition.** A *duodigit* is a positive integer whose decimal representation contains at most 2 distinct digit values from $\{0, 1, \ldots, 9\}$.

**Lemma 1.** For any $n \geq 1$, the value $d(n)$ exists and satisfies $d(n) \leq 10^n$ (a crude upper bound).

**Proof.** Consider the $n+1$ numbers $\underbrace{1\cdots1}_{1}, \underbrace{1\cdots1}_{2}, \ldots, \underbrace{1\cdots1}_{n+1}$ (repunits of lengths 1 through $n+1$). By the pigeonhole principle, two of these have the same residue modulo $n$, and their difference is a duodigit (consisting of digits 1 and 0). Hence a duodigit multiple of $n$ exists. $\square$

**Theorem 1.** For each $n$, $d(n)$ can be found by BFS on the state space of residues modulo $n$. For each pair of allowed digits $(a, b)$ with $0 \leq a < b \leq 9$ (and also single-digit numbers), we perform BFS building numbers digit-by-digit from the most significant position, tracking the residue modulo $n$. The first number reaching residue $0$ is the smallest duodigit multiple.

**Proof.** BFS explores numbers in order of number of digits, and within the same digit count, in lexicographic (hence numerical) order. Since we track residues modulo $n$, the state space has at most $n$ states per digit pair. BFS finds the shortest (hence smallest) path to residue 0. Taking the minimum over all $\binom{10}{2} + 10 = 55$ digit pairs yields $d(n)$. $\square$

**Lemma 2.** The BFS for a single digit pair $(a, b)$ terminates in at most $n$ steps (since there are only $n$ distinct residues).

**Proof.** Each BFS level adds at most 2 new residues per existing residue. Since there are $n$ residues total and BFS never revisits a residue, it terminates within $n$ levels. $\square$

## Algorithm

```
function d(n):
    best = infinity
    for each digit pair (a, b) with a < b, a in 0..9, b in 0..9:
        // Also handle single-digit (a = b)
        result = bfs_duodigit(n, a, b)
        best = min(best, result)
    // Also check repdigits (single digit type)
    for a = 1 to 9:
        result = bfs_duodigit(n, a, a)
        best = min(best, result)
    return best

function bfs_duodigit(n, a, b):
    // BFS: state = remainder mod n
    // Start with leading digits (nonzero)
    queue = empty
    visited = array of size n, all false
    for digit d in {a, b} where d > 0:
        r = d mod n
        if r == 0: return d
        if not visited[r]:
            visited[r] = true
            queue.push((r, d))   // (remainder, value)
    while queue not empty:
        (r, val) = queue.pop_front()
        for digit d in {a, b}:
            new_val = val * 10 + d
            new_r = (r * 10 + d) mod n
            if new_r == 0: return new_val
            if not visited[new_r]:
                visited[new_r] = true
                queue.push((new_r, new_val))
    return infinity

function D(k):
    total = 0
    for n = 1 to k:
        total += d(n)
    return total
```

## Complexity Analysis

- **Time:** For each $n$, the BFS over all 55 digit pairs runs in $O(55 \cdot n)$ time. Over all $n = 1, \ldots, k$, the total is $O(55 \cdot k^2) = O(k^2)$. For $k = 50000$, this is approximately $1.4 \times 10^{11}$ operations -- tight but feasible with careful implementation and early termination. In practice, BFS terminates much earlier for most $n$.
- **Space:** $O(n)$ per BFS call for the visited array and queue.

## Answer

$$\boxed{2.452767775565e20}$$
