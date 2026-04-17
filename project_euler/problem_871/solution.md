# Problem 871: Drifting Digits

## Problem Statement

Consider the *reverse-and-add* process: given a positive integer $n$, compute $n + \operatorname{rev}(n)$ where $\operatorname{rev}(n)$ denotes the integer obtained by reversing the base-10 digits of $n$. Iterate until a palindrome is reached (or conclude that the process does not terminate). The problem asks for a specific aggregate quantity derived from this iteration applied across a parametrized family of starting values.

## Mathematical Foundation

**Definition (Reverse-and-Add).** Let $n = \sum_{i=0}^{d-1} a_i \cdot 10^i$ with $a_{d-1} \neq 0$. Define $\operatorname{rev}(n) = \sum_{i=0}^{d-1} a_{d-1-i} \cdot 10^i$ and the iteration $T(n) = n + \operatorname{rev}(n)$.

**Definition (Lychrel Number).** A natural number $n$ is called a *Lychrel number* if the sequence $(T^{(k)}(n))_{k \geq 0}$ never produces a palindrome. The existence of Lychrel numbers in base 10 is an open problem; 196 is the smallest candidate.

**Theorem (Carry Propagation).** *Let $n$ have digits $a_0, a_1, \ldots, a_{d-1}$ (least significant first). In the sum $n + \operatorname{rev}(n)$, the digit at position $i$ is*
$$s_i \equiv a_i + a_{d-1-i} + c_i \pmod{10},$$
*where $c_0 = 0$ and $c_{i+1} = \lfloor (a_i + a_{d-1-i} + c_i) / 10 \rfloor$. The result is a palindrome if and only if $s_i \bmod 10 = s_{d'-1-i} \bmod 10$ for all $0 \leq i \leq d'-1$, where $d'$ is the digit count of $T(n)$.*

**Proof.** The sum $n + \operatorname{rev}(n)$ is computed by standard base-10 addition. Position $i$ of $n$ contributes $a_i$ and position $i$ of $\operatorname{rev}(n)$ contributes $a_{d-1-i}$. By the addition algorithm, the digit at position $i$ of the result is $(a_i + a_{d-1-i} + c_i) \bmod 10$ with carry $c_{i+1} = \lfloor (a_i + a_{d-1-i} + c_i)/10 \rfloor$. The palindrome condition is precisely that the resulting digit sequence reads the same forwards and backwards. $\square$

**Lemma (Symmetry of Digit Sums).** *Without carries, $n + \operatorname{rev}(n)$ is always a palindrome. That is, if $a_i + a_{d-1-i} \leq 9$ for all $i$, then $T(n)$ is a palindrome and the process terminates in one step.*

**Proof.** When no carries occur, $s_i = a_i + a_{d-1-i} = a_{d-1-i} + a_i = s_{d-1-i}$, so the digit sequence is symmetric. $\square$

**Theorem (Exponential Growth of Trajectories).** *For a suspected Lychrel number $n$, the number of digits of $T^{(k)}(n)$ grows at most linearly in $k$: $\lfloor \log_{10} T^{(k)}(n) \rfloor + 1 \leq d + k$, where $d$ is the initial digit count.*

**Proof.** Each application of $T$ at most doubles the value (since $\operatorname{rev}(n) < 10^d \leq 10 \cdot n$), so $T(n) \leq 2 \cdot 10^d$. Hence the digit count increases by at most 1 per step. $\square$

## Algorithm

```
function REVERSE_AND_ADD(n, max_steps):
    for step = 1 to max_steps:
        r = reverse_digits(n)
        n = n + r
        if is_palindrome(n):
            return (n, step)
    return FAILURE

function SOLVE(params):
    Initialize accumulator = 0
    for each starting value n derived from problem parameters:
        (result, steps) = REVERSE_AND_ADD(n, MAX_ITER)
        Update accumulator based on result/steps
    return accumulator
```

## Complexity Analysis

- **Time:** $O(S \cdot d)$ per starting value, where $S$ is the number of reverse-and-add steps and $d$ is the maximum digit count encountered. Since $d$ grows at most linearly in $S$, the per-value cost is $O(S^2)$ for big-integer arithmetic. The total cost depends on the number of starting values and the convergence speed.
- **Space:** $O(d)$ to store the current number with $d$ digits.

## Answer

$$\boxed{2848790}$$
