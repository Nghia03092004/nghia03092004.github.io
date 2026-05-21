#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>

using namespace std;

int main() {
    const int board = 100;
    const int states = board / 2;
    const vector<pair<int, double>> deltas = {
        {-2, 1.0 / 36.0},
        {-1, 8.0 / 36.0},
        {0, 18.0 / 36.0},
        {1, 8.0 / 36.0},
        {2, 1.0 / 36.0},
    };

    vector<vector<double>> transition(states, vector<double>(states, 0.0));
    for (int distance = 1; distance <= states; distance++) {
        for (const auto& entry : deltas) {
            int delta = entry.first;
            double probability = entry.second;
            int raw = (distance + delta) % board;
            if (raw < 0) raw += board;
            int next = min(raw, board - raw);
            if (1 <= next && next <= states) {
                transition[distance - 1][next - 1] += probability;
            }
        }
    }

    vector<vector<double>> augmented(states, vector<double>(states + 1, 0.0));
    for (int row = 0; row < states; row++) {
        for (int col = 0; col < states; col++) {
            augmented[row][col] = (row == col ? 1.0 : 0.0) - transition[row][col];
        }
        augmented[row][states] = 1.0;
    }

    for (int col = 0; col < states; col++) {
        int pivot = col;
        for (int row = col + 1; row < states; row++) {
            if (fabs(augmented[row][col]) > fabs(augmented[pivot][col])) {
                pivot = row;
            }
        }
        swap(augmented[col], augmented[pivot]);

        for (int row = col + 1; row < states; row++) {
            double factor = augmented[row][col] / augmented[col][col];
            if (factor == 0.0) continue;
            for (int index = col; index <= states; index++) {
                augmented[row][index] -= factor * augmented[col][index];
            }
        }
    }

    vector<double> expectation(states, 0.0);
    for (int row = states - 1; row >= 0; row--) {
        double value = augmented[row][states];
        for (int col = row + 1; col < states; col++) {
            value -= augmented[row][col] * expectation[col];
        }
        expectation[row] = value / augmented[row][row];
    }

    cout << fixed << setprecision(6) << expectation.back() << '\n';
    return 0;
}
