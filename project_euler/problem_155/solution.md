# Problem 155: Counting Capacitor Circuits

## Problem Statement

Using up to 18 unit capacitors (each of capacitance 1) connected in series and/or parallel combinations, how many distinct capacitance values can be obtained?

Let $D(n)$ be the set of distinct capacitance values achievable using exactly $n$ capacitors. Find $C(18) = |D(1) \cup D(2) \cup \cdots \cup D(18)|$.

## Mathematical Foundation

**Theorem 1 (Recursive Construction).** *Let $D(n)$ be the set of distinct capacitance values achievable with exactly $n$ unit capacitors. Then $D(1) = \{1\}$ and for $n \ge 2$:*

$$D(n) = \bigcup_{k=1}^{\lfloor n/2 \rfloor} \left\{ a + b \mid a \in D(k),\, b \in D(n-k) \right\} \cup \left\{ \frac{ab}{a+b} \;\middle|\; a \in D(k),\, b \in D(n-k) \right\}$$

**Proof.** Any circuit of $n \ge 2$ capacitors has a top-level structure that is either a parallel or series combination of two sub-circuits. If the sub-circuits use $k$ and $n - k$ capacitors respectively ($1 \le k \le n - 1$), then:
- Parallel: total capacitance $= a + b$.
- Series: total capacitance $= \frac{1}{1/a + 1/b} = \frac{ab}{a+b}$.

Since the combination is commutative ($a + b = b + a$ and the harmonic formula is symmetric), we restrict to $k \le n - k$, i.e., $k \le \lfloor n/2 \rfloor$, without loss of generality. Every achievable capacitance arises from some such decomposition (by structural induction on circuits), and conversely every value produced by the formula is achievable. $\square$

**Theorem 2 (Rationality).** *Every element of $D(n)$ is a positive rational number.*

**Proof.** By induction on $n$. Base case: $D(1) = \{1\} \subset \mathbb{Q}_{>0}$. Inductive step: if $a, b \in \mathbb{Q}_{>0}$, then $a + b \in \mathbb{Q}_{>0}$ and $ab/(a+b) \in \mathbb{Q}_{>0}$ (since $a + b > 0$). $\square$

**Lemma 1 (Series-Parallel Duality).** *If $c \in D(n)$, then $1/c \in D(n)$.*

**Proof.** By induction on circuit structure. Base: $1 \in D(1)$ and $1/1 = 1 \in D(1)$. Inductive step: if circuit $A$ achieves capacitance $a$ and circuit $B$ achieves $b$, then:
- Parallel of $A, B$ gives $a + b$. Replacing with series gives $ab/(a+b) = 1/(1/a + 1/b)$.
- Series of $A, B$ gives $ab/(a+b)$.

By the inductive hypothesis, circuits $A', B'$ achieving $1/a, 1/b$ exist. Series of $A', B'$ gives $\frac{(1/a)(1/b)}{1/a + 1/b} = \frac{1}{a+b}$, which is $1/(a+b)$.

Thus swapping every parallel with series and vice versa transforms $c$ to $1/c$. $\square$

**Lemma 2 (Representation).** *Each capacitance is stored as a reduced fraction $(p, q)$ with $\gcd(p, q) = 1$ and $q > 0$. Two capacitances are equal iff their reduced fractions are identical.*

**Proof.** The rational numbers form a field, and the reduced-fraction representation is canonical. $\square$

## Editorial
S(1) = {1} S(n) = union over k=1..n//2 of { a+b, a*b/(a+b) : a in S(k), b in S(n-k) } C(n) = |S(1) union ... union S(n)| Uses tuples (numerator, denominator) for speed instead of Fraction. We compute cumulative union.

## Pseudocode

```text
    D = array of sets, indexed 1..N
    D[1] = {Fraction(1, 1)}

    For n from 2 to N:
        D[n] = empty set
        For k from 1 to n/2:
            For each a in D[k]:
                For each b in D[n-k]:
                    D[n].add(a + b) // parallel
                    D[n].add(a * b / (a + b)) // series

    Compute cumulative union
    all_values = D[1] union D[2] union ... union D[N]
    Return |all_values|
```

## Complexity Analysis

- **Time:** For each $n$, we iterate over $\lfloor n/2 \rfloor$ splits. For each split $(k, n-k)$, we compute $|D(k)| \times |D(n-k)|$ combinations. Let $s_n = |D(n)|$. The total work is $\sum_{n=2}^{N} \sum_{k=1}^{\lfloor n/2 \rfloor} s_k \cdot s_{n-k}$. Empirically, $s_n$ grows to several hundred thousand for $n \le 18$, making the total work on the order of $10^9$ set insertions. Each insertion into a hash set is $O(1)$ amortized.
- **Space:** $O\!\left(\sum_{n=1}^{N} s_n\right)$ to store all sets. For $N = 18$, this is on the order of millions of fractions.

## Answer

$$\boxed{3857447}$$
