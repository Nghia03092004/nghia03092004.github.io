#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int PLAYERS = 4;
constexpr int SETS = 8;
constexpr int CARDS_PER_SET = 4;
constexpr int TOTAL_CARDS = 32;
constexpr int UNKNOWN = -1;
constexpr int GONE = 4;

struct Action {
    char type = '?';  // 'A' or 'Q'
    int x = -1;
    int y = -1;
    int set_id = -1;
    int card = -1;
    bool yes = false;
};

int card_id(int set_id, char rank) {
    return set_id * CARDS_PER_SET + (rank - 'A');
}

bool player_known_to_have_set(int player, int set_id,
                              const array<array<char, SETS>, PLAYERS>& need,
                              const array<int, TOTAL_CARDS>& cur_owner) {
    if (need[player][set_id]) {
        return true;
    }
    int first = set_id * CARDS_PER_SET;
    for (int card = first; card < first + CARDS_PER_SET; ++card) {
        if (cur_owner[card] == player) {
            return true;
        }
    }
    return false;
}

bool assign_start_owner(int card, int player, array<int, TOTAL_CARDS>& start_owner,
                        array<array<char, PLAYERS>, TOTAL_CARDS>& forbidden) {
    if (forbidden[card][player]) {
        return false;
    }
    if (start_owner[card] != UNKNOWN && start_owner[card] != player) {
        return false;
    }
    start_owner[card] = player;
    return true;
}

bool prefix_consistent(const vector<Action>& actions, int prefix_len) {
    array<int, TOTAL_CARDS> start_owner;
    start_owner.fill(UNKNOWN);

    array<int, TOTAL_CARDS> cur_owner;
    cur_owner.fill(UNKNOWN);

    array<array<char, PLAYERS>, TOTAL_CARDS> forbidden{};
    array<array<char, SETS>, PLAYERS> need{};

    for (int i = 0; i < prefix_len; ++i) {
        const Action& action = actions[i];
        if (action.type == 'A') {
            int x = action.x;
            int y = action.y;
            int set_id = action.set_id;
            int card = action.card;

            if (!player_known_to_have_set(x, set_id, need, cur_owner)) {
                need[x][set_id] = true;
            }

            if (action.yes) {
                if (cur_owner[card] == GONE) {
                    return false;
                }
                if (cur_owner[card] != UNKNOWN && cur_owner[card] != y) {
                    return false;
                }

                bool resolved_start = false;
                if (cur_owner[card] == UNKNOWN) {
                    if (!assign_start_owner(card, y, start_owner, forbidden)) {
                        return false;
                    }
                    resolved_start = true;
                }
                cur_owner[card] = x;
                if (resolved_start) {
                    need[y][set_id] = false;
                }
            } else {
                if (cur_owner[card] == y || cur_owner[card] == GONE) {
                    return false;
                }
                if (cur_owner[card] == UNKNOWN) {
                    if (start_owner[card] == y) {
                        return false;
                    }
                    forbidden[card][y] = true;
                }
            }
        } else {
            int x = action.x;
            int set_id = action.set_id;
            for (int offset = 0; offset < CARDS_PER_SET; ++offset) {
                int card = set_id * CARDS_PER_SET + offset;
                if (cur_owner[card] == GONE) {
                    return false;
                }
                if (cur_owner[card] != UNKNOWN && cur_owner[card] != x) {
                    return false;
                }
                if (cur_owner[card] == UNKNOWN) {
                    if (!assign_start_owner(card, x, start_owner, forbidden)) {
                        return false;
                    }
                }
                cur_owner[card] = GONE;
            }
            need[x][set_id] = false;
        }
    }

    static bool dp[9][9][9];
    static bool next_dp[9][9][9];
    memset(dp, 0, sizeof(dp));
    dp[0][0][0] = true;

    for (int set_id = 0; set_id < SETS; ++set_id) {
        vector<array<int, PLAYERS>> options;
        options.reserve(256);

        for (int mask = 0; mask < (1 << (2 * CARDS_PER_SET)); ++mask) {
            array<int, PLAYERS> cnt{};
            bool ok = true;
            for (int i = 0; i < CARDS_PER_SET; ++i) {
                int player = (mask >> (2 * i)) & 3;
                int card = set_id * CARDS_PER_SET + i;
                if (start_owner[card] != UNKNOWN && start_owner[card] != player) {
                    ok = false;
                    break;
                }
                if (forbidden[card][player]) {
                    ok = false;
                    break;
                }
                ++cnt[player];
            }
            if (!ok) {
                continue;
            }
            for (int player = 0; player < PLAYERS; ++player) {
                if (need[player][set_id] && cnt[player] == 0) {
                    ok = false;
                    break;
                }
            }
            if (ok) {
                options.push_back(cnt);
            }
        }

        if (options.empty()) {
            return false;
        }

        memset(next_dp, 0, sizeof(next_dp));
        int processed_cards = set_id * CARDS_PER_SET;
        for (int c0 = 0; c0 <= 8; ++c0) {
            for (int c1 = 0; c1 <= 8; ++c1) {
                for (int c2 = 0; c2 <= 8; ++c2) {
                    if (!dp[c0][c1][c2]) {
                        continue;
                    }
                    int c3 = processed_cards - c0 - c1 - c2;
                    if (c3 < 0 || c3 > 8) {
                        continue;
                    }
                    for (const auto& cnt : options) {
                        int n0 = c0 + cnt[0];
                        int n1 = c1 + cnt[1];
                        int n2 = c2 + cnt[2];
                        int n3 = c3 + cnt[3];
                        if (n0 > 8 || n1 > 8 || n2 > 8 || n3 > 8) {
                            continue;
                        }
                        next_dp[n0][n1][n2] = true;
                    }
                }
            }
        }
        memcpy(dp, next_dp, sizeof(dp));
    }

    return dp[8][8][8];
}

void solve() {
    int n;
    cin >> n;

    vector<Action> actions;
    actions.reserve(n);
    for (int i = 0; i < n; ++i) {
        Action action;
        cin >> action.x;
        --action.x;
        char kind;
        cin >> kind;
        if (kind == 'A') {
            action.type = 'A';
            cin >> action.y;
            --action.y;
            string card;
            string result;
            cin >> card >> result;
            action.set_id = card[0] - '1';
            action.card = card_id(action.set_id, card[1]);
            action.yes = (result == "yes");
        } else {
            action.type = 'Q';
            string set_str;
            cin >> set_str;
            action.set_id = set_str[0] - '1';
        }
        actions.push_back(action);
    }

    if (prefix_consistent(actions, n)) {
        cout << "yes\n";
        return;
    }

    int low = 1;
    int high = n;
    while (low < high) {
        int mid = (low + high) / 2;
        if (prefix_consistent(actions, mid)) {
            low = mid + 1;
        } else {
            high = mid;
        }
    }

    cout << "no\n" << low << '\n';
}

}  // namespace

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    solve();
    return 0;
}
