# Problem 516: 5-Smooth Totients

## Problem Statement

A number is **5-smooth** (or a **Hamming number**) if its largest prime factor does not exceed 5. Let $S(L)$ be the sum of all positive integers $n \leq L$ such that $\varphi(n)$ is 5-smooth. Given $S(100) = 3728$, find $S(10^{12}) \bmod 2^{32}$.

## Mathematical Foundation

**Theorem 1 (Characterization of 5-Smooth Totient Numbers).** A positive integer $n$ satisfies "$\varphi(n)$ is 5-smooth" if and only if

$$n = 2^a \cdot 3^b \cdot 5^c \cdot p_1 \cdot p_2 \cdots p_k,$$

where $a, b, c \geq 0$, $k \geq 0$, and $p_1, p_2, \ldots, p_k$ are distinct primes (each $> 5$) such that $p_i - 1$ is a Hamming number for every $i$.

**Proof.** ($\Leftarrow$) By multiplicativity of $\varphi$ on pairwise coprime factors:

$$\varphi(n) = \varphi(2^a)\,\varphi(3^b)\,\varphi(5^c)\prod_{i=1}^{k}\varphi(p_i).$$

We have $\varphi(2^a) = 2^{a-1}$ (for $a \geq 1$; $\varphi(1) = 1$), $\varphi(3^b) = 2 \cdot 3^{b-1}$, $\varphi(5^c) = 4 \cdot 5^{c-1}$, and $\varphi(p_i) = p_i - 1$, each of which is 5-smooth by hypothesis. A product of 5-smooth numbers is 5-smooth, so $\varphi(n)$ is 5-smooth.

($\Rightarrow$) Suppose $\varphi(n)$ is 5-smooth. Let $q > 5$ be a prime dividing $n$.

- If $q^2 \mid n$, then $\varphi(q^2) = q(q-1)$ divides $\varphi(n)$. Since $q > 5$, the factor $q$ makes $\varphi(n)$ non-5-smooth, a contradiction. So $q \| n$ (exact power 1).
- Since $q \| n$, we have $\varphi(q) = q - 1 \mid \varphi(n)$. For $\varphi(n)$ to be 5-smooth, $q - 1$ must be 5-smooth.

Thus every prime $q > 5$ dividing $n$ appears to the first power and satisfies $q - 1$ being 5-smooth. The remaining part of $n$ is $2^a \cdot 3^b \cdot 5^c$, and $\varphi$ of any prime power of 2, 3, or 5 is 5-smooth. $\square$

**Lemma 1 (Hamming Number Count).** The number of Hamming numbers not exceeding $L$ is $O(\log^3 L)$.

**Proof.** A Hamming number $h = 2^a \cdot 3^b \cdot 5^c \leq L$ requires $a \leq \log_2 L$, $b \leq \log_3 L$, $c \leq \log_5 L$. The count is at most $(\log_2 L + 1)(\log_3 L + 1)(\log_5 L + 1) = O(\log^3 L)$. For $L = 10^{12}$, this is approximately 3,428. $\square$

**Lemma 2 (Hamming Prime Count).** A *Hamming prime* is a prime $p$ such that $p - 1$ is a Hamming number. The number of Hamming primes up to $L$ is at most the number of Hamming numbers up to $L$, i.e., $O(\log^3 L)$. For $L = 10^{12}$, there are approximately 545 such primes.

**Proof.** Each Hamming prime $p$ corresponds to a unique Hamming number $p - 1$, so the count is bounded by the number of Hamming numbers. $\square$

## Algorithm

```
function S(L):
    // Step 1: Generate all Hamming numbers up to L
    hamming := []
    for a := 0 while 2^a <= L:
        for b := 0 while 2^a * 3^b <= L:
            for c := 0 while 2^a * 3^b * 5^c <= L:
                hamming.append(2^a * 3^b * 5^c)
    sort(hamming)

    // Step 2: Find Hamming primes
    hamming_primes := []
    for h in hamming:
        if is_prime(h + 1) and h + 1 > 5:
            hamming_primes.append(h + 1)
    sort(hamming_primes)

    // Step 3: Enumerate valid n via DFS over subsets of Hamming primes
    total := 0
    mod := 2^32

    function dfs(idx, product):
        // Multiply product by each Hamming number h with product * h <= L
        // and add product * h to total
        for h in hamming:
            if product * h > L: break
            total := (total + product * h) mod mod

        // Extend product by including more Hamming primes
        for i := idx to len(hamming_primes) - 1:
            p := hamming_primes[i]
            if product * p > L: break
            dfs(i + 1, product * p)

    dfs(0, 1)
    return total
```

## Complexity Analysis

- **Time:** The Hamming number generation is $O(\log^3 L)$. Primality testing for each Hamming number costs $O(\sqrt{h})$ per test, totaling $O(\log^3 L \cdot \sqrt{L})$ in the worst case (mitigated by Miller-Rabin). The DFS over subsets of $\sim$545 primes is pruned aggressively by the $\leq L$ bound; in practice, the tree is small and the total runtime is under 1 second.
- **Space:** $O(\log^3 L)$ for the Hamming number list, plus $O(k)$ recursion depth where $k$ is the number of Hamming primes used (at most $\sim$40 for $L = 10^{12}$).

## Answer

$$\boxed{939087315}$$
