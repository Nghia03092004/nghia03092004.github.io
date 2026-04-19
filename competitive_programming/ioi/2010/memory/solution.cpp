// IOI 2010 - Memory (Interactive)
// Two-phase strategy: explore cards, match known pairs immediately.
// O(N) flips.
#include <bits/stdc++.h>
using namespace std;

// Grader-provided functions.
int faceup(int pos);
void matched(int pos1, int pos2);

void play(int N) {
    // 2N cards at positions 1..2N.
    map<int, int> known;   // card value -> known position
    set<int> remaining;    // unmatched positions
    for (int i = 1; i <= 2 * N; i++) remaining.insert(i);

    while (!remaining.empty()) {
        // Pick the first remaining card.
        int pos1 = *remaining.begin();
        int val1 = faceup(pos1);

        if (known.count(val1) && remaining.count(known[val1])) {
            // We already know where the match is.
            int pos2 = known[val1];
            faceup(pos2);
            matched(pos1, pos2);
            remaining.erase(pos1);
            remaining.erase(pos2);
            known.erase(val1);
        } else {
            known[val1] = pos1;

            // Pick another unseen card.
            auto it2 = remaining.begin();
            if (*it2 == pos1) ++it2;
            if (it2 == remaining.end()) break; // shouldn't happen
            int pos2 = *it2;
            int val2 = faceup(pos2);

            if (val1 == val2) {
                matched(pos1, pos2);
                remaining.erase(pos1);
                remaining.erase(pos2);
                known.erase(val1);
            } else {
                known[val2] = pos2;
            }
        }
    }
}

// Stub main for standalone compilation.
int main() {
    int N;
    cin >> N;
    play(N);
    return 0;
}

int faceup(int /*pos*/) { return 0; }
void matched(int /*pos1*/, int /*pos2*/) {}
