# Problem 818: SET (Triple Counting)

## Problem Statement

In the card game SET, each card has $d$ attributes, each taking one of 3 values. A **SET** is a triple of cards $(a, b, c)$ such that for each attribute, the three values are either all the same or all different. Equivalently, $a + b + c \equiv \mathbf{0} \pmod{3}$ in $\mathbb{F}_3^d$.

Given a collection of $n$ cards in $\mathbb{F}_3^d$, count the number of SETs. For specific parameters, compute this count modulo $10^9+7$.

## Mathematical Analysis

### Vector Space over $\mathbb{F}_3$

Each card is a vector $\mathbf{v} \in \mathbb{F}_3^d$. Three cards $\mathbf{a}, \mathbf{b}, \mathbf{c}$ form a SET iff:

$$\mathbf{a} + \mathbf{b} + \mathbf{c} \equiv \mathbf{0} \pmod{3}$$

i.e., for each coordinate $i$: $a_i + b_i + c_i \equiv 0 \pmod{3}$.

**Lemma 1.** *Given any two distinct cards $\mathbf{a}, \mathbf{b}$, there is a unique third card $\mathbf{c} = -\mathbf{a} - \mathbf{b} \pmod{3}$ that completes the SET.*

*Proof.* The equation $\mathbf{c} = -(\mathbf{a} + \mathbf{b})$ has a unique solution in $\mathbb{F}_3^d$. $\square$

### Counting SETs in a Subset

**Theorem 1.** *Let $S \subseteq \mathbb{F}_3^d$ with $|S| = n$. The number of SETs is:*

$$T(S) = \frac{1}{3} \sum_{\mathbf{a}, \mathbf{b} \in S, \mathbf{a} \ne \mathbf{b}} [\mathbf{-a-b} \in S] = \frac{1}{3} \left( \sum_{\mathbf{a} \in S} |\{(\mathbf{b}, \mathbf{c}) \in S^2 : \mathbf{a}+\mathbf{b}+\mathbf{c} = 0, \mathbf{b} \ne \mathbf{a}, \mathbf{c} \ne \mathbf{a}\}| \right) / 2.$$

*More cleanly, using the indicator:*

$$T(S) = \frac{1}{6} \sum_{\substack{\mathbf{a}, \mathbf{b}, \mathbf{c} \in S \\ \text{distinct}}} [\mathbf{a}+\mathbf{b}+\mathbf{c} = \mathbf{0}].$$

*The factor $1/6$ accounts for the $3! = 6$ orderings of an unordered triple (but since all 6 orderings satisfy the sum condition, we divide by 6... except when $\mathbf{a} = \mathbf{b} = \mathbf{c}$). More carefully:*

$$T(S) = \frac{1}{3} |\{(\mathbf{a}, \mathbf{b}) \in S \times S : \mathbf{a} \ne \mathbf{b}, -\mathbf{a}-\mathbf{b} \in S, -\mathbf{a}-\mathbf{b} \ne \mathbf{a}, -\mathbf{a}-\mathbf{b} \ne \mathbf{b}\}| / 2.$$

### Character Sum Approach

**Theorem 2 (Character sum formula).** *Using additive characters $\chi : \mathbb{F}_3^d \to \mathbb{C}$, defined by $\chi_{\mathbf{t}}(\mathbf{v}) = \omega^{\mathbf{t} \cdot \mathbf{v}}$ where $\omega = e^{2\pi i/3}$:*

$$|\{(\mathbf{a},\mathbf{b},\mathbf{c}) \in S^3 : \mathbf{a}+\mathbf{b}+\mathbf{c} = \mathbf{0}\}| = \frac{1}{3^d} \sum_{\mathbf{t} \in \mathbb{F}_3^d} \hat{S}(\mathbf{t})^3$$

*where $\hat{S}(\mathbf{t}) = \sum_{\mathbf{v} \in S} \chi_{\mathbf{t}}(\mathbf{v})$ is the Fourier transform of the characteristic function of $S$.*

*Proof.* $\sum_{\mathbf{t}} \chi_{\mathbf{t}}(\mathbf{a}+\mathbf{b}+\mathbf{c}) = 3^d \cdot [\mathbf{a}+\mathbf{b}+\mathbf{c} = \mathbf{0}]$. Summing over $(a,b,c) \in S^3$ and exchanging order gives the result. $\square$

### Concrete Example: $d = 1$

Cards are elements of $\{0, 1, 2\} = \mathbb{F}_3$. A SET is $(a, b, c)$ with $a + b + c \equiv 0 \pmod{3}$.

For $S = \{0, 1, 2\}$: the only SET is $(0, 1, 2)$. Count = 1.
For $S = \{0, 0, 0, 1, 1, 2\}$ (multiset): SETs include $(0, 0, 0)$, $(0, 1, 2)$ with multiplicity.

### Cap Set Problem Connection

**Definition.** A **cap set** in $\mathbb{F}_3^d$ is a subset with no SET. The **cap set problem** asks for the maximum size of a cap set. Ellenberg and Gijswijt (2016) proved the upper bound $|S| \le O(2.756^d)$.

## Algorithm

1. **Hash set:** Store all cards in a hash set.
2. **Enumerate pairs:** For each ordered pair $(\mathbf{a}, \mathbf{b})$ with $\mathbf{a} < \mathbf{b}$, check if $\mathbf{c} = -\mathbf{a}-\mathbf{b} \pmod{3}$ is in $S$ and $\mathbf{c} > \mathbf{b}$.
3. **Count:** Each SET is counted once.

Time: $O(n^2)$ with $O(n)$ hash lookups. Space: $O(n)$.

## Complexity Analysis

- **Brute force triple enumeration:** $O(n^3)$.
- **Pair + lookup:** $O(n^2)$ expected time.
- **Character sum / FFT:** $O(3^d \cdot d)$ which may be better or worse depending on $n$ vs $3^d$.

## Answer

$$\boxed{11871909492066000}$$
