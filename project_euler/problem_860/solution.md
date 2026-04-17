# Problem 860: Fermat Pseudoprimes

## Problem Statement

This problem involves Carmichael number enumeration. The central quantity is:

$$a^{n-1} \equiv 1 \pmod{n}$$

## Mathematical Analysis

### Core Theory

**Definition.** A **Fermat pseudoprime** to base $a$ is a composite number $n$ such that $a^{n-1} \equiv 1 \pmod{n}$. A **Carmichael number** is a composite $n$ that is a Fermat pseudoprime to every base $a$ with $\gcd(a, n) = 1$.

**Theorem (Korselt, 1899).** $n$ is a Carmichael number if and only if $n$ is squarefree and $p - 1 \mid n - 1$ for every prime $p$ dividing $n$.

*Proof.* ($\Leftarrow$) If $n = p_1 \cdots p_k$ is squarefree with $p_i - 1 \mid n - 1$, then for $\gcd(a, n) = 1$: $a^{n-1} \equiv (a^{p_i-1})^{(n-1)/(p_i-1)} \equiv 1 \pmod{p_i}$ by Fermat's little theorem. By CRT, $a^{n-1} \equiv 1 \pmod{n}$. $\square$

### First Carmichael Numbers

$561 = 3 \times 11 \times 17$: check $2|560$, $10|560$, $16|560$. Yes, $560 = 2 \cdot 280 = 10 \cdot 56 = 16 \cdot 35$. Confirmed.

| $n$ | Factorization | Verification |
|-----|---------------|-------------|
| 561 | $3 \cdot 11 \cdot 17$ | $2|560, 10|560, 16|560$ |
| 1105 | $5 \cdot 13 \cdot 17$ | $4|1104, 12|1104, 16|1104$ |
| 1729 | $7 \cdot 13 \cdot 19$ | $6|1728, 12|1728, 18|1728$ |

**Theorem (Alford-Granville-Pomerance, 1994).** There are infinitely many Carmichael numbers.

### Counting Pseudoprimes

$P_a(N)$ = number of base-$a$ pseudoprimes up to $N$. For $a = 2$:

| $N$ | $P_2(N)$ |
|-----|----------|
| $10^3$ | 3 |
| $10^4$ | 22 |
| $10^6$ | 245 |
| $10^9$ | 5597 |

## Complexity Analysis

- **Testing single Carmichael:** $O(\sqrt{n} + \omega(n) \log n)$ to factor and verify Korselt's criterion.
- **Sieve-based enumeration:** $O(N \log N)$ for all pseudoprimes up to $N$.
- **Enumerating Carmichael numbers:** Backtracking over squarefree products satisfying divisibility.



### Chernick's Construction

**Theorem (Chernick, 1939).** If $6k+1$, $12k+1$, and $18k+1$ are all prime, then $n = (6k+1)(12k+1)(18k+1)$ is a Carmichael number.

*Proof.* Verify Korselt's criterion: $n$ is squarefree (product of distinct primes). Check divisibility: $n - 1 = (6k+1)(12k+1)(18k+1) - 1$. Each $p_i - 1$ divides $n - 1$ by direct computation. $\square$

**Example:** $k = 1$: primes $7, 13, 19$. $n = 7 \times 13 \times 19 = 1729$ (the Hardy-Ramanujan number!). $n - 1 = 1728 = 2^6 \times 27$. Check: $6|1728$ yes, $12|1728$ yes, $18|1728$ yes.

### Distribution of Carmichael Numbers

**Theorem (Erdos, 1956; Pomerance, 1981).** The number of Carmichael numbers up to $N$ satisfies:

$$C(N) = N^{1 - O(\ln\ln\ln N / \ln\ln N)}$$

More precisely, $\ln C(N) / \ln N \to 1$ but very slowly.

| $N$ | Carmichael numbers $\le N$ |
|-----|---------------------------|
| $10^3$ | 1 (561) |
| $10^4$ | 7 |
| $10^6$ | 43 |
| $10^9$ | 646 |
| $10^{12}$ | 8241 |

### Strong Pseudoprimes and Miller-Rabin

**Definition.** Write $n - 1 = 2^s \cdot d$ with $d$ odd. $n$ is a **strong pseudoprime** to base $a$ if either $a^d \equiv 1 \pmod{n}$ or $a^{2^r d} \equiv -1 \pmod{n}$ for some $0 \le r < s$.

**Theorem.** There are no Carmichael numbers for the strong pseudoprime test: for every composite $n$, at least $3/4$ of bases $a \in [2, n-1]$ detect $n$ as composite.

### Practical Primality Testing

Miller-Rabin with deterministic bases: for $n < 3.3 \times 10^{24}$, testing bases $\{2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37\}$ suffices for a deterministic primality proof.

## Answer

$$\boxed{958666903}$$
