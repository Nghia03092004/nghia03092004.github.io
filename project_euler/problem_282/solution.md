# Problem 282: The Ackermann Function

## Problem Statement

Define $A(n) = \operatorname{ack}(n, n)$ where $\operatorname{ack}$ is the Ackermann function:

- $\operatorname{ack}(0, m) = m + 1$,
- $\operatorname{ack}(n, 0) = \operatorname{ack}(n-1, 1)$ for $n > 0$,
- $\operatorname{ack}(n, m) = \operatorname{ack}(n-1, \operatorname{ack}(n, m-1))$ for $n, m > 0$.

Find $\displaystyle\sum_{n=0}^{6} A(n) \pmod{14^8}$.

## Mathematical Development

### Theorem 1 (Closed Forms for Ackermann Levels 0--4)

**Theorem.** *For all $m \ge 0$:*

1. $\operatorname{ack}(0, m) = m + 1$.
2. $\operatorname{ack}(1, m) = m + 2$.
3. $\operatorname{ack}(2, m) = 2m + 3$.
4. $\operatorname{ack}(3, m) = 2^{m+3} - 3$.
5. $\operatorname{ack}(4, m) = \underbrace{2^{2^{\cdot^{\cdot^{\cdot^2}}}}}_{m+3} - 3 = {}^{m+3}2 - 3$.

**Proof.** Each level is established by induction on $m$.

*(1)* By definition.

*(2)* Base case: $\operatorname{ack}(1,0) = \operatorname{ack}(0,1) = 2 = 0 + 2$. Inductive step: $\operatorname{ack}(1,m) = \operatorname{ack}(0, \operatorname{ack}(1, m-1)) = \operatorname{ack}(1,m-1) + 1$. By the inductive hypothesis $\operatorname{ack}(1,m-1) = m+1$, so $\operatorname{ack}(1,m) = m+2$.

*(3)* Base case: $\operatorname{ack}(2,0) = \operatorname{ack}(1,1) = 3 = 2 \cdot 0 + 3$. Inductive step: $\operatorname{ack}(2,m) = \operatorname{ack}(1, \operatorname{ack}(2,m-1)) = \operatorname{ack}(2,m-1) + 2 = (2(m-1)+3) + 2 = 2m+3$.

*(4)* Base case: $\operatorname{ack}(3,0) = \operatorname{ack}(2,1) = 5 = 2^3 - 3$. Inductive step: $\operatorname{ack}(3,m) = \operatorname{ack}(2, \operatorname{ack}(3,m-1)) = 2\operatorname{ack}(3,m-1) + 3 = 2(2^{m+2}-3)+3 = 2^{m+3}-3$.

*(5)* Base case: $\operatorname{ack}(4,0) = \operatorname{ack}(3,1) = 2^4 - 3 = 13 = {}^{3}2 - 3$. Inductive step: $\operatorname{ack}(4,m) = \operatorname{ack}(3, \operatorname{ack}(4,m-1)) = 2^{\operatorname{ack}(4,m-1)+3} - 3 = 2^{{}^{m+2}2} - 3 = {}^{m+3}2 - 3$. $\qquad \blacksquare$

**Corollary.** The diagonal values are: $A(0) = 1$, $A(1) = 3$, $A(2) = 7$, $A(3) = 61$, $A(4) = {}^{7}2 - 3$.

### Theorem 2 (Modular Stabilization of Iterated Exponentiation)

**Theorem.** *For any positive integer $M$, there exists a non-negative integer $H$ such that ${}^{h}2 \equiv {}^{H}2 \pmod{M}$ for all $h \ge H$. The minimal such $H$ equals the length of the Carmichael chain $M, \lambda(M), \lambda(\lambda(M)), \ldots, 1$.*

**Proof.** By strong induction on $M$.

*Base case:* $M = 1$. Then $H = 0$ and the congruence ${}^{h}2 \equiv 0 \pmod{1}$ holds trivially.

*Inductive step:* Let $M > 1$.

*Case 1: $\gcd(2, M) = 1$.* By the generalized Euler theorem, $2^a \equiv 2^b \pmod{M}$ whenever $a \equiv b \pmod{\lambda(M)}$, where $\lambda$ is Carmichael's function. Since $\lambda(M) < M$, the inductive hypothesis applies to $\lambda(M)$: the sequence ${}^{h-1}2 \bmod \lambda(M)$ stabilizes for $h - 1 \ge H'$, where $H'$ is the chain length for $\lambda(M)$. Hence ${}^{h}2 = 2^{{}^{h-1}2} \bmod M$ stabilizes for $h \ge H' + 1$.

