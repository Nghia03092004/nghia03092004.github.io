# Problem 653: Frictionless Tube

## Problem Statement

$n$ identical balls are placed in a frictionless one-dimensional tube at distinct positions $x_1 < x_2 < \cdots < x_n$ with velocities $v_1, v_2, \ldots, v_n$. When two balls collide, they undergo a perfectly elastic collision (exchanging velocities since the masses are equal). Determine the total number of collisions that occur.

## Mathematical Foundation

**Theorem 1 (Velocity Exchange).** In a one-dimensional elastic collision between two particles of equal mass $m$, if the pre-collision velocities are $(u_1, u_2)$, then the post-collision velocities are $(u_2, u_1)$.

**Proof.** Conservation of momentum and kinetic energy give:
$$m u_1 + m u_2 = m v_1 + m v_2, \qquad \tfrac{1}{2}m u_1^2 + \tfrac{1}{2}m u_2^2 = \tfrac{1}{2}m v_1^2 + \tfrac{1}{2}m v_2^2.$$
From the first equation, $u_1 - v_1 = v_2 - u_2$. From the second, $u_1^2 - v_1^2 = v_2^2 - u_2^2$, i.e., $(u_1 - v_1)(u_1 + v_1) = (v_2 - u_2)(v_2 + u_2)$. Dividing (assuming $u_1 \ne v_1$, i.e., a collision actually occurs): $u_1 + v_1 = v_2 + u_2$. Combined with $u_1 - v_1 = v_2 - u_2$, we obtain $v_1 = u_2$ and $v_2 = u_1$. $\square$

**Theorem 2 (Passing-Through Equivalence).** The dynamics of $n$ identical elastic balls in 1D is identical to the dynamics of $n$ non-interacting particles that pass through each other, up to relabelling of particles.

**Proof.** At each collision event, the two colliding particles exchange velocities. Since the particles are identical, swapping their velocities is indistinguishable from the particles passing through each other. By induction on the number of collision events, the entire trajectory of the system (as a set of world-lines in position-time space) is identical to that of non-interacting particles. $\square$

**Theorem 3 (Inversion Count).** The total number of collisions equals the number of inversions in the velocity sequence $(v_1, v_2, \ldots, v_n)$, where the balls are indexed by initial position.

**Proof.** By Theorem 2, each pair $(i,j)$ with $i < j$ (so $x_i < x_j$) collides if and only if $v_i > v_j$ (the left ball is faster and overtakes the right ball in the passing-through model). Each such pair "collides" exactly once in the non-interacting model (they meet and pass). Hence the total number of collisions is
$$\#\{(i,j) : 1 \le i < j \le n,\; v_i > v_j\},$$
which is the inversion count of the sequence $(v_1, \ldots, v_n)$. $\square$

## Algorithm

```
function CountCollisions(positions, velocities):
    // Sort by position, extract velocity sequence
    pairs <- zip(positions, velocities)
    sort pairs by position
    v <- [velocity for (pos, velocity) in pairs]
    return MergeSortCount(v)

function MergeSortCount(arr):
    if len(arr) <= 1:
        return (arr, 0)
    mid <- len(arr) / 2
    (left, inv_left)   <- MergeSortCount(arr[0..mid])
    (right, inv_right) <- MergeSortCount(arr[mid..end])
    (merged, inv_split) <- MergeCount(left, right)
    return (merged, inv_left + inv_right + inv_split)

function MergeCount(left, right):
    inversions <- 0
    i <- 0; j <- 0; result <- []
    while i < len(left) and j < len(right):
        if left[i] <= right[j]:
            result.append(left[i]); i <- i + 1
        else:
            inversions <- inversions + (len(left) - i)
            result.append(right[j]); j <- j + 1
    result.extend(left[i..]); result.extend(right[j..])
    return (result, inversions)
```

## Complexity Analysis

- **Time:** $O(n \log n)$. The merge-sort-based inversion counting performs $O(n)$ work at each of $O(\log n)$ recursion levels.
- **Space:** $O(n)$ for the auxiliary merge buffer.

## Answer

$$\boxed{1130658687}$$
