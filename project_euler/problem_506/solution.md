# Problem 506: Clock Sequence

## Problem Statement

Consider the infinite periodic digit stream

$$
123432123432123432\cdots
$$

and read it from left to right. Define \(v_n\) to be the shortest consecutive block of unread digits whose digit sum is exactly \(n\). The beginning of the sequence is

$$
1,\ 2,\ 3,\ 4,\ 32,\ 123,\ 43,\ 2123,\ 432,\ 1234,\ 32123,\ldots
$$

Let

$$
S(N)=\sum_{n=1}^{N} v_n.
$$

It is given that \(S(11)=36120\). Find \(S(10^{14}) \bmod 123454321\).

## Mathematical Analysis

Let

$$
c = 123432
$$

be the fundamental 6-digit cycle. Its digit sum is

$$
1+2+3+4+3+2 = 15.
$$

This immediately suggests splitting \(n\) into quotient and remainder modulo \(15\).

### Lemma 1

The first fifteen terms are

$$
\begin{aligned}
&v_1=1,\quad v_2=2,\quad v_3=3,\quad v_4=4,\quad v_5=32,\\
&v_6=123,\quad v_7=43,\quad v_8=2123,\quad v_9=432,\quad v_{10}=1234,\\
&v_{11}=32123,\quad v_{12}=43212,\quad v_{13}=34321,\quad v_{14}=23432,\quad v_{15}=123432.
\end{aligned}
$$

**Proof.** Starting from the beginning of the stream, one greedily consumes the shortest unread block whose digit sum is the prescribed value. Since all digits are positive, the block is uniquely determined. A direct check yields the displayed list. $\square$

### Lemma 2

After the first fifteen terms have been read, the stream is again aligned with the start of the 6-digit cycle \(c\).

**Proof.** The fifteen blocks in Lemma 1 contain

$$
1+1+1+1+2+3+2+4+3+4+5+5+5+5+6 = 48
$$

digits in total, and \(48 = 8 \cdot 6\) is a multiple of the cycle length. Hence the next unread digit is again the first digit of \(c\). $\square$

### Theorem 1

The pattern of starting phases repeats with period \(15\). In particular, for every \(q \ge 0\) and every \(1 \le r \le 15\), the block \(v_{15q+r}\) starts in the same phase of the cycle as \(v_r\).

**Proof.** By Lemma 2 the stream is realigned after one full block of fifteen terms. Applying the same argument inductively proves the claim for every subsequent block of fifteen terms. $\square$

### Rotations and Residues

Fix one of the fifteen residue classes \(r \in \{1,\dots,15\}\). Let

- \(p_r\) be the starting phase of \(v_r\) inside the cycle \(c\),
- \(C_r\) be the 6-digit rotation of \(c\) beginning at phase \(p_r\),
- \(U_r\) be the terminal partial block needed to realize the residue \(r \bmod 15\),
- \(L_r\) be the number of digits of \(U_r\).

Because a full cycle contributes digit sum \(15\) and returns to the same phase, writing

$$
n = 15q + r \qquad (1 \le r \le 15)
$$

gives the decomposition

$$
v_n = \underbrace{C_r C_r \cdots C_r}_{q\text{ copies}}\, U_r.
$$

Therefore, modulo \(M = 123454321\),

$$
v_n \equiv A_r \, G(q) + B_r \pmod M,
$$

where

$$
A_r \equiv C_r \cdot 10^{L_r} \pmod M,\qquad
B_r \equiv U_r \pmod M,
$$

and

$$
G(q)=1+10^6+10^{12}+\cdots+10^{6(q-1)}.
$$

### Theorem 2

The sum over all complete 15-term blocks can be reduced to geometric sums.

**Proof.** By Theorem 1, the pair \((A_r,B_r)\) depends only on \(r\), not on the block index \(q\). Hence

$$
\sum_{q=0}^{Q-1} v_{15q+r}
\equiv
A_r \sum_{q=0}^{Q-1} G(q) + Q B_r \pmod M.
$$

Now \(G(q)\) itself is a geometric sum in the common ratio \(10^6\), so the outer sum is again expressible through geometric-series identities. This reduces the computation to \(O(\log Q)\) modular exponentiations. $\square$

## Algorithm

1. Simulate the first fifteen terms once and record, for each residue \(r\), the phase \(p_r\), the partial block \(U_r\), and its length \(L_r\).
2. Form the corresponding rotations \(C_r\) and coefficients \(A_r,B_r\).
3. Let \(N = 10^{14}\), \(Q = \lfloor N/15 \rfloor\), and \(R = N \bmod 15\).
4. Sum the contributions of the \(Q\) complete blocks using the geometric-series formula of Theorem 2.
5. Add the first \(R\) terms of the next block directly.

## Correctness

**Theorem 3.** The algorithm above computes \(S(10^{14}) \bmod 123454321\).

**Proof.** Lemma 1 gives the exact first-block data. Lemma 2 and Theorem 1 show that the phase information repeats every fifteen terms. The decomposition \(v_{15q+r} = C_r^q U_r\) is exact because each full cycle contributes digit sum \(15\) and restores the same phase. Theorem 2 then shows that summing over all complete blocks is equivalent to evaluating explicit geometric sums. The remaining \(R\) terms are handled directly, so every term \(v_n\) with \(1 \le n \le 10^{14}\) is counted exactly once. Therefore the returned residue is \(S(10^{14}) \bmod 123454321\). $\square$

## Complexity Analysis

The precomputation over one 15-term block is constant-time. The final evaluation uses only a constant number of modular exponentiations and geometric-sum evaluations, each in \(O(\log N)\) time.

- Time: \(O(\log N)\)
- Space: \(O(1)\)

## Answer

$$\boxed{18934502}$$
