"""
Project Euler 679: Freefarea

Count words of length n over {A, E, F, R} containing each of FREE, FARE, AREA, REEF exactly once.

We use a DFA-based DP approach. Track:
- Which of the 4 keywords have been completed (bitmask 0-15)
- The current suffix state for partial matches of each keyword
- The count of each keyword that has been completed

Since we need exactly one occurrence of each, we track a bitmask of which keywords
have appeared, and reject any state where a keyword appears a second time.

Keywords: FREE, FARE, AREA, REEF
Alphabet: {A, E, F, R}

We use Aho-Corasick automaton to track all four patterns simultaneously.
"""

from collections import deque

def solve():
    keywords = ["FREE", "FARE", "AREA", "REEF"]
    alphabet = ['A', 'E', 'F', 'R']

    # Build Aho-Corasick automaton
    # Each node: dict of children, failure link, output (set of keyword indices)
    goto = [{}]  # goto function
    fail = [0]   # failure function
    output = [0] # bitmask of keywords ending at this node

    # Build trie
    for ki, kw in enumerate(keywords):
        cur = 0
        for ch in kw:
            if ch not in goto[cur]:
                goto[cur][ch] = len(goto)
                goto.append({})
                fail.append(0)
                output.append(0)
            cur = goto[cur][ch]
        output[cur] |= (1 << ki)

    # Build failure links using BFS
    queue = deque()
    for ch in alphabet:
        if ch in goto[0]:
            s = goto[0][ch]
            fail[s] = 0
            queue.append(s)
        else:
            goto[0][ch] = 0  # loop back to root

    while queue:
        r = queue.popleft()
        for ch in alphabet:
            if ch in goto[r]:
                s = goto[r][ch]
                queue.append(s)
                state = fail[r]
                while state != 0 and ch not in goto[state]:
                    state = fail[state]
                fail[s] = goto[state].get(ch, 0)
                if fail[s] == s:
                    fail[s] = 0
                output[s] |= output[fail[s]]
            else:
                # Create goto for missing transitions
                state = fail[r]
                while state != 0 and ch not in goto[state]:
                    state = fail[state]
                goto[r][ch] = goto[state].get(ch, 0)

    num_nodes = len(goto)

    # DP state: (aho_node, keyword_bitmask)
    # keyword_bitmask tracks which keywords have been seen exactly once
    # If a keyword is seen twice, that state is invalid (discard)

    # For "exactly once", we need to handle overlapping matches carefully.
    # When we enter a state that outputs keyword k:
    # - If k is already in the bitmask, this path is invalid (more than once)
    # - Otherwise, add k to bitmask

    # But we also need to track if we're about to complete a keyword for the second time.
    # The Aho-Corasick output tells us which keywords end at each position.
    # However, for "exactly once" we need: each keyword appears as a substring exactly once.

    # State: (node, seen_mask) where seen_mask is a 4-bit mask
    # We need to count strings where at end, seen_mask = 15 (all 4 keywords seen)
    # and no keyword was seen more than once.

    # To track "more than once": if at any step we would add a keyword already in seen_mask,
    # that string is invalid. We mark it with a special "dead" state or simply don't count it.

    # But there's a subtlety: a keyword might be detected at multiple positions.
    # We need to count strings where each keyword occurs as a substring exactly once.
    # The Aho-Corasick approach detects every occurrence, so if a keyword appears twice,
    # we'll see it twice in the output, and we can discard that path.

    # DP: dp[node][mask] = number of strings
    # Transition: for each character, compute new_node, check output of new_node,
    # if any keyword in output is already in mask -> dead state
    # otherwise, mask |= output

    N = 30

    # Initialize: dp[0][0] = 1
    dp = {}
    dp[(0, 0)] = 1

    for step in range(N):
        new_dp = {}
        for (node, mask), count in dp.items():
            for ch in alphabet:
                new_node = goto[node][ch]
                new_out = output[new_node]

                # Check if any keyword in new_out is already in mask
                if new_out & mask:
                    continue  # This would mean a keyword appears more than once

                new_mask = mask | new_out
                key = (new_node, new_mask)
                new_dp[key] = new_dp.get(key, 0) + count

        dp = new_dp

    # Sum all states where mask == 15 (all 4 keywords seen)
    result = sum(count for (node, mask), count in dp.items() if mask == 15)
    print(result)

solve()
