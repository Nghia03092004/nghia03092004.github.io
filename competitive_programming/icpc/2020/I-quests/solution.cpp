#include <bits/stdc++.h>
using namespace std;

namespace {

struct Quest {
    int x;
    int deadline;
};

void solve() {
    int n, v, c;
    cin >> n >> v >> c;

    vector<Quest> quests(n);
    int sum_x = 0;
    long long base_xp = 0;

    for (int i = 0; i < n; ++i) {
        int x, d;
        cin >> x >> d;
        long long latest_start = (1LL * d * v - 1) / c;
        quests[i] = {x, int(latest_start + x)};
        sum_x += x;
        base_xp += x;
    }

    sort(quests.begin(), quests.end(), [](const Quest& a, const Quest& b) {
        if (a.deadline != b.deadline) {
            return a.deadline < b.deadline;
        }
        return a.x < b.x;
    });

    vector<unsigned long long> reachable((sum_x >> 6) + 1, 0);
    reachable[0] = 1ULL;
    int max_reachable = 0;

    for (const Quest& quest : quests) {
        int shift = quest.x;
        int limit = min(sum_x, quest.deadline);
        int target_max = min(limit, max_reachable + shift);
        int word_shift = shift >> 6;
        int bit_shift = shift & 63;

        for (int word = target_max >> 6; word >= word_shift; --word) {
            unsigned long long value = reachable[word - word_shift] << bit_shift;
            if (bit_shift != 0 && word - word_shift - 1 >= 0) {
                value |= reachable[word - word_shift - 1] >> (64 - bit_shift);
            }
            reachable[word] |= value;
        }

        reachable[target_max >> 6] &= (~0ULL) >> (63 - (target_max & 63));
        max_reachable = target_max;
    }

    int best_bonus_sum = 0;
    for (int s = max_reachable; s >= 0; --s) {
        if ((reachable[s >> 6] >> (s & 63)) & 1ULL) {
            best_bonus_sum = s;
            break;
        }
    }

    cout << base_xp + 1LL * (c - 1) * best_bonus_sum << '\n';
}

}  // namespace

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    solve();
    return 0;
}
