"""
Problem 480: The Last Question

Find W(P(legionary) + P(calorimeters) - P(annihilate) + P(orchestrated) - P(fluttering))
where words are formed from letters of "thereisasyetinsufficientdataforameaningfulanswer"
with at most 15 letters, listed alphabetically.
"""
from math import factorial
from collections import Counter

PHRASE = "thereisasyetinsufficientdataforameaningfulanswer"
MAX_LEN = 15

def get_letter_counts():
    freq = Counter(PHRASE)
    letters = sorted(freq.keys())
    counts = [freq[c] for c in letters]
    return letters, counts

LETTERS, MAX_COUNTS = get_letter_counts()
K = len(LETTERS)

def count_words_of_length(avail, length):
    """Count distinct words of given length from available letter counts."""
    if length == 0:
        return 1
    # DP: dp[j] = number of distinct words of length j using letters considered so far
    dp = [0] * (length + 1)
    dp[0] = 1

    for i in range(len(avail)):
        ndp = [0] * (length + 1)
        for j in range(length + 1):
            if dp[j] == 0:
                continue
            for s in range(min(avail[i], length - j) + 1):
                # C(j+s, s) positions for letter i
                comb = factorial(j + s) // (factorial(j) * factorial(s))
                ndp[j + s] += dp[j] * comb
        dp = ndp
    return dp[length]

def word_position(word, avail_orig):
    """Find 1-indexed position of word in alphabetical listing."""
    avail = list(avail_orig)
    pos = 0

    # Count all words of length 1 to len(word)-1
    for length in range(1, len(word)):
        pos += count_words_of_length(avail, length)

    # Count words of same length that come before
    wlen = len(word)
    cur = list(avail)
    for i, ch in enumerate(word):
        # Count words with smaller letter at position i
        for li in range(K):
            if LETTERS[li] >= ch:
                break
            if cur[li] > 0:
                cur[li] -= 1
                pos += count_words_of_length(cur, wlen - i - 1)
                cur[li] += 1
        # Use current letter
        idx = LETTERS.index(ch)
        cur[idx] -= 1

    return pos + 1  # 1-indexed

def find_word_at_position(target, avail_orig):
    """Find the word at the given 1-indexed position."""
    avail = list(avail_orig)

    # Determine word length
    cumulative = 0
    word_len = 0
    for length in range(1, MAX_LEN + 1):
        cnt = count_words_of_length(avail, length)
        if cumulative + cnt >= target:
            word_len = length
            target -= cumulative
            break
        cumulative += cnt

    # Find the word
    result = []
    cur = list(avail)
    for i in range(word_len):
        for li in range(K):
            if cur[li] > 0:
                cur[li] -= 1
                cnt = count_words_of_length(cur, word_len - i - 1)
                if cnt >= target:
                    result.append(LETTERS[li])
                    break
                target -= cnt
                cur[li] += 1

    return ''.join(result)

def main():
    p_legionary = word_position("legionary", MAX_COUNTS)
    p_calorimeters = word_position("calorimeters", MAX_COUNTS)
    p_annihilate = word_position("annihilate", MAX_COUNTS)
    p_orchestrated = word_position("orchestrated", MAX_COUNTS)
    p_fluttering = word_position("fluttering", MAX_COUNTS)

    print(f"P(legionary) = {p_legionary}")
    print(f"P(calorimeters) = {p_calorimeters}")
    print(f"P(annihilate) = {p_annihilate}")
    print(f"P(orchestrated) = {p_orchestrated}")
    print(f"P(fluttering) = {p_fluttering}")

    target = p_legionary + p_calorimeters - p_annihilate + p_orchestrated - p_fluttering
    print(f"Target position = {target}")

    answer = find_word_at_position(target, MAX_COUNTS)
    print(f"Answer: {answer}")

if __name__ == "__main__":
    main()
