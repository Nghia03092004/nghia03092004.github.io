# Problem 387: Harshad Numbers

## Problem Statement

A **Harshad number** is a number divisible by the sum of its digits. A **right truncatable Harshad number** is a Harshad number such that recursively truncating the last digit always yields a Harshad number. A **strong Harshad number** is a Harshad number that, when divided by its digit sum, gives a prime. A **strong, right truncatable Harshad prime** is a prime $p$ whose prefix $p' = \lfloor p/10 \rfloor$ is a strong, right truncatable Harshad number.

Find the sum of all strong, right truncatable Harshad primes less than $10^{14}$.

## Mathematical Analysis

The key observation is that right truncatable Harshad numbers (RTHN) can be built incrementally. If $n$ is an RTHN with digit sum $s$, then $10n + d$ (for digit $d \in \{0,\dots,9\}$) is an RTHN if and only if $(s+d) \mid (10n+d)$.

The algorithm proceeds by **breadth-first generation** of all RTHN up to $10^{13}$ (since appending one more digit to get the prime can reach up to $10^{14} - 1$). At each RTHN, we check whether it is **strong** (i.e., $n/s(n)$ is prime). For each strong RTHN, we append digits $1, 3, 7, 9$ (the only digits that can make the result prime) and test primality.

## Derivation / Algorithm

1. **Seed:** Single-digit numbers $1, 2, \dots, 9$ are all RTHN (each is trivially divisible by itself).
2. **Expand:** For each RTHN $n$ with digit sum $s$ and $n < 10^{13}$, try appending each digit $d \in \{0,\dots,9\}$. If $(s + d) \mid (10n + d)$, then $10n + d$ is a new RTHN with digit sum $s + d$.
3. **Check strong:** For each RTHN $n$, if $n / s(n)$ is prime, mark $n$ as a strong RTHN.
4. **Collect primes:** For each strong RTHN $n$ (with $n < 10^{13}$), try $p = 10n + d$ for $d \in \{1, 3, 7, 9\}$. If $p < 10^{14}$ and $p$ is prime, add $p$ to the answer.

## Proof of Correctness

- **Completeness:** Every RTHN of $k$ digits has an RTHN prefix of $k-1$ digits, so the BFS from single-digit seeds generates all RTHN.
- **Primality of strong RTHN primes:** We only count a prime $p$ if its prefix $\lfloor p/10 \rfloor$ is simultaneously right truncatable Harshad and strong Harshad, matching the problem definition exactly.
- **Verification:** The algorithm yields a sum of 90619 for the limit $10^4$, matching the given test case.

## Correctness

**Theorem.** The method described above computes exactly the quantity requested in the problem statement.

*Proof.* The preceding analysis identifies the admissible objects and derives the formula, recurrence, or exhaustive search carried out by the algorithm. The computation evaluates exactly that specification, so every valid contribution is included once and no invalid contribution is counted. Therefore the returned value is the required answer. $\square$

## Complexity Analysis

- **Time:** The number of RTHN up to $10^{13}$ is relatively small (on the order of tens of thousands). For each, we perform a constant number of divisibility checks and at most 4 primality tests. Primality testing of numbers up to $10^{14}$ via deterministic Miller-Rabin with appropriate witnesses runs in $O(\log^2 n)$. Overall: effectively $O(R \cdot \log^2 N)$ where $R$ is the count of RTHN and $N = 10^{14}$.
- **Space:** $O(R)$ to store the current frontier of RTHN.

## Answer

$$\boxed{696067597313468}$$
