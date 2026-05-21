# Problem 225: Tribonacci Non-divisors

## Problem Statement

The Tribonacci sequence is defined by

$$
T_1=T_2=T_3=1,
\qquad
T_n=T_{n-1}+T_{n-2}+T_{n-3}\quad(n>3).
$$

Find the $124$th odd number that does **not** divide any Tribonacci number.

## Mathematical Development

Modulo any positive integer $m$, the sequence is controlled by triples

$$
(T_n,T_{n+1},T_{n+2}) \pmod m.
$$

There are only $m^3$ such triples, so some triple must repeat. The recurrence is invertible:

$$
T_{n-1}=T_{n+2}-T_{n+1}-T_n.
$$

Therefore once a triple repeats, the whole sequence before and after that point repeats as well. In particular, the Tribonacci sequence modulo $m$ is **purely periodic**.

So to decide whether $m$ divides any Tribonacci number, it is enough to start from

$$
(1,1,1)
$$

and follow the sequence modulo $m$ until one of two things happens:

- a term becomes $0$, so $m$ is a divisor of some Tribonacci number;
- the triple returns to $(1,1,1)$, so one full period has been traversed without seeing $0$.

Because every Tribonacci number is odd, only odd candidates can possibly fail to divide the sequence.

## Editorial

The key observation is that this is not a growth problem at all; it is a finite-state problem. Modulo $m$, the sequence never produces anything except triples of residues, and there are only $m^3$ of those. Once the initial triple comes back, the cycle is closed and nothing new can happen.

That means every candidate odd number can be tested independently with constant memory. The program walks the Tribonacci recurrence modulo $m$, stops immediately if it ever hits $0$, and otherwise declares success when the state returns to $(1,1,1)$. Counting such odd moduli in increasing order gives the required $124$th example.

## Pseudocode

```text
Set the target count to 124.
Initialize the current odd candidate m = 1.

Repeat forever:
    Skip m = 1 because it divides every integer.

    Start the Tribonacci state modulo m at (1, 1, 1).

    Advance the recurrence:
        next = (a + b + c) mod m
        shift (a, b, c) to (b, c, next)

        If c becomes 0:
            m divides some Tribonacci number
            stop testing this m

        If the state returns to (1, 1, 1):
            m is a Tribonacci non-divisor
            increase the running count
            if this is the 124th one, print m and stop

    Move on to the next odd number.
```

## Complexity Analysis

- **Time:** For a fixed modulus $m$, at most one full period is explored. The crude bound is $O(m^3)$, though the actual periods are much shorter.
- **Space:** $O(1)$.

## Answer

$$\boxed{2009}$$
