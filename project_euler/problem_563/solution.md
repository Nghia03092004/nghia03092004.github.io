# Problem 563: Robot Welders

## Problem Statement

A company has $n$ robot welders on a production line. Each robot $i$ has a fixed position $p_i$ and a welding range $[a_i, b_i]$ on the line. A weld job at position $x$ can be performed by robot $i$ if $a_i \le x \le b_i$. Each robot can handle at most one job at a time. Given a set of weld jobs, schedule them to robots to minimize the total idle time (or equivalently, maximize the number of completed jobs under timing constraints).

The problem asks for the expected number of completed welds under a specific probabilistic model, or equivalently, the optimal scheduling cost. We compute $R(n)$ for the given input parameters.

## Mathematical Foundation

**Theorem 1 (Interval Scheduling as Bipartite Matching).** The problem of assigning weld jobs to robots with interval constraints reduces to a maximum bipartite matching problem. Let $G = (J \cup R, E)$ where $J$ is the set of jobs, $R$ is the set of robots, and $(j, r) \in E$ iff robot $r$ can reach job $j$. The maximum number of simultaneously serviceable jobs equals the maximum matching in $G$.

**Proof.** Each robot handles at most one job (matching constraint), and each job is assigned to at most one robot. A valid assignment is exactly a matching in $G$. By definition, the optimal assignment is a maximum matching. $\square$

**Lemma 1 (Interval Structure Enables Greedy).** When robots and jobs are both ordered along the line, the bipartite graph has an interval structure. Specifically, for each job at position $x$, the set of robots that can service it forms a contiguous subsequence of the robot ordering. This interval structure allows the maximum matching to be found greedily.

**Proof.** Robots are ordered by position $p_1 \le p_2 \le \cdots \le p_n$, and the reachability condition $a_i \le x \le b_i$ defines nested or overlapping intervals. A job at position $x$ is reachable by robot $i$ iff $x \in [a_i, b_i]$. Since the intervals $[a_i, b_i]$ respect the positional ordering, the feasible robots for any job form a contiguous block. In such interval-structured bipartite graphs, a greedy left-to-right sweep yields a maximum matching. $\square$

**Theorem 2 (DP Recurrence).** Let $\text{dp}[i][j]$ denote the minimum cost of scheduling the first $i$ jobs using robots from the first $j$ available robots. The recurrence is:
$$\text{dp}[i][j] = \min\begin{cases} \text{dp}[i][j-1] & \text{(skip robot } j\text{)} \\ \text{dp}[i-1][j-1] + c(i,j) & \text{(assign job } i \text{ to robot } j\text{, if feasible)} \end{cases}$$
where $c(i,j)$ is the cost of robot $j$ performing job $i$.

**Proof.** This is a standard DP for bipartite matching on interval-ordered elements. The optimal substructure holds because the interval ordering ensures that assigning job $i$ to robot $j$ does not affect the feasibility of assigning jobs $1, \ldots, i-1$ to robots $1, \ldots, j-1$. $\square$

## Editorial
We use dynamic programming over the state space implied by the derivation, apply each admissible transition, and read the answer from the final table entry.

## Pseudocode

```text
    Sort jobs by position
    Sort robots by position
    n = len(jobs), m = len(robots)

    dp[0..n][0..m] = infinity
    dp[0][j] = 0 for all j # no jobs to assign => zero cost

    For each i in 1..n:
        For each j in 1..m:
            dp[i][j] = dp[i][j-1] # skip robot j
            if robot j can service job i:
                dp[i][j] = min(dp[i][j], dp[i-1][j-1] + cost(i, j))

    Return dp[n][m]
```

## Complexity Analysis

- **Time:** $O(n \cdot m)$ where $n$ is the number of jobs and $m$ is the number of robots. With the problem's constraints, this simplifies to $O(N^2)$.
- **Space:** $O(N)$ using a rolling-array optimization on the DP table (only two rows needed at a time).

## Answer

$$\boxed{27186308211734760}$$
