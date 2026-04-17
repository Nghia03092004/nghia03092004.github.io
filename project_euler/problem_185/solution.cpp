#include <bits/stdc++.h>
using namespace std;

struct Clue {
    string guess;
    int correct;
};

vector<Clue> clues;
int secret[16];
set<int> possible[16]; // possible digits at each position

bool check_feasible(int pos) {
    // Check all clue constraints given assignment up to position `pos`
    // (positions 0..pos are assigned)
    for (auto& c : clues) {
        int matched = 0, can_match = 0;
        for (int i = 0; i < 16; i++) {
            if (i <= pos) {
                if (secret[i] == (c.guess[i] - '0'))
                    matched++;
            } else {
                if (possible[i].count(c.guess[i] - '0'))
                    can_match++;
            }
        }
        if (matched > c.correct) return false;
        if (matched + can_match < c.correct) return false;
    }
    return true;
}

bool solve(int depth, vector<int>& order) {
    if (depth == 16) {
        // Verify all constraints exactly
        for (auto& c : clues) {
            int matched = 0;
            for (int i = 0; i < 16; i++)
                if (secret[i] == (c.guess[i] - '0'))
                    matched++;
            if (matched != c.correct) return false;
        }
        return true;
    }

    int pos = order[depth];
    for (int d : possible[pos]) {
        secret[pos] = d;
        if (check_feasible(pos)) {
            // We need to check with all assigned positions, not just up to pos
            // Actually check_feasible checks positions 0..pos but order might not be sequential
            // Let's fix: check all assigned positions
        }
        if (solve(depth + 1, order))
            return true;
    }
    secret[pos] = -1;
    return false;
}

bool check_feasible_v2(int assigned[], int num_assigned) {
    for (auto& c : clues) {
        int matched = 0, can_match = 0;
        bool is_assigned[16] = {};
        for (int i = 0; i < num_assigned; i++)
            is_assigned[assigned[i]] = true;

        for (int i = 0; i < 16; i++) {
            if (is_assigned[i]) {
                if (secret[i] == (c.guess[i] - '0'))
                    matched++;
            } else {
                if (possible[i].count(c.guess[i] - '0'))
                    can_match++;
            }
        }
        if (matched > c.correct) return false;
        if (matched + can_match < c.correct) return false;
    }
    return true;
}

int assigned_positions[16];

bool solve2(int depth, vector<int>& order) {
    if (depth == 16) {
        for (auto& c : clues) {
            int matched = 0;
            for (int i = 0; i < 16; i++)
                if (secret[i] == (c.guess[i] - '0'))
                    matched++;
            if (matched != c.correct) return false;
        }
        return true;
    }

    int pos = order[depth];
    assigned_positions[depth] = pos;
    for (int d : possible[pos]) {
        secret[pos] = d;
        if (check_feasible_v2(assigned_positions, depth + 1)) {
            if (solve2(depth + 1, order))
                return true;
        }
    }
    secret[pos] = -1;
    return false;
}

int main(){
    clues = {
        {"5616185650518293", 2},
        {"3847439647293047", 1},
        {"5855462940810587", 3},
        {"9742855507068353", 3},
        {"4296849643607543", 3},
        {"3174248439465858", 1},
        {"4513559094146117", 2},
        {"7890971548908067", 3},
        {"8157356344118483", 1},
        {"2615250744386899", 2},
        {"8690095851526254", 3},
        {"6375711915077050", 1},
        {"6913859173121360", 1},
        {"6442889055042768", 2},
        {"2321386104303845", 0},
        {"2326509471271448", 2},
        {"5765889547612327", 0},
        {"6880193648553567", 0},
        {"8265420455326143", 2},
    };

    // Initialize possible digits
    for (int i = 0; i < 16; i++) {
        for (int d = 0; d <= 9; d++)
            possible[i].insert(d);
        secret[i] = -1;
    }

    // Apply zero-clue constraints
    for (auto& c : clues) {
        if (c.correct == 0) {
            for (int i = 0; i < 16; i++) {
                possible[i].erase(c.guess[i] - '0');
            }
        }
    }

    // Order positions by fewest candidates (MRV)
    vector<int> order(16);
    iota(order.begin(), order.end(), 0);
    sort(order.begin(), order.end(), [](int a, int b){
        return possible[a].size() < possible[b].size();
    });

    solve2(0, order);

    for (int i = 0; i < 16; i++)
        cout << secret[i];
    cout << endl;

    return 0;
}
