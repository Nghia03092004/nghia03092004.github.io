# Problem 371: Licence Plates

## Problem Statement

Oregon licence plates consist of three letters followed by a three-digit number (000 to 999). While driving to work you play a game: each plate you see gives you its three-digit number $N$; if you have previously seen a plate with number $M$ such that $N + M = 1000$, you win. Plates are observed independently and uniformly at random from the 1000 possible numbers. What is the expected number of plates you need to observe before winning? Give the answer rounded to 8 decimal places.

## Mathematical Foundation

The plate numbers $\{0, 1, \ldots, 999\}$ partition into:
- 499 **complementary pairs** $\{i, 1000 - i\}$ for $i = 1, 2, \ldots, 499$.
- The **self-complementary** number 500, since $500 + 500 = 1000$.
- The **inert** number 000, which has no complement in $\{0, \ldots, 999\}$.

We model the process as a **Markov chain** with state $(k, s)$ where $k \in \{0, 1, \ldots, 499\}$ counts the number of complementary pairs with exactly one element seen, and $s \in \{0, 1\}$ indicates whether plate 500 has been observed.

**Theorem (Transition Probabilities).** From state $(k, s)$, the next plate (drawn uniformly from 1000 values) produces the following transitions:

*Case $s = 0$ (plate 500 not yet seen):*
- Win with probability $\frac{k}{1000}$ (seeing the complement of one of the $k$ active singletons).
- Transition to $(k, 1)$ with probability $\frac{1}{1000}$ (seeing plate 500 for the first time).
- Transition to $(k+1, 0)$ with probability $\frac{2(499 - k)}{1000}$ (seeing a new element from an untouched pair).
- Stay at $(k, 0)$ with probability $\frac{k + 1}{1000}$ (seeing 000, or re-seeing an already-active singleton).

*Case $s = 1$ (plate 500 already seen):*
- Win with probability $\frac{k + 1}{1000}$ (seeing a complement of an active singleton, or seeing 500 again).
- Transition to $(k+1, 1)$ with probability $\frac{2(499 - k)}{1000}$.
- Stay at $(k, 1)$ with probability $\frac{k + 1}{1000}$.

**Proof.** There are 1000 equally likely plate numbers. In state $(k, s)$:
- Exactly $k$ numbers are complements of active singletons (winning numbers from pairs).
- If $s = 1$, plate 500 itself is also a winning number (since $500 + 500 = 1000$), giving $k + 1$ winners total.
- If $s = 0$, plate 500 is a non-winning unseen special number: 1 such number.
- The $499 - k$ untouched pairs contribute $2(499 - k)$ fresh numbers.
- Plate 000 is always inert (1 number).
- The $k$ active singletons themselves, if re-seen, do nothing (these $k$ numbers are already counted as seen).

Summing in case $s = 0$: $k + 1 + 2(499 - k) + 1 + k = 1000$. Check: $k + 1 + 998 - 2k + 1 + k = 1000$. $\checkmark$

Summing in case $s = 1$: $(k + 1) + 2(499 - k) + (k + 1) = k + 1 + 998 - 2k + k + 1 = 1000$. Here the $(k+1)$ wasted includes 000, re-seen singletons, and re-seen 500 (but 500 wins, so actually the waste count for $s=1$ is: 000 plus $k$ re-seen singletons $= k + 1$, and the $k+1$ winning numbers include $k$ complements plus 500). Total: $(k+1) + 2(499-k) + (k+1) = 1000$. $\checkmark$ $\square$

**Lemma (Recurrence).** Let $E(k, s)$ denote the expected number of additional plates needed from state $(k, s)$. Then:

$$E(k, 1) = \frac{1000 + 2(499 - k)\,E(k+1, 1)}{999 - k}$$

$$E(k, 0) = \frac{1000 + E(k, 1) + 2(499 - k)\,E(k+1, 0)}{999 - k}$$

**Proof.** Conditioning on the next observation:

$$E(k, 1) = 1 + \frac{k+1}{1000}\,E(k,1) + \frac{2(499-k)}{1000}\,E(k+1,1)$$

Rearranging: $(1 - \frac{k+1}{1000})\,E(k,1) = 1 + \frac{2(499-k)}{1000}\,E(k+1,1)$, giving the first equation. The derivation for $E(k,0)$ is analogous. $\square$

**Lemma (Base Case).** $E(499, 1) = 2$ and $E(499, 0) = \frac{1002}{500}$.

**Proof.** At $k = 499$, all pairs are active. With $s = 1$, win probability is $\frac{500}{1000} = \frac{1}{2}$, and no new pairs exist, so $E(499,1) = \frac{1000}{500} = 2$. With $s = 0$, $E(499, 0) = \frac{1000 + E(499,1)}{500} = \frac{1002}{500}$. $\square$

## Algorithm

```
function solve():
    E1[499] = 2.0
    E0[499] = 1002.0 / 500.0
    for k = 498 downto 0:
        E1[k] = (1000 + 2*(499 - k)*E1[k+1]) / (999 - k)
        E0[k] = (1000 + E1[k] + 2*(499 - k)*E0[k+1]) / (999 - k)
    return E0[0]
```

## Complexity Analysis

- **Time:** $O(n)$ where $n = 499$, i.e., $O(1)$ in terms of the fixed problem size.
- **Space:** $O(n)$ for storing the two arrays (reducible to $O(1)$ since we iterate downward and only need the next value).

## Answer

$$\boxed{40.66368097}$$
