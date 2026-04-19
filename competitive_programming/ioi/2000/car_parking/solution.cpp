// IOI 2000 - Car Parking
// Rearrange cars from initial to target configuration using one empty space.
// Each move drives a car into the empty space.
// Uses permutation cycle decomposition to count minimum moves.
// Cycle containing empty: (len - 1) moves. Other cycles of len >= 2: (len + 1) moves.
// Complexity: O(N) time, O(N) space.

#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;

    // initial[i] = car in space i (0 = empty)
    // target[i] = car that should be in space i
    vector<int> initial(N + 1), target(N + 1);
    int emptyPos = -1;

    for (int i = 1; i <= N; i++) {
        cin >> initial[i];
        if (initial[i] == 0) emptyPos = i;
    }
    for (int i = 1; i <= N; i++) {
        cin >> target[i];
    }

    // Build permutation: where should the item at position i go?
    // targetPos[car] = position where car belongs in target
    unordered_map<int, int> targetPos;
    int emptyTarget = -1;
    for (int i = 1; i <= N; i++) {
        if (target[i] == 0)
            emptyTarget = i;
        else
            targetPos[target[i]] = i;
    }

    // perm[i] = j means the item at position i should go to position j
    vector<int> perm(N + 1);
    for (int i = 1; i <= N; i++) {
        if (initial[i] == 0)
            perm[i] = emptyTarget;
        else
            perm[i] = targetPos[initial[i]];
    }

    // Find cycles and compute total moves
    vector<bool> visited(N + 1, false);
    int totalMoves = 0;

    for (int i = 1; i <= N; i++) {
        if (visited[i]) continue;

        int len = 0;
        bool containsEmpty = false;
        int cur = i;
        while (!visited[cur]) {
            visited[cur] = true;
            if (cur == emptyPos) containsEmpty = true;
            len++;
            cur = perm[cur];
        }

        if (len == 1) continue; // fixed point, no moves needed

        if (containsEmpty)
            totalMoves += len - 1;
        else
            totalMoves += len + 1;
    }

    cout << totalMoves << "\n";

    return 0;
}
