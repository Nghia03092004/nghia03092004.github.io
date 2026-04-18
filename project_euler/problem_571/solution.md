# Problem 571: Super Pandigital Numbers

## Problem Statement

A positive number is **pandigital in base $b$** if it contains all digits from $0$ to $b-1$ when written in base $b$. An **$n$-super-pandigital number** is a number that is simultaneously pandigital in all bases from $2$ to $n$ inclusively.

For example, $978$ is the smallest 5-super-pandigital number. The number $1093265784$ is the smallest 10-super-pandigital number, and the sum of the 10 smallest 10-super-pandigital numbers is $20319792309$.

**Find the sum of the 10 smallest 12-super-pandigital numbers.**

## Mathematical Foundation

**Definition 1.** A positive integer $N$ is *pandigital in base $b$* if the multiset of digits in the base-$b$ representation of $N$ contains every element of $\{0, 1, \ldots, b-1\}$ at least once.

**Theorem 1 (Minimum digit count).** If $N$ is pandigital in base $b$, then $N$ has at least $b$ digits in base $b$, and consequently $N \geq b^{b-1}$.

*Proof.* The base-$b$ representation of $N$ must include each of the $b$ distinct symbols $\{0, 1, \ldots, b-1\}$ at least once. Hence the representation has at least $b$ digits. Since $N > 0$, the leading digit is nonzero, so $N \geq 1 \cdot b^{b-1} = b^{b-1}$. $\square$

**Lemma 1 (Base-12 structure).** A 12-super-pandigital number $N$ that is minimally pandigital in base 12 (i.e., has exactly 12 base-12 digits) satisfies $12^{11} \leq N < 12^{12}$, and its base-12 representation is a permutation of $\{0, 1, 2, \ldots, 11\}$ with leading digit $\geq 1$.

*Proof.* By Theorem 1, $N$ has at least 12 digits in base 12. Suppose $N$ has exactly 12 digits. Then $12^{11} \leq N < 12^{12}$. Each of the 12 required symbols must appear at least once among the 12 digit positions. By the pigeonhole principle, each symbol appears *exactly* once, so the digit string is a permutation of $\{0, 1, \ldots, 11\}$. The leading digit must be nonzero since $N \geq 12^{11} > 0$. $\square$

**Remark.** We restrict the search to exactly-12-digit base-12 numbers. A number with 13 or more base-12 digits would satisfy $N \geq 12^{12} \approx 8.9 \times 10^{12}$, which is far larger than the smallest 12-super-pandigital numbers (which turn out to be around $1.8 \times 10^{11}$). Hence the 10 smallest are all 12-digit base-12 numbers.

**Lemma 2 (Bitmask pandigitality test).** A number $N$ is pandigital in base $b$ if and only if the set $\{d_i : i = 0, 1, \ldots, \lfloor \log_b N \rfloor\}$ equals $\{0, 1, \ldots, b-1\}$, where $d_i = \lfloor N / b^i \rfloor \bmod b$. This can be tested in $O(\log_b N)$ time using a $b$-bit mask.

*Proof.* The digits of $N$ in base $b$ are precisely $d_i = \lfloor N / b^i \rfloor \bmod b$ for $i = 0, 1, \ldots, \lfloor \log_b N \rfloor$. Maintain a bitmask $m$ (initially $0$) where bit $j$ is set when digit $j$ is encountered: $m \leftarrow m \mathbin{|} (1 \ll j)$. After processing all digits, $N$ is pandigital in base $b$ if and only if $m = 2^b - 1$. Each digit extraction and bitmask update takes $O(1)$ time, and there are $\lfloor \log_b N \rfloor + 1$ digits. $\square$

**Proposition 1 (Early termination ordering).** When checking pandigitality across bases $2, 3, \ldots, 11$, testing base 11 first maximizes the rejection rate. A random 12-digit base-12 permutation is pandigital in base 11 with probability approximately $11!/11^{11} \approx 1.4 \times 10^{-3}$, so the vast majority of candidates are rejected at the first check.

*Proof.* A base-12 number with 12 digits has approximately $\lceil 12 \log 12 / \log 11 \rceil = 13$ digits in base 11. Pandigitality in base 11 requires all 11 symbols among these $\sim 13$ digits, and the fraction of such arrangements is small. More precisely, by inclusion-exclusion on missing symbols, the probability of pandigitality in base 11 is $\sum_{k=0}^{11} (-1)^k \binom{11}{k} (1 - k/11)^{13}$, which evaluates to roughly $0.0014$. $\square$

## Editorial
We iterate over each permutation P of remaining. Finally, iterate over b from 11 down to 2. Candidates are generated from the derived formulas, filtered by the required conditions, and processed in order until the desired value is obtained.

## Pseudocode

```text
for each permutation P of remaining
for b from 11 down to 2
```

## Complexity Analysis

- **Time.** The outer loop iterates over at most $11 \cdot 11!$ permutations ($\approx 4.4 \times 10^8$). For each candidate, the base-11 pandigitality check costs $O(\log_{11} N) = O(13)$ digit extractions. With the empirical rejection rate of $\sim 99.86\%$ at base 11, only $\sim 6 \times 10^5$ candidates survive to be tested in base 10, and further attrition occurs at each subsequent base. The effective cost is dominated by the base-11 filtering: $O(11! \times 13) \approx 5 \times 10^8$ operations for the first digit alone.
- **Space.** $O(12)$ for the permutation and bitmask storage, i.e., $O(1)$ auxiliary.

## Answer

$$\boxed{30510390701978}$$