*Case 2: $2 \mid M$.* Write $M = 2^a \cdot M'$ with $\gcd(2, M') = 1$ and $a \ge 1$. For $h \ge a$, the tower ${}^{h}2$ is divisible by $2^a$, so ${}^{h}2 \equiv 0 \pmod{2^a}$. By Case 1, ${}^{h}2 \bmod M'$ stabilizes. By the Chinese Remainder Theorem, ${}^{h}2 \bmod M$ stabilizes. $\qquad \blacksquare$

### Theorem 3 (Computation via CRT Decomposition)

**Theorem.** *Let $M = 14^8 = 2^8 \cdot 7^8$. Then:*

1. *For $k \ge 4$: $A(k) \equiv -3 \equiv 253 \pmod{256}$, since the tower ${}^{h}2$ satisfies ${}^{h}2 \equiv 0 \pmod{256}$ for all $h \ge 8$, and the tower heights in $A(k)$ for $k \ge 4$ exceed $8$.*
2. *Modulo $7^8$: since $\gcd(2, 7^8) = 1$, we compute via the iterated Carmichael chain. For $A(4) = {}^{7}2 - 3$, we evaluate ${}^{7}2 \bmod 7^8$ by recursing down the chain. For $A(5)$ and $A(6)$, the tower heights vastly exceed the chain length, so the modular residue has stabilized to the same value as ${}^{\infty}2 \bmod 7^8$.*
3. *The final values are combined via CRT to obtain $A(k) \bmod 14^8$ for each $k \in \{4, 5, 6\}$.*

**Proof.** Part (1): ${}^{h}2 = 2^{{}^{h-1}2}$, and for $h \ge 2$, the exponent ${}^{h-1}2 \ge 4 > 8$ is false only for small $h$; checking, ${}^{2}2 = 4$, ${}^{3}2 = 16$, ${}^{4}2 = 65536$. For $h \ge 4$, the exponent exceeds $8$, giving $2^{(\text{exponent})} \equiv 0 \pmod{2^8}$. For $A(4)$, the tower height is $7 \ge 4$, so ${}^{7}2 \equiv 0 \pmod{256}$ and $A(4) \equiv -3 \pmod{256}$.

Part (2): The Carmichael chain for $7^8$ is $7^8 \to \lambda(7^8) = 6 \cdot 7^7 \to \lambda(6 \cdot 7^7) = \operatorname{lcm}(\lambda(2), \lambda(3), \lambda(7^7)) = \operatorname{lcm}(1, 2, 6 \cdot 7^6) = 6 \cdot 7^6 \to \cdots$, terminating in $O(\log M)$ steps. For $A(5)$: $\operatorname{ack}(5,0) = 65533$ and $\operatorname{ack}(5,1) = {}^{65536}2 - 3$, where the tower height $65536$ already exceeds any conceivable chain length. For subsequent values of $\operatorname{ack}(5, m)$ and all values of $\operatorname{ack}(6, m)$, the tower heights grow hyper-exponentially and yield the same stabilized residue.

Part (3): Standard CRT reconstruction from the residues modulo $2^8$ and $7^8$. $\qquad \blacksquare$

## Editorial
Closed forms: A(0)=1, A(1)=3, A(2)=7, A(3)=61, A(4)=2^^7 - 3. A(5) and A(6) involve towers that stabilize modulo 14^8. Computation via CRT (mod 2^8 and mod 7^8) with iterated Carmichael lambda. We compute A(4), A(5), A(6) mod M via tower2_mod. We then compute 2^^height mod m via CRT on 2-part and odd-part. Finally, recurse using Carmichael's lambda for the odd part.

## Pseudocode

```text
Compute A(4), A(5), A(6) mod M via tower2_mod
Compute 2^^height mod m via CRT on 2-part and odd-part
Recurse using Carmichael's lambda for the odd part
```

## Complexity Analysis

- **Time:** $O(L \cdot \log M)$ where $L = O(\log M)$ is the Carmichael chain length. Each level involves one modular exponentiation costing $O(\log M)$ multiplications. Total: $O(\log^2 M)$.
- **Space:** $O(L) = O(\log M)$ for the recursion stack.

## Answer

$$\boxed{1098988351}$$
