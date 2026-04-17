# Problem 4: Largest Palindrome Product

## Problem Statement

Find the largest palindrome made from the product of two 3-digit numbers. That is, determine
$$M = \max\bigl\{xy : 100 \le x,y \le 999,\; xy \text{ is a palindrome in base 10}\bigr\}.$$

## Mathematical Development

### Definitions

**Definition 1.** A non-negative integer $n$ is a *palindrome in base 10* if its decimal representation $d_k d_{k-1} \cdots d_1 d_0$ satisfies $d_i = d_{k-i}$ for all $0 \le i \le k$.

**Definition 2.** A *6-digit palindrome* is an integer $P$ with $100{,}000 \le P \le 999{,}999$ that is a palindrome. Its decimal form is $\overline{abccba}$ where $a \in \{1,\dots,9\}$ and $b,c \in \{0,\dots,9\}$.

**Theorem 1** (Structure of 6-digit palindromes). *Every 6-digit palindrome $P = \overline{abccba}$ admits the representation*
$$P = 11(9091a + 910b + 100c).$$
*In particular, $11 \mid P$.*

*Proof.* By positional notation,
$$P = 10^5 a + 10^4 b + 10^3 c + 10^2 c + 10 b + a.$$
Collecting terms:
$$P = (10^5 + 1)a + (10^4 + 10)b + (10^3 + 10^2)c = 100001a + 10010b + 1100c.$$

We verify each coefficient is divisible by 11:
- $100001 = 11 \times 9091$, since $11 \times 9091 = 11 \times 9000 + 11 \times 91 = 99000 + 1001 = 100001$.
- $10010 = 11 \times 910$, since $11 \times 910 = 10010$.
- $1100 = 11 \times 100$.

Therefore $P = 11(9091a + 910b + 100c)$, and $11 \mid P$. $\square$

**Lemma 1** (Factor constraint). *If $P = xy$ where $P$ is a 6-digit palindrome and $x, y$ are positive integers, then $11 \mid x$ or $11 \mid y$.*

*Proof.* By Theorem 1, $11 \mid P$. Since $11$ is prime, Euclid's lemma (if $p$ is prime and $p \mid ab$, then $p \mid a$ or $p \mid b$) yields $11 \mid x$ or $11 \mid y$. $\square$

**Theorem 2** (Product range). *If $100 \le x, y \le 999$, then $10{,}000 \le xy \le 998{,}001$. Moreover, the largest palindromic product is necessarily a 6-digit palindrome.*

*Proof.* The bounds follow from $100 \times 100 = 10{,}000$ and $999 \times 999 = 998{,}001$. The largest 5-digit palindrome is $99{,}999$. We claim that a 6-digit palindromic product exists in the given range. Indeed, $143 \times 707 = 101{,}101 = \overline{101101}$, which is a 6-digit palindrome and satisfies $100 \le 143, 707 \le 999$. Since $101{,}101 > 99{,}999$, the maximum palindromic product must be a 6-digit palindrome. $\square$

**Theorem 3** (Optimality). *The largest palindrome that is a product of two 3-digit numbers is*
$$M = 906{,}609 = 913 \times 993.$$

*Proof.* By Theorem 2, any maximal palindromic product of two 3-digit numbers must be a 6-digit palindrome. Therefore Lemma 1 applies: if $P = xy$, then at least one of the two factors is divisible by 11.

By commutativity of multiplication, every candidate product can thus be written in the form $xy$ with

$$110 \le x \le y \le 999, \qquad 11 \mid x.$$

Hence it suffices to search the finite set

$$\mathcal{C} = \{(x,y) \in \mathbb{Z}^2 : 110 \le x \le y \le 999,\ 11 \mid x\}.$$

The algorithm below enumerates $\mathcal{C}$ in descending order of $x$ and, for each fixed $x$, in descending order of $y$. Its pruning rule is sound because for fixed $x$ the products $xy$ strictly decrease as $y$ decreases. Consequently:

- once $xy \le \text{best}$, no smaller value of $y$ can improve the answer;
- the first palindromic product found for a fixed $x$ is the largest palindromic product with that value of $x$.

A complete execution over $\mathcal{C}$ returns

$$\text{best} = 906{,}609 = 913 \times 993.$$

We verify that $906{,}609$ is a palindrome and that $913 = 11 \times 83$, so the candidate indeed lies in the restricted search space. Since the search is exhaustive over all relevant pairs, no larger palindromic product exists. $\square$

## Algorithm

We search the factor pairs in descending order while exploiting the divisibility-by-11 constraint for six-digit palindromes. The outer loop enumerates 3-digit multiples of 11 from largest to smallest, the inner loop scans the matching partner in descending order, and each product is tested by reversing its decimal digits. Two early exits avoid useless work: once a product is no larger than the current best, later partners for the same outer factor cannot improve it, and once a palindrome is found for a fixed outer factor, smaller partners only decrease the product.

**Theorem 4** (Algorithm correctness). *`LargestPalindromeProduct()` returns the largest palindrome of the form $xy$ with $100 \le x, y \le 999$.*

*Proof.* By Theorem 2 and Lemma 1, every optimal pair may be reordered so that it belongs to

$$\mathcal{C} = \{(x,y) : 110 \le x \le y \le 999,\ 11 \mid x\}.$$

The outer loop enumerates every admissible value of $x$ in $\mathcal{C}$ exactly once, and for each such $x$ the inner loop enumerates admissible $y$ values from $999$ down to $x$. Thus every pair in $\mathcal{C}$ is considered unless a justified early exit occurs.

There are two early exits:

1. If `P <= best`, then for every later inner-loop value $y' < y$ we have $xy' < xy \le \text{best}$, so no skipped pair can improve the answer.
2. If `P` is palindromic, the algorithm stores `best <- P` and breaks. This is valid because any later inner-loop value satisfies $y' < y$, hence $xy' < xy = P$, so no later pair with the same outer-loop value $x$ can produce a larger palindrome.

Therefore the algorithm never discards a pair that could beat the final answer, and it eventually records the maximum palindromic product. $\square$

## Pseudocode

```text
function isPalindrome(value):
    digits <- decimal string of value
    return digits = reverse(digits)

function largestPalindromeProduct():
    best <- 0
    for x <- 990 downto 110 step 11:
        for y <- 999 downto x step -1:
            product <- x * y
            if product <= best:
                break
            if isPalindrome(product):
                best <- product
                break
    return best
```

## Complexity Analysis

**Theorem 5** (Time complexity). *The algorithm runs in $O(N^2/11)$ time in the worst case, where $N = 900$ is the count of 3-digit numbers.*

*Proof.* The outer loop visits the 3-digit multiples of 11

$$110, 121, 132, \ldots, 990,$$

which form an arithmetic progression with

$$\frac{990 - 110}{11} + 1 = 81$$

terms. For each outer-loop value, the inner loop performs at most $900$ iterations in the worst case. Each iteration carries out $O(1)$ work: one multiplication, one comparison, and at most one palindrome test on a decimal string of length at most $6$. Hence the total work is bounded by

$$81 \times 900 = 72{,}900 = O(N^2/11),$$

where $N = 900$ is the number of 3-digit integers. Early termination can only decrease this bound. $\square$

**Space complexity.** $O(1)$. Only a constant number of integer variables and a string of length $\le 6$ are used.

## Answer

$$\boxed{906609}$$
