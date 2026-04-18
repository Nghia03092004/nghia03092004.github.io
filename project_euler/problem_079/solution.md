# Problem 79: Passcode Derivation

## Problem Statement

A common security method for online banking is to ask the user for three random characters from a passcode. Given 50 successful login attempts where each attempt reveals a 3-character subsequence of the unknown passcode, determine the shortest possible secret passcode.

### Login Attempts

```
319 680 180 690 129 620 762 689 762 318
368 710 720 710 629 168 160 689 716 731
736 729 316 729 729 710 769 290 719 680
318 389 162 289 162 718 729 319 790 680
890 362 319 760 316 729 380 319 728 716
```

## Mathematical Foundation

**Definition 1 (Subsequence Constraint).** A login attempt $d_1 d_2 d_3$ imposes the constraint that characters $d_1$, $d_2$, $d_3$ appear in this relative order (not necessarily consecutively) within the passcode.

**Definition 2 (Precedence Graph).** The *precedence graph* is a directed graph $G = (V, E)$ where $V$ is the set of distinct digits appearing in the login data and $(u, v) \in E$ if and only if some login attempt contains $u$ before $v$ (i.e., $u = d_i$ and $v = d_j$ with $i < j$ in some attempt $d_1 d_2 d_3$).

**Assumption (No Repeated Digits).** We assume the passcode contains each digit at most once. This is justified a posteriori: if the resulting DAG admits a valid topological ordering of length $|V|$ that satisfies all constraints, then no repetition is needed.

**Theorem 1 (Acyclicity).** *Under the no-repetition assumption, $G$ is a directed acyclic graph (DAG).*

*Proof.* Suppose $G$ contains a directed cycle $v_1 \to v_2 \to \cdots \to v_k \to v_1$. Then the passcode requires $v_1$ to appear before $v_2$, $v_2$ before $v_3$, ..., and $v_k$ before $v_1$. By transitivity of the "precedes" relation, $v_1$ must appear strictly before itself. Since each digit appears exactly once, this is impossible. Hence $G$ is acyclic. $\blacksquare$

**Theorem 2 (Minimality of Topological Ordering).** *If $G$ is a DAG on $|V|$ vertices, then any topological ordering of $G$ yields a string of length $|V|$ respecting all precedence constraints, and no shorter string containing all digits of $V$ exists.*

*Proof.* A topological ordering $v_{\sigma(1)}, v_{\sigma(2)}, \ldots, v_{\sigma(|V|)}$ has the property that $(v_{\sigma(i)}, v_{\sigma(j)}) \in E$ implies $i < j$. Thus every precedence constraint is satisfied. The string has length $|V|$, and any string containing all $|V|$ distinct digits must have length $\geq |V|$. $\blacksquare$

**Theorem 3 (Uniqueness via Hamiltonian Path).** *A DAG has a unique topological ordering if and only if it contains a Hamiltonian path. Equivalently, at each step of Kahn's algorithm, there is exactly one vertex with in-degree $0$.*

*Proof.* ($\Rightarrow$) Let $v_1, v_2, \ldots, v_n$ be the unique topological order. Suppose edge $(v_i, v_{i+1}) \notin E$ for some $i$. Then $v_i$ and $v_{i+1}$ have no direct precedence constraint between them. Moreover, swapping them yields a sequence $v_1, \ldots, v_{i-1}, v_{i+1}, v_i, v_{i+2}, \ldots, v_n$. We claim this is also a valid topological ordering: the only pair whose relative order changed is $(v_i, v_{i+1})$, and since $(v_i, v_{i+1}) \notin E$, no constraint is violated. (We also need $(v_{i+1}, v_i) \notin E$, which holds since $v_i$ precedes $v_{i+1}$ in the original valid ordering.) This contradicts uniqueness. Hence $(v_i, v_{i+1}) \in E$ for all $i$, forming a Hamiltonian path.

($\Leftarrow$) Let $v_1 \to v_2 \to \cdots \to v_n$ be a Hamiltonian path. Then $v_1$ is the unique vertex with in-degree $0$ in $G$ (any other vertex $v_j$ with $j > 1$ has the incoming edge from $v_{j-1}$). Removing $v_1$ and its outgoing edges, $v_2$ becomes the unique vertex with in-degree $0$ in the residual graph. By induction, each step of Kahn's algorithm has a unique choice, producing a unique topological ordering. $\blacksquare$

**Theorem 4 (Correctness of Kahn's Algorithm).** *Given a DAG $G = (V, E)$, Kahn's algorithm outputs a valid topological ordering.*

*Proof.* The algorithm maintains a queue $Q$ of vertices with in-degree $0$ and repeatedly: (i) dequeues a vertex $v$, (ii) appends $v$ to the output sequence, and (iii) for each outgoing edge $(v, w)$, decrements the in-degree of $w$ and enqueues $w$ if its in-degree reaches $0$.

We prove the output $v_{\sigma(1)}, \ldots, v_{\sigma(|V|)}$ is a valid topological ordering by contradiction. Suppose $(u, w) \in E$ but $w$ appears before $u$ in the output. Then $w$ was dequeued before $u$. At the time $w$ was dequeued, its in-degree was $0$, meaning every predecessor of $w$ (including $u$, since $(u, w) \in E$) had already been removed from the graph. But $u$ has not yet been dequeued, so it has not been removed--a contradiction. $\blacksquare$

**Lemma 1 (Extracted Graph for the Given Data).** *From the 50 login attempts, the distinct digits are $V = \{0, 1, 2, 3, 6, 7, 8, 9\}$ (digits $4$ and $5$ never appear, $|V| = 8$). The extracted precedence edges form a DAG with a unique topological ordering: $7, 3, 1, 6, 2, 8, 9, 0$.*

*Proof.* Extracting all ordered pairs from each 3-digit attempt (3 pairs per attempt) and computing in-degrees in the resulting DAG, Kahn's algorithm proceeds:
1. In-degree $0$: only vertex $7$.
2. Remove $7$: only vertex $3$ has in-degree $0$.
3. Remove $3$: only vertex $1$ has in-degree $0$.
4. Continue: $6$, then $2$, then $8$, then $9$, then $0$.

At each step, exactly one vertex has in-degree $0$, confirming uniqueness by Theorem 3. $\blacksquare$

## Editorial
Given 50 successful login attempts (3-digit subsequences), determine the shortest possible secret passcode via topological sort of the precedence DAG. We iterate over each attempt d1 d2 d3 in attempts. Finally, kahn's algorithm.

## Pseudocode

```text
for each attempt d1 d2 d3 in attempts
Kahn's algorithm
while queue is not empty
```

## Complexity Analysis

**Time:** Graph construction scans 50 attempts, extracting $\binom{3}{2} = 3$ ordered pairs each, with $O(1)$-amortized hash set membership checks. Total: $O(150)$. Kahn's algorithm runs in $O(|V| + |E|)$. With $|V| = 8$ and $|E| \leq 56$ (at most $\binom{8}{2}$ directed edges): $O(1)$ for this fixed input.

**Space:** $O(|V| + |E|)$ for the adjacency representation and in-degree array.

## Answer

$$\boxed{73162890}$$
