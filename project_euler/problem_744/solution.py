"""
Problem 744: What? Where? When?

Game with 2n+1 envelopes (2n questions, 1 RED card). Expert answers with probability p. f(n,p) = prob game ends normally. Find f(10^11, 0.4999) to 10 d.p.
"""

# Problem 744: What? Where? When?
# The game ends normally if the RED card is never drawn in the first 2n-1 rounds and one side reaches n points. P(RED not drawn in first k rounds) = product of (2n+1-k)/(2n+1-j) terms. Combined with the...

print("Problem 744: What? Where? When?")
print("Solution requires specialized algorithm - see solution.md for analysis")
