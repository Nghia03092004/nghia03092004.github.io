#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

const ll MOD = 1e9 + 7;

ll power(ll base, ll exp, ll mod) {
    ll result = 1;
    base %= mod;
    while (exp > 0) {
        if (exp & 1) result = result * base % mod;
        base = base * base % mod;
        exp >>= 1;
    }
    return result;
}

ll modinv(ll a, ll mod) {
    return power(a, mod - 2, mod);
}

// State: tuple of peg assignments for disks 1..n, encoded as base-3 number
int encode(vector<int>& pegs) {
    int code = 0;
    for (int i = pegs.size() - 1; i >= 0; i--) {
        code = code * 3 + pegs[i];
    }
    return code;
}

void decode(int code, int n, vector<int>& pegs) {
    pegs.resize(n);
    for (int i = 0; i < n; i++) {
        pegs[i] = code % 3;
        code /= 3;
    }
}

int main() {
    int n = 4; // Small n for demonstration

    int total_states = 1;
    for (int i = 0; i < n; i++) total_states *= 3;

    // Initial: all on peg 0; Goal: all on peg 2
    int initial = 0; // all pegs[i] = 0
    int goal = 0;
    {
        vector<int> g(n, 2);
        goal = encode(g);
    }

    // For each state, find legal moves
    vector<vector<int>> neighbors(total_states);
    for (int s = 0; s < total_states; s++) {
        vector<int> pegs;
        decode(s, n, pegs);

        // Find top disk on each peg
        map<int, int> top; // peg -> smallest disk
        for (int d = 0; d < n; d++) {
            int p = pegs[d];
            if (top.find(p) == top.end()) {
                top[p] = d; // disk d is smallest (0-indexed, 0=smallest)
            }
        }

        for (auto& [from_peg, disk] : top) {
            for (int to_peg = 0; to_peg < 3; to_peg++) {
                if (to_peg == from_peg) continue;
                // Check if legal
                if (top.find(to_peg) == top.end() || top[to_peg] > disk) {
                    vector<int> new_pegs = pegs;
                    new_pegs[disk] = to_peg;
                    neighbors[s].push_back(encode(new_pegs));
                }
            }
        }
    }

    // Solve linear system E[s] = 1 + (1/deg) * sum E[neighbors] for s != goal
    // E[goal] = 0
    // Using iterative method with doubles for demonstration
    vector<double> E(total_states, 0.0);
    for (int iter = 0; iter < 100000; iter++) {
        vector<double> E_new(total_states, 0.0);
        double max_diff = 0;
        for (int s = 0; s < total_states; s++) {
            if (s == goal) { E_new[s] = 0; continue; }
            double sum = 0;
            for (int nb : neighbors[s]) sum += E[nb];
            E_new[s] = 1.0 + sum / neighbors[s].size();
            max_diff = max(max_diff, abs(E_new[s] - E[s]));
        }
        E = E_new;
        if (max_diff < 1e-12) break;
    }

    printf("E(%d) = %.6f\n", n, E[initial]);
    printf("Optimal = %d\n", (1 << n) - 1);

    return 0;
}
