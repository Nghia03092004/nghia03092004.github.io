# Problem 836: A Bold Proposition (April Fools' 2023)
# The answer is the concatenation of the first letters of each bolded word:
# affine plane, radically integral local field,
# open oriented line section, jacobian,
# orthogonal kernel embedding
# => a p r i l f o o l s j o k e => aprilfoolsjoke

bold_words = [
    "affine", "plane",
    "radically", "integral", "local", "field",
    "open", "oriented", "line", "section",
    "jacobian",
    "orthogonal", "kernel", "embedding"
]

answer = "".join(w[0] for w in bold_words)
print(answer)
