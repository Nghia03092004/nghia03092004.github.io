// IOI 2001 - Ioiwari
// Two-player game on a triangular board with 10 cells.
// Players move stones to adjacent empty cells, scoring based on neighboring values.
// Uses minimax with memoization. State encoded as 10-digit base-11 number.
// Complexity: bounded by number of reachable states (much less than 10!).

#include <bits/stdc++.h>
using namespace std;

// Triangular board adjacency (cells 0-9):
//     0
//    1 2
//   3 4 5
//  6 7 8 9
const vector<int> adj[10] = {
    {1, 2},              // 0
    {0, 2, 3, 4},        // 1
    {0, 1, 4, 5},        // 2
    {1, 4, 6, 7},        // 3
    {1, 2, 3, 5, 7, 8},  // 4
    {2, 4, 8, 9},        // 5
    {3, 7},              // 6
    {3, 4, 6, 8},        // 7
    {4, 5, 7, 9},        // 8
    {5, 8}               // 9
};

unordered_map<long long, int> memo;

long long encodeState(int board[10]) {
    long long h = 0;
    for (int i = 0; i < 10; i++) {
        h = h * 11 + board[i];  // values 0-10
    }
    return h;
}

// Returns net advantage for the current player (my score - opponent's score)
int minimax(int board[10]) {
    long long key = encodeState(board);
    auto it = memo.find(key);
    if (it != memo.end()) return it->second;

    int bestVal = INT_MIN;
    bool hasMoves = false;

    for (int from = 0; from < 10; from++) {
        if (board[from] == 0) continue;
        for (int to : adj[from]) {
            if (board[to] != 0) continue;

            // Score = sum of values of stones adjacent to destination
            // (excluding the moved stone itself)
            int score = 0;
            for (int nb : adj[to]) {
                if (nb != from && board[nb] != 0) {
                    score += board[nb];
                }
            }

            // Make move
            board[to] = board[from];
            board[from] = 0;

            int val = score - minimax(board);
            bestVal = max(bestVal, val);
            hasMoves = true;

            // Undo move
            board[from] = board[to];
            board[to] = 0;
        }
    }

    if (!hasMoves) bestVal = 0;

    memo[key] = bestVal;
    return bestVal;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int board[10];
    for (int i = 0; i < 10; i++) cin >> board[i];

    // Find optimal first move and game result
    int bestVal = INT_MIN;
    int bestFrom = -1, bestTo = -1;

    for (int from = 0; from < 10; from++) {
        if (board[from] == 0) continue;
        for (int to : adj[from]) {
            if (board[to] != 0) continue;

            int score = 0;
            for (int nb : adj[to]) {
                if (nb != from && board[nb] != 0) {
                    score += board[nb];
                }
            }

            board[to] = board[from];
            board[from] = 0;

            int val = score - minimax(board);
            if (val > bestVal) {
                bestVal = val;
                bestFrom = from;
                bestTo = to;
            }

            board[from] = board[to];
            board[to] = 0;
        }
    }

    // Output result
    if (bestVal > 0)
        cout << "Player 1 wins\n";
    else if (bestVal < 0)
        cout << "Player 2 wins\n";
    else
        cout << "Draw\n";

    cout << "First move: " << bestFrom + 1 << " -> " << bestTo + 1 << "\n";
    cout << "Advantage: " << bestVal << "\n";

    return 0;
}
