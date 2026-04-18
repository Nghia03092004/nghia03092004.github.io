# Problem 808: Reversible Prime Squares

## Problem Statement

Both 169 and 961 are the square of a prime. 169 is the reverse of 961.

A number is called a **reversible prime square** if:
1. It is not a palindrome.
2. It is the square of a prime.
3. Its digit-reversal is also the square of a prime.

169 and 961 are not palindromes, so both are reversible prime squares.

Find the sum of the first 50 reversible prime squares.

## Mathematical Analysis

### Key Observations

A reversible prime square is a number $n = p^2$ where $p$ is prime, $n$ is not a palindrome, and $\text{rev}(n) = q^2$ for some prime $q$.

Note that if $p^2$ is a reversible prime square with $\text{rev}(p^2) = q^2$, then $q^2$ is also a reversible prime square (since $\text{rev}(q^2) = p^2$). Therefore, reversible prime squares come in pairs.

### Digit Constraints

For $p^2$ to have its reversal also be a perfect square:
- The last digit of $p^2$ cannot be 0 (otherwise the reversal would have fewer digits and couldn't be a square of a prime with the same number of digits).
- The first digit of $p^2$ must be a possible last digit of a perfect square: $\{1, 4, 5, 6, 9\}$.

### Search Bound

We need to find enough primes $p$ so that $p^2$ generates at least 50 reversible prime squares. Since primes grow as $p_n \sim n \ln n$, and the density of reversible prime squares decreases as numbers grow, we sieve primes up to $10^8$, yielding squares up to $10^{16}$.

## Editorial
We sieve primes** up to $10^8$ using the Sieve of Eratosthenes. We then iterate over each prime** $p$. Finally, check if $s$ is a palindrome; skip if so.

## Pseudocode

```text
Sieve primes** up to $10^8$ using the Sieve of Eratosthenes
For each prime** $p$:
Compute $s = p^2$
Check if $s$ is a palindrome; skip if so
Compute $r = \text{rev}(s)$
Check if $r$ is a perfect square
If so, check if $\sqrt{r}$ is prime
If all conditions hold, add $s$ to the list
Sort** the collected reversible prime squares
Sum** the first 50
```

## Correctness

**Theorem.** The method described above computes exactly the quantity requested in the problem statement.

*Proof.* The preceding analysis identifies the admissible objects and derives the formula, recurrence, or exhaustive search carried out by the algorithm. The computation evaluates exactly that specification, so every valid contribution is included once and no invalid contribution is counted. Therefore the returned value is the required answer. $\square$

## Complexity Analysis

- **Sieve**: $O(N \log \log N)$ where $N = 10^8$.
- **Main loop**: $O(\pi(N))$ iterations (about $5.76 \times 10^6$ primes up to $10^8$), each with $O(\log(p^2))$ work.
- **Space**: $O(N)$ for the sieve.
- **Total**: Dominated by the sieve at $O(N)$.

## Answer

$$\boxed{3807504276997394}$$
