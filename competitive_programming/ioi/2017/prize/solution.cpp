#include <bits/stdc++.h>
using namespace std;

// Grader-provided function.
pair<int, int> ask(int i);

namespace {

vector<pair<int, int>> cached;
map<int, vector<int>> positions_by_sum;

pair<int, int> query(int idx) {
    if (cached[idx].first != -1) {
        return cached[idx];
    }

    pair<int, int> res = ask(idx);
    cached[idx] = res;

    int sum = res.first + res.second;
    vector<int>& positions = positions_by_sum[sum];
    positions.insert(lower_bound(positions.begin(), positions.end(), idx), idx);
    return res;
}

int count_between(const vector<int>& positions, int left, int right) {
    auto it_left = upper_bound(positions.begin(), positions.end(), left);
    auto it_right = lower_bound(positions.begin(), positions.end(), right);
    return int(it_right - it_left);
}

bool can_skip_segment(int left, int right) {
    for (const auto& [sum, positions] : positions_by_sum) {
        if (sum == 0) {
            continue;
        }

        auto it_right = upper_bound(positions.begin(), positions.end(), right);
        if (it_right == positions.end()) {
            continue;
        }
        int outer_right = *it_right;

        auto it_left = lower_bound(positions.begin(), positions.end(), left);
        if (it_left == positions.begin()) {
            continue;
        }
        --it_left;
        int outer_left = *it_left;

        const auto [left_a, left_b] = cached[outer_left];
        const auto [right_a, right_b] = cached[outer_right];
        (void)left_b;
        (void)right_b;

        int identified_more_expensive = 0;
        for (const auto& [smaller_sum, smaller_positions] : positions_by_sum) {
            if (smaller_sum >= sum) {
                break;
            }
            identified_more_expensive += count_between(smaller_positions, outer_left, outer_right);
        }

        if (identified_more_expensive == right_a - left_a) {
            return true;
        }
    }
    return false;
}

}  // namespace

int find_best(int n) {
    cached.assign(n, {-1, -1});
    positions_by_sum.clear();

    vector<pair<int, int>> segments;
    segments.push_back({0, n - 1});

    while (!segments.empty()) {
        auto [left, right] = segments.back();
        segments.pop_back();

        if (left > right) {
            continue;
        }
        if (can_skip_segment(left, right)) {
            continue;
        }

        int mid = left + (right - left) / 2;
        auto [a, b] = query(mid);
        if (a + b == 0) {
            return mid;
        }

        segments.push_back({mid + 1, right});
        segments.push_back({left, mid - 1});
    }

    return -1;
}

int main() {
    // Grader handles interaction.
    return 0;
}
