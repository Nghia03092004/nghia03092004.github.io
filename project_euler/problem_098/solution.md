# Problem 98: Anagramic Squares

## Problem Statement

An *anagram pair* consists of two distinct words that are permutations of the same multiset of letters. A *square anagram word pair* is an anagram pair $(w_1, w_2)$ admitting an injective mapping $\varphi$ from letters to digits such that both $\varphi(w_1)$ and $\varphi(w_2)$ are perfect squares with no leading zeros. Using the provided word list, find the largest perfect square formed by any member of such a pair.

## Mathematical Foundation

**Definition 1 (Anagram equivalence).** Two words $w_1, w_2 \in \Sigma^*$ are *anagrams* if one is a permutation of the other, i.e., they share the same sorted letter sequence: $\operatorname{sort}(w_1) = \operatorname{sort}(w_2)$.

**Definition 2 (Letter-digit substitution).** A *substitution* for a word $w$ of length $n$ with distinct letter set $\Lambda \subseteq \Sigma$ is an injective function $\varphi: \Lambda \to \{0, 1, \ldots, 9\}$. The substitution maps $w = \ell_1 \ell_2 \cdots \ell_n$ to the integer $\overline{\varphi(\ell_1)\varphi(\ell_2)\cdots\varphi(\ell_n)}$.

**Definition 3 (Character pattern).** The *character pattern* of a string $s = s_1 s_2 \cdots s_n$ is the tuple $(p_1, \ldots, p_n)$ obtained by replacing each character with its order of first appearance (0-indexed). For example, $\operatorname{pat}(\text{CARE}) = (0,1,2,3)$ and $\operatorname{pat}(\text{NOON}) = (0,1,1,0)$.

**Lemma 1 (Pattern characterization of substitutions).** A consistent injective substitution from word $w$ to $n$-digit number $m$ (both viewed as strings of length $n$) exists if and only if $\operatorname{pat}(w) = \operatorname{pat}(m)$.

**Proof.** ($\Rightarrow$) If $\varphi$ is a consistent injective substitution mapping $w$ to $m$, then identical letters in $w$ map to identical digits in $m$, and distinct letters map to distinct digits. This means the first-occurrence ordering is preserved: $\operatorname{pat}(w) = \operatorname{pat}(m)$.

($\Leftarrow$) If $\operatorname{pat}(w) = \operatorname{pat}(m)$, define $\varphi(\ell) = d$ whenever $\ell$ occupies the same position as $d$ in the pattern alignment. Equal patterns guarantee consistency (same letter always maps to the same digit) and injectivity (distinct pattern indices correspond to distinct characters/digits). $\square$

**Theorem 1 (Algorithm correctness).** The following procedure finds the largest square in any square anagram word pair:
1. Group words by $\operatorname{sort}(w)$; keep groups of size $\ge 2$.
2. For each anagram pair $(w_1, w_2)$ of length $n$, compute $\operatorname{pat}(w_1)$.
3. For each $n$-digit perfect square $s$ with $\operatorname{pat}(s) = \operatorname{pat}(w_1)$, extract the substitution $\varphi$ and apply it to $w_2$, yielding candidate $s'$.
4. Accept $(s, s')$ if $s'$ has no leading zero and $s'$ is a perfect square.
5. Return $\max(s, s')$ over all accepted pairs.

**Proof.** Step 1 correctly identifies all anagram pairs (by Definition 1). Step 2--3 enumerates all valid substitutions by Lemma 1. Step 4 checks the square anagram conditions. Step 5 selects the maximum. Since all anagram pairs and all compatible squares are considered, no valid pair is missed. $\square$

**Proposition 1 (Search space bound).** For word length $n$, the number of $n$-digit perfect squares is $\lfloor\sqrt{10^n - 1}\rfloor - \lceil\sqrt{10^{n-1}}\rceil + 1 = \Theta(10^{n/2})$. For the given word list with maximum length $L \le 14$, this is at most $\sim 10^7$ squares per length, and the number of anagram groups and pairs is small, making the algorithm efficient.

## Editorial
Candidates are generated from the derived formulas, filtered by the required conditions, and processed in order until the desired value is obtained.

## Pseudocode

```text
    groups = group words by sort(w)
    pairs = [(w1, w2) for each group with |group| >= 2, for each pair in group]
    max_len = max word length among pairs

    for each length n from max_len down to 1:
        squares_n = {s^2 : ceil(sqrt(10^{n-1})) <= s <= floor(sqrt(10^n - 1))}
        sq_by_pattern = group squares_n by pat(str(s^2))

        for each pair (w1, w2) of length n:
            p = pat(w1)
            For each each square s in sq_by_pattern[p]:
                phi = mapping from w1's letters to s's digits
                s' = apply phi to w2
                If s' has no leading zero and is_perfect_square(s') then
                    update best = max(best, s, s')

    Return best
```

## Complexity Analysis

**Time:** Let $W$ be the number of words, $L$ the maximum length. Grouping: $O(W L \log L)$. Per length $n$: enumerating squares is $O(10^{n/2})$; checking each pair against each pattern-matched square is $O(n)$. Total is dominated by the largest word length.

**Space:** $O(10^{L/2})$ for storing squares of the maximum length, plus $O(W)$ for word storage.

## Answer

$$\boxed{18769}$$
