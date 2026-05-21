# Problem 227: The Chase

## Problem Statement

One hundred players sit around a circular table. Two opposite players start with one die each. On every turn, the two players who currently hold dice roll them:

- on a $1$, the die is passed to the neighbour on the left;
- on a $6$, the die is passed to the neighbour on the right;
- otherwise the die stays where it is.

The game ends when, after the roll-and-pass step, one player holds both dice. Find the expected number of turns, rounded to $10$ significant digits.

## Mathematical Development

The rotational symmetry means that the full state is determined by the shorter circular distance $d$ between the two dice:

$$
d \in \{0,1,\dots,50\}.
$$

State $d=0$ is absorbing.

For one die, the move distribution is

$$
-1 \text{ with probability } \frac16,
\qquad
0 \text{ with probability } \frac46,
\qquad
1 \text{ with probability } \frac16.
$$

Therefore the distance change for the pair is

$$
\Delta \in \{-2,-1,0,1,2\}
$$

with probabilities

$$
\frac1{36},\ \frac8{36},\ \frac{18}{36},\ \frac8{36},\ \frac1{36}.
$$

If the current state is $d$, let

$$
r=(d+\Delta)\bmod 100.
$$

Then the actual next state is the shorter of the two arcs:

$$
d'=\min(r,100-r).
$$

Let $E(d)$ be the expected remaining number of turns from state $d$. Then

$$
E(0)=0
$$

and for $1 \le d \le 50$,

$$
E(d)=1+\sum_{d'=1}^{50} T(d,d')\,E(d'),
$$

where $T(d,d')$ is the transition probability. This is a $50\times 50$ linear system:

$$
(I-T)\,E=\mathbf 1.
$$

## Editorial

Once the distance between the dice is chosen as the state, nothing else matters. The labels of the players disappear completely, and the stochastic process becomes a small absorbing Markov chain.

The transition law is also tiny. Each die only has three possible effects on the gap, so the combined move has five possibilities, from $-2$ to $+2$. That means the whole problem is just linear algebra: build the $50\times 50$ transition matrix on the transient states, write down the first-step equations

$$
E(d)=1+\sum T(d,d')E(d'),
$$

and solve them with Gaussian elimination. The wanted value is the expectation from the initial opposite state $d=50$.

## Pseudocode

```text
Build the five probabilities for the net distance change:
    -2, -1, 0, 1, 2.

For each transient state d from 1 to 50:
    For each possible net change:
        update the probability of the resulting shorter-arc distance d'

Form the linear system
    (I - T) * E = 1
on the 50 transient states.

Solve the system by Gaussian elimination.
Output E(50) with 6 digits after the decimal point.
```

## Complexity Analysis

- **Time:** $O(50^3)$ for Gaussian elimination.
- **Space:** $O(50^2)$ for the matrix.

## Answer

$$\boxed{3780.618622}$$
