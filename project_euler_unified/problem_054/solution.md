# Problem 54: Poker Hands

## Problem Statement

The file `poker.txt` contains one-thousand random hands dealt to two players. Each line contains ten cards (separated by spaces): the first five are Player 1's hand and the last five are Player 2's hand. How many hands does Player 1 win?

## Formal Development

**Definition 1 (Card and Hand).** A *card* is a pair $(v, s)$ where $v \in \{2, 3, \ldots, 14\}$ is the *value* (with $11 = \mathrm{J}$, $12 = \mathrm{Q}$, $13 = \mathrm{K}$, $14 = \mathrm{A}$) and $s \in \{\clubsuit, \diamondsuit, \heartsuit, \spadesuit\}$ is the *suit*. A *hand* is an ordered 5-tuple of distinct cards $H = (c_1, c_2, c_3, c_4, c_5)$.

**Definition 2 (Derived Quantities).** For a hand $H$ with values $v_1, \ldots, v_5$ and suits $s_1, \ldots, s_5$, define:
- *Frequency function*: $f(v) = |\{i : v_i = v\}|$ for each $v \in \{2, \ldots, 14\}$.
- *Frequency partition*: The multiset $\{f(v) : f(v) > 0\}$, sorted in decreasing order.
- *Flush predicate*: $\phi(H) \iff s_1 = s_2 = \cdots = s_5$.
- *Straight predicate*: $\sigma(H) \iff$ the sorted distinct values form 5 consecutive integers, or equal $\{2, 3, 4, 5, 14\}$ (the ace-low, or "wheel," straight).

**Definition 3 (Hand Category).** The *category* $c(H) \in \{0, 1, \ldots, 9\}$ is defined by the following decision procedure, evaluated in order of priority (highest first):

| $c$ | Name | Condition |
|-----|------|-----------|
| 9 | Royal Flush | $\sigma(H) \wedge \phi(H) \wedge \max(v_i) = 14 \wedge \min(v_i) = 10$ |
| 8 | Straight Flush | $\sigma(H) \wedge \phi(H) \wedge \neg(c = 9)$ |
| 7 | Four of a Kind | Frequency partition = $(4, 1)$ |
| 6 | Full House | Frequency partition = $(3, 2)$ |
| 5 | Flush | $\phi(H) \wedge \neg\sigma(H)$ |
| 4 | Straight | $\sigma(H) \wedge \neg\phi(H)$ |
| 3 | Three of a Kind | Frequency partition = $(3, 1, 1)$ |
| 2 | Two Pairs | Frequency partition = $(2, 2, 1)$ |
| 1 | One Pair | Frequency partition = $(2, 1, 1, 1)$ |
| 0 | High Card | Frequency partition = $(1, 1, 1, 1, 1) \wedge \neg\sigma(H) \wedge \neg\phi(H)$ |

**Definition 4 (Comparison Key).** The *comparison key* $\kappa(H)$ is a tuple enabling lexicographic comparison:

$$\kappa(H) = \bigl(c(H),\; \tau_1,\; \tau_2,\; \ldots,\; \tau_m\bigr),$$

where $(\tau_1, \ldots, \tau_m)$ are tiebreaker values defined as follows:
- For straights and straight flushes: $\tau_1 = $ the high card of the straight (with the wheel having high card 5, not 14).
- For all other categories: the distinct values appearing in $H$, sorted by $(f(v), v)$ in descending order.

**Theorem 1 (Correctness of Lexicographic Comparison).** For any two hands $H_1, H_2$ (with no ties, as guaranteed by the problem), $H_1$ beats $H_2$ if and only if $\kappa(H_1) > \kappa(H_2)$ in lexicographic order.

*Proof.* The proof proceeds by case analysis on the structure of the comparison key.

*Primary comparison.* If $c(H_1) \neq c(H_2)$, the hand with higher category wins. This is correct since the category ordering reflects the standard poker hand hierarchy.

*Tiebreaking within a category.* Suppose $c(H_1) = c(H_2) = c$. The standard poker rules prescribe:

1. **Four of a Kind** ($c = 7$): Compare the value of the quadruple, then the kicker. The key $(\tau_1, \tau_2)$ = (quad value, kicker value) achieves this.

2. **Full House** ($c = 6$): Compare the triple value, then the pair value. The key $(\tau_1, \tau_2)$ = (triple value, pair value) achieves this.

3. **Flush / High Card** ($c \in \{0, 5\}$): Compare values in descending order. Since all frequencies are 1, sorting by $(f(v), v)$ descending is equivalent to sorting by $v$ descending.

4. **Straight / Straight Flush** ($c \in \{4, 8, 9\}$): The straight's high card determines the winner.

5. **Three of a Kind** ($c = 3$): Compare triple value, then kickers in descending order.

6. **Two Pairs** ($c = 2$): Sorting by $(f(v), v)$ descending yields (higher pair, lower pair, kicker), which is the correct comparison order.

7. **One Pair** ($c = 1$): Sorting yields (pair value, then kickers descending).

In each case, the lexicographic ordering of $\kappa$ faithfully implements the poker comparison rules. $\blacksquare$

**Lemma 1 (Ace-Low Straight Correction).** When the sorted values are $\{2, 3, 4, 5, 14\}$ and the straight predicate holds, the ace must be reinterpreted as value 1. The comparison key uses high card $= 5$.

*Proof.* By standard poker rules, $A$-$2$-$3$-$4$-$5$ is the lowest straight (high card 5). Without this correction, the ace (value 14) would make this straight rank above $2$-$3$-$4$-$5$-$6$ (high card 6), contradicting the rules. $\blacksquare$

## Algorithm

```
POKER_HANDS(filename):
    wins := 0
    For each line in file:
        cards := parse_line(line)
        H1 := cards[0..4], H2 := cards[5..9]
        if kappa(H1) > kappa(H2):
            wins := wins + 1
    Return wins

COMPARISON_KEY(H):
    V := sorted values of H (descending)
    S := suits of H
    f := frequency_count(V)
    flush := |set(S)| = 1
    // Straight check
    U := sorted(set(V))
    straight := (|U| = 5 and U[4] - U[0] = 4) or U = [2,3,4,5,14]
    // Ace-low correction
    if U = [2,3,4,5,14] and straight:
        high := 5
    else if straight:
        high := max(V)
    // Classify category c
    c := classify(f, flush, straight)
    // Build tiebreaker
    if c in {4, 8, 9}:
        tau := (high,)
    else:
        tau := distinct values sorted by (f(v), v) descending
    Return (c, tau)
```

## Complexity Analysis

**Proposition 1 (Time).** The algorithm processes $H = 1000$ hands. Each hand evaluation involves:
- Parsing 5 cards: $O(5) = O(1)$.
- Sorting 5 values: $O(5 \log 5) = O(1)$.
- Frequency counting over at most 13 values: $O(1)$.
- Category classification and key construction: $O(1)$.
- Lexicographic comparison of fixed-length tuples: $O(1)$.

Total: $O(H) = O(1000)$.

**Proposition 2 (Space).** $O(1)$ per hand evaluation. $O(H)$ if all lines are stored; $O(1)$ if processed line-by-line.

## Answer

$$\boxed{376}$$
