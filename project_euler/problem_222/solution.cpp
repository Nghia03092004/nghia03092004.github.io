#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>

using namespace std;

double gap(int left, int right, double radius = 50.0) {
    return 2.0 * sqrt(radius * (left + right - radius));
}

double tubeLength(const vector<int>& order) {
    double total = order.front() + order.back();
    for (size_t i = 0; i + 1 < order.size(); i++) {
        total += gap(order[i], order[i + 1]);
    }
    return total;
}

vector<vector<int>> pendulumOrders(const vector<int>& radii) {
    vector<int> sorted = radii;
    sort(sorted.begin(), sorted.end());

    vector<vector<int>> orders;

    for (int parity = 0; parity < 2; parity++) {
        vector<int> first;
        vector<int> second;

        for (auto it = sorted.rbegin(); it != sorted.rend(); ++it) {
            if ((*it & 1) == parity) first.push_back(*it);
        }

        for (int value : sorted) {
            if ((value & 1) != parity) second.push_back(value);
        }

        orders.push_back(first);
        orders.back().insert(orders.back().end(), second.begin(), second.end());
    }

    return orders;
}

int main() {
    vector<int> radii;
    for (int value = 30; value <= 50; value++) {
        radii.push_back(value);
    }

    double best = 1e100;
    for (const auto& order : pendulumOrders(radii)) {
        best = min(best, tubeLength(order));
    }

    cout << llround(best * 1000.0) << '\n';
    return 0;
}
