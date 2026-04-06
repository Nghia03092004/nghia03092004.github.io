# Problem 121: Disc Game Prize Fund

## Problem Statement

A bag contains one red disc and one blue disc. In a game of chance, the player pays $1 to play and the following procedure is repeated each turn:

- A red disc is added to the bag.
- A disc is randomly drawn from the bag and placed to one side (not returned).

The player wins if, after $n = 15$ turns, they have drawn more blue discs than red discs. Determine the maximum prize fund the banker should allocate for a single game.

## Mathematical Development

**Definition 1.** Let $n$ denote the number of turns. At turn $k$ ($1 \le k \le n$), the bag contains one blue disc and $k$ red discs (the original red disc, plus the $k - 1$ previously added red discs, plus the one added at the start of turn $k$), for a total of $k + 1$ discs.

**Theorem 1 (Turn probabilities and independence).** *At turn $k$, $\Pr(\text{blue at turn } k) = \frac{1}{k+1}$ and $\Pr(\text{red at turn } k) = \frac{k}{k+1}$. The outcomes across distinct turns are mutually independent.*

*Proof.* Consider the equivalent probabilistic model. Before turn $k$, one red disc is added to the bag and then one disc is drawn uniformly at random from the $k + 1$ discs present. Since the drawn disc is set aside and one new red disc replaces it before the next turn, the bag at each turn $k$ is deterministically composed of 1 blue and $k$ red discs, regardless of prior history. Consequently, the conditional probability of drawing blue at turn $k$ given any history equals $1/(k+1)$, and by the definition of conditional independence, the turn outcomes are mutually independent. $\square$

**Theorem 2 (Winning probability).** *The probability that the player wins (draws at least $\lceil(n+1)/2\rceil$ blue discs in $n$ turns) is*
$$P(\text{win}) = \frac{N}{(n+1)!},$$
*where*
$$N = \sum_{\substack{T \subseteq \{1, \ldots, n\} \\ |T| \le \lfloor n/2 \rfloor}} \prod_{k \in T} k,$$
*and $T$ indexes the set of turns on which red is drawn.*

*Proof.* Let $S \subseteq \{1, \ldots, n\}$ be the set of turns on which blue is drawn, and set $T = \{1,\ldots,n\} \setminus S$. By Theorem 1 and the multiplication rule for independent events,
$$\Pr(S) = \prod_{k \in S} \frac{1}{k+1} \cdot \prod_{k \in T} \frac{k}{k+1} = \frac{\prod_{k \in T} k}{\prod_{k=1}^{n}(k+1)}.$$
The common denominator is $\prod_{k=1}^{n}(k+1) = 2 \cdot 3 \cdots (n+1) = (n+1)!$. The player wins when $|S| > n/2$, equivalently $|T| \le \lfloor n/2 \rfloor$. Summing the numerators $\prod_{k \in T} k$ over all qualifying $T$ yields $N$, and therefore $P(\text{win}) = N / (n+1)!$. $\square$

**Lemma 1 (Elementary symmetric polynomial recurrence).** *Define $e(i, j)$ to be the $j$-th elementary symmetric polynomial of $\{1, 2, \ldots, i\}$, i.e., the sum of products of all $j$-element subsets. Then*
$$e(i, j) = e(i-1, j) + i \cdot e(i-1, j-1),$$
*with boundary conditions $e(0, 0) = 1$ and $e(i, j) = 0$ for $j < 0$ or $j > i$. Moreover,*
$$N = \sum_{j=0}^{\lfloor n/2 \rfloor} e(n, j).$$

*Proof.* Partition the $j$-element subsets of $\{1, \ldots, i\}$ by membership of $i$:
- Subsets not containing $i$: these are $j$-element subsets of $\{1, \ldots, i-1\}$, contributing $e(i-1, j)$.
- Subsets containing $i$: formed by selecting a $(j-1)$-element subset of $\{1, \ldots, i-1\}$ and multiplying by $i$, contributing $i \cdot e(i-1, j-1)$.

Since each subset $T$ of $\{1, \ldots, n\}$ with $|T| = j$ contributes $\prod_{k \in T} k$ to $e(n, j)$, summing $e(n, j)$ for $0 \le j \le \lfloor n/2 \rfloor$ yields $N$. $\square$

**Theorem 3 (Maximum prize fund).** *The maximum prize fund is $\lfloor (n+1)! / N \rfloor$.*

*Proof.* The banker charges $1 per game. For a prize of $M$ dollars, the expected payout is $M \cdot P(\text{win}) = MN/(n+1)!$. Non-negative expected profit requires $MN/(n+1)! \le 1$, hence $M \le (n+1)!/N$. The maximum integer prize is therefore $\lfloor (n+1)!/N \rfloor$. $\square$

## Algorithm

```
function disc_game_prize(n):
    dp[0..floor(n/2)] initialized to 0
    dp[0] = 1

    for i = 1 to n:
        for j = min(i, floor(n/2)) down to 1:
            dp[j] = dp[j] + i * dp[j-1]

    N = sum(dp[0..floor(n/2)])
    D = (n+1)!
    return floor(D / N)
```

## Complexity Analysis

- **Time:** The double loop performs $\sum_{i=1}^{n} \min(i, \lfloor n/2 \rfloor)$ iterations, which is $O(n \cdot \lfloor n/2 \rfloor) = O(n^2)$.
- **Space:** $O(n)$ for the one-dimensional DP array, updated in-place by processing the inner index in decreasing order.

## Answer

$$\boxed{2269}$$
