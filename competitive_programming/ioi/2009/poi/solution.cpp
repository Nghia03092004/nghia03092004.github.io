// IOI 2009 - POI
// Compute scores (harder tasks = more points), rank contestants, output P's info.
// O(NT + N log N) time.
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, T, P;
    cin >> N >> T >> P;
    P--; // convert to 0-indexed

    vector<vector<int>> solved(N, vector<int>(T));
    vector<int> solvers(T, 0);

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < T; j++) {
            cin >> solved[i][j];
            solvers[j] += solved[i][j];
        }
    }

    // Task j is worth (N - solvers[j]) points.
    vector<int> score(N, 0), taskCount(N, 0);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < T; j++) {
            if (solved[i][j]) {
                score[i] += N - solvers[j];
                taskCount[i]++;
            }
        }
    }

    // Sort: score desc, tasks solved desc, contestant number asc.
    vector<int> order(N);
    iota(order.begin(), order.end(), 0);
    sort(order.begin(), order.end(), [&](int a, int b) {
        if (score[a] != score[b]) return score[a] > score[b];
        if (taskCount[a] != taskCount[b]) return taskCount[a] > taskCount[b];
        return a < b;
    });

    int rank = -1;
    for (int i = 0; i < N; i++) {
        if (order[i] == P) {
            rank = i + 1;
            break;
        }
    }

    cout << score[P] << " " << rank << "\n";
    return 0;
}
