# Problem 822: Square the Smallest

## Problem Statement

Start with the list

$$
[2,3,4,\dots,10000].
$$

At each step, replace the smallest element by its square. If several elements are tied for the minimum, choosing any one of them gives the same multiset after re-sorting. After \(10^{16}\) steps, let \(S(10000,10^{16})\) be the sum of the final list. Find

$$
S(10000,10^{16}) \bmod 1234567891.
$$

## Mathematical Analysis

Let \(K=n-1\), so here \(K=9999\). At any moment write the sorted list as

$$
a_1 \le a_2 \le \cdots \le a_K.
$$

### Lemma 1

If several elements are equal to the minimum, then squaring any one of them produces the same multiset after re-sorting.

**Proof.** Equal elements have the same square. Replacing one copy of the minimum value \(m\) by \(m^2\) therefore removes one copy of \(m\) and inserts one copy of \(m^2\), independently of which equal minimum was chosen. $\square$

### Lemma 2

Suppose the current sorted list satisfies

$$
a_1^2 \ge a_K.
$$

Then during the next \(K\) operations the elements are squared in the order

$$
a_1,\ a_2,\ \dots,\ a_K,
$$

and after these \(K\) steps the multiset becomes

$$
\{a_1^2,a_2^2,\dots,a_K^2\}.
$$

**Proof.** After squaring \(a_1\), the new value \(a_1^2\) is at least \(a_K\), so it is no longer smaller than any unsquared element. Hence the next minimum is \(a_2\). Because \(a_2 \ge a_1\), we also have

$$
a_2^2 \ge a_1^2 \ge a_K,
$$

so after squaring \(a_2\) it likewise moves to the top end of the sorted list. Repeating the same argument inductively shows that the next \(K\) operations square \(a_1,a_2,\dots,a_K\) exactly once each. $\square$

### Theorem 1

Once the condition \(a_1^2 \ge a_K\) is first reached, the process enters a periodic regime of block length \(K\): every full block of \(K\) operations squares each current element exactly once.

**Proof.** Lemma 2 applies to the current sorted list, and after one full block the new sorted list is simply the sorted multiset

$$
\{a_1^2,\dots,a_K^2\}.
$$

Its minimum squared is

$$
(a_1^2)^2 = a_1^4 \ge a_K^2,
$$

so the same condition holds again for the next block. Induction proves the claim for all subsequent blocks. $\square$

### Logarithmic Representation

If an original value \(b\) has been squared \(e\) times, its current value is

$$
b^{2^e}.
$$

To compare numbers without constructing huge integers, it is enough to compare their logarithms:

$$
\log\!\bigl(b^{2^e}\bigr) = 2^e \log b.
$$

Thus a min-heap on the weights \(2^e \log b\) exactly reproduces the greedy rule.

### Theorem 2

Assume the periodic regime begins after \(T\) transient steps, leaving the sorted current values

$$
b_1 \le b_2 \le \cdots \le b_K.
$$

Write the remaining number of steps as

$$
10^{16}-T = qK + r,\qquad 0 \le r < K.
$$

Then the final multiset is

$$
\{b_1^{2^{q+1}},\dots,b_r^{2^{q+1}},b_{r+1}^{2^q},\dots,b_K^{2^q}\}.
$$

**Proof.** By Theorem 1, each complete block of length \(K\) squares every element exactly once, so \(q\) complete blocks add \(q\) squarings to every current element. The remaining \(r\) operations square the first \(r\) elements of the current sorted list once more. $\square$

### Modular Evaluation

The modulus

$$
P=1234567891
$$

is prime. Therefore, for every base \(b < P\),

$$
b^{2^t} \equiv b^{\,2^t \bmod (P-1)} \pmod P
$$

by Fermat's little theorem. Hence the only remaining work is to compute \(2^q \bmod (P-1)\) and \(2^{q+1} \bmod (P-1)\) by fast exponentiation.

## Editorial
We initialize a min-heap with the logarithmic weights \(\log 2,\log 3,\dots,\log 10000\). We then simulate the greedy process until the critical condition \(a_1^2 \ge a_K\) is met, tracking the number \(T\) of transient steps. Finally, sort the current values \(b_1,\dots,b_K\).

## Pseudocode

```text
Initialize a min-heap with the logarithmic weights \(\log 2,\log 3,\dots,\log 10000\)
Simulate the greedy process until the critical condition \(a_1^2 \ge a_K\) is met, tracking the number \(T\) of transient steps
Sort the current values \(b_1,\dots,b_K\)
Decompose the remaining number of steps as \(qK+r\)
Use Theorem 2 to determine the final exponent of each value
Evaluate the final sum modulo \(1234567891\) using Fermat reduction of the exponents
```

## Correctness

**Theorem 3.** The algorithm above computes \(S(10000,10^{16}) \bmod 1234567891\).

**Proof.** Lemma 1 shows that tie-breaking does not matter. The heap of logarithmic weights orders the current values correctly because the logarithm is strictly increasing. Lemma 2 proves the block behavior once the critical condition is reached, and Theorem 1 upgrades that to a permanent periodic regime. Theorem 2 then gives the exact number of remaining squarings applied to each current element. Finally, modular exponentiation with Fermat reduction evaluates these exact final values modulo the prime \(1234567891\). Therefore the algorithm returns precisely \(S(10000,10^{16}) \bmod 1234567891\). $\square$

## Complexity Analysis

Let \(T\) be the length of the transient. The heap simulation costs \(O(T \log K)\). After that, all remaining \(10^{16}-T\) steps are handled in closed form.

- Time: \(O(T \log K + K \log K)\)
- Space: \(O(K)\)

In practice, \(T\) is tiny compared to \(10^{16}\), so the method is easily feasible.

## Answer

$$\boxed{950591530}$$
