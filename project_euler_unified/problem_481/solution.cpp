/*
 * Project Euler Problem 481: Chef Showdown
 *
 * Chef k has skill S(k) = F_k / F_{n+1}. Chefs eliminate each other optimally.
 * Compute E(14) via bitmask DP over game states (remaining chefs, start index).
 *
 * Theorems applied:
 *   - Geometric round structure (Theorem 1): first-success probabilities P_j
 *   - Optimal elimination (Theorem 2): backward induction on win probabilities
 *   - Expected dishes (Theorem 3): geometric + recursive decomposition
 */

#include <bits/stdc++.h>
using namespace std;

const int MAXN = 14;
double fib[MAXN + 2];
double skill[MAXN];

void init_fib(int n) {
    fib[1] = fib[2] = 1;
    for (int i = 3; i <= n + 1; i++)
        fib[i] = fib[i - 1] + fib[i - 2];
}

vector<int> get_chefs(int mask, int n) {
    vector<int> v;
    for (int i = 0; i < n; i++)
        if (mask & (1 << i)) v.push_back(i);
    return v;
}

int find_next_turn(const vector<int>& order, int j, int elim_chef) {
    int m = order.size();
    for (int k = j + 1; k < m; k++)
        if (order[k] != elim_chef) return order[k];
    for (int k = 0; k < j; k++)
        if (order[k] != elim_chef) return order[k];
    return -1;
}

map<pair<int,int>, vector<double>> memo_win;
map<pair<int,int>, double> memo_exp;

vector<double> compute_win(int mask, int start_idx, int n);
double compute_exp(int mask, int start_idx, int n);

vector<double> compute_win(int mask, int start_idx, int n) {
    auto key = make_pair(mask, start_idx);
    auto it = memo_win.find(key);
    if (it != memo_win.end()) return it->second;

    vector<int> chefs = get_chefs(mask, n);
    int m = chefs.size();

    if (m == 1) {
        vector<double> w(n, 0);
        w[chefs[0]] = 1.0;
        return memo_win[key] = w;
    }

    vector<int> order;
    for (int i = start_idx; i < m; i++) order.push_back(chefs[i]);
    for (int i = 0; i < start_idx; i++) order.push_back(chefs[i]);

    vector<double> p_reach(m);
    p_reach[0] = 1.0;
    for (int j = 1; j < m; j++)
        p_reach[j] = p_reach[j - 1] * (1.0 - skill[order[j - 1]]);
    double p_none = p_reach[m - 1] * (1.0 - skill[order[m - 1]]);

    vector<double> result(n, 0);
    for (int j = 0; j < m; j++) {
        double p_j = p_reach[j] * skill[order[j]] / (1.0 - p_none);

        double best_wp = -1;
        int best_e = -1;
        vector<double> best_w;

        for (int e = 0; e < m; e++) {
            if (e == j) continue;
            int elim = order[e];
            int new_mask = mask ^ (1 << elim);
            vector<int> nc = get_chefs(new_mask, n);

            int nxt = find_next_turn(order, j, elim);
            int ns = 0;
            if (nxt >= 0)
                for (int k = 0; k < (int)nc.size(); k++)
                    if (nc[k] == nxt) { ns = k; break; }

            vector<double> w = compute_win(new_mask, ns, n);
            if (w[order[j]] > best_wp + 1e-15) {
                best_wp = w[order[j]];
                best_e = e;
                best_w = w;
            } else if (fabs(w[order[j]] - best_wp) < 1e-15) {
                if ((e - j + m) % m < (best_e - j + m) % m) {
                    best_e = e;
                    best_w = w;
                }
            }
        }

        for (int k = 0; k < n; k++)
            result[k] += p_j * best_w[k];
    }

    return memo_win[key] = result;
}

double compute_exp(int mask, int start_idx, int n) {
    auto key = make_pair(mask, start_idx);
    auto it = memo_exp.find(key);
    if (it != memo_exp.end()) return it->second;

    vector<int> chefs = get_chefs(mask, n);
    int m = chefs.size();

    if (m == 1) return memo_exp[key] = 0;

    vector<int> order;
    for (int i = start_idx; i < m; i++) order.push_back(chefs[i]);
    for (int i = 0; i < start_idx; i++) order.push_back(chefs[i]);

    vector<double> p_reach(m);
    p_reach[0] = 1.0;
    for (int j = 1; j < m; j++)
        p_reach[j] = p_reach[j - 1] * (1.0 - skill[order[j - 1]]);
    double p_none = p_reach[m - 1] * (1.0 - skill[order[m - 1]]);

    double exp_partial = 0;
    for (int j = 0; j < m; j++)
        exp_partial += (j + 1) * p_reach[j] * skill[order[j]];
    exp_partial /= (1.0 - p_none);

    double exp_dishes = p_none / (1.0 - p_none) * m + exp_partial;

    double exp_after = 0;
    for (int j = 0; j < m; j++) {
        double p_j = p_reach[j] * skill[order[j]] / (1.0 - p_none);

        double best_wp = -1;
        int best_e = -1, best_ns = 0, best_nm = 0;

        for (int e = 0; e < m; e++) {
            if (e == j) continue;
            int elim = order[e];
            int new_mask = mask ^ (1 << elim);
            vector<int> nc = get_chefs(new_mask, n);

            int nxt = find_next_turn(order, j, elim);
            int ns = 0;
            if (nxt >= 0)
                for (int k = 0; k < (int)nc.size(); k++)
                    if (nc[k] == nxt) { ns = k; break; }

            vector<double> w = compute_win(new_mask, ns, n);
            if (w[order[j]] > best_wp + 1e-15) {
                best_wp = w[order[j]];
                best_e = e;
                best_ns = ns;
                best_nm = new_mask;
            } else if (fabs(w[order[j]] - best_wp) < 1e-15) {
                if ((e - j + m) % m < (best_e - j + m) % m) {
                    best_e = e;
                    best_ns = ns;
                    best_nm = new_mask;
                }
            }
        }

        exp_after += p_j * compute_exp(best_nm, best_ns, n);
    }

    return memo_exp[key] = exp_dishes + exp_after;
}

int main() {
    int n = 7;
    init_fib(n);
    for (int k = 0; k < n; k++)
        skill[k] = fib[k + 1] / fib[n + 1];
    printf("E(7) = %.8f\n", compute_exp((1 << n) - 1, 0, n));

    memo_win.clear();
    memo_exp.clear();
    n = 14;
    init_fib(n);
    for (int k = 0; k < n; k++)
        skill[k] = fib[k + 1] / fib[n + 1];
    printf("E(14) = %.8f\n", compute_exp((1 << n) - 1, 0, n));

    return 0;
}
