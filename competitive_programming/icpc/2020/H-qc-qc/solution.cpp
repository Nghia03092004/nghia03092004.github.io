#include <bits/stdc++.h>
using namespace std;

namespace {

const int UNKNOWN = 0;
const int GOOD = 1;
const int BAD = 2;
const int MAXN = 100;
const int SIZE_CNT = 7;
const int SIZES[SIZE_CNT] = {1, 2, 4, 8, 16, 32, 64};

int size_index(int sz) {
    for (int i = 0; i < SIZE_CNT; ++i) {
        if (SIZES[i] == sz) {
            return i;
        }
    }
    return -1;
}

struct Group {
    int sz;
    int status;
    vector<int> members;
};

struct UPair {
    int a;
    int b;
    int sz;
};

struct OPair {
    int bad;
    int unk;
    int sz;
};

struct Action {
    unsigned char prep[SIZE_CNT];
    unsigned char finish_u[SIZE_CNT];
    unsigned char finish_o[SIZE_CNT];

    Action() {
        memset(prep, 0, sizeof(prep));
        memset(finish_u, 0, sizeof(finish_u));
        memset(finish_o, 0, sizeof(finish_o));
    }
};

struct SearchState {
    unsigned char rounds_left;
    unsigned char bad_budget;
    unsigned char u[SIZE_CNT];
    unsigned char o[SIZE_CNT];
    unsigned short good;

    bool operator==(const SearchState& other) const {
        return rounds_left == other.rounds_left &&
               bad_budget == other.bad_budget &&
               good == other.good &&
               memcmp(u, other.u, sizeof(u)) == 0 &&
               memcmp(o, other.o, sizeof(o)) == 0;
    }
};

struct SearchStateHash {
    size_t operator()(const SearchState& st) const {
        size_t h = st.rounds_left;
        h = h * 239017 + st.bad_budget;
        h = h * 239017 + st.good;
        for (int i = 0; i < SIZE_CNT; ++i) {
            h = h * 239017 + st.u[i];
        }
        for (int i = 0; i < SIZE_CNT; ++i) {
            h = h * 239017 + st.o[i];
        }
        return h;
    }
};

struct MemoEntry {
    bool known;
    bool win;
    Action action;

    MemoEntry() : known(false), win(false), action() {}
};

struct SearchEngine {
    unordered_map<SearchState, MemoEntry, SearchStateHash> memo;

    SearchState normalize(SearchState st) const {
        int good = st.good;
        int y = st.bad_budget;
        for (int i = 0; i < SIZE_CNT; ++i) {
            if (SIZES[i] > y && st.o[i] != 0) {
                good += SIZES[i] * st.o[i];
                st.o[i] = 0;
            }
        }
        if (good > MAXN) {
            good = MAXN;
        }
        st.good = static_cast<unsigned short>(good);
        return st;
    }

    bool empty_pairs(const SearchState& st) const {
        for (int i = 0; i < SIZE_CNT; ++i) {
            if (st.u[i] != 0 || st.o[i] != 0) {
                return false;
            }
        }
        return true;
    }

    bool can_win(const SearchState& raw, Action* best_action = nullptr) {
        SearchState st = normalize(raw);
        if (empty_pairs(st)) {
            if (best_action != nullptr) {
                *best_action = Action();
            }
            return true;
        }
        if (st.rounds_left == 0) {
            return false;
        }
        unordered_map<SearchState, MemoEntry, SearchStateHash>::iterator it = memo.find(st);
        if (it != memo.end() && it->second.known) {
            if (best_action != nullptr && it->second.win) {
                *best_action = it->second.action;
            }
            return it->second.win;
        }

        MemoEntry entry;
        vector<int> difficult;
        vector<int> easy_desc;
        for (int i = 0; i < SIZE_CNT; ++i) {
            if (st.u[i] == 0 && st.o[i] == 0) {
                continue;
            }
            if (SIZES[i] <= st.bad_budget) {
                difficult.push_back(i);
            } else {
                easy_desc.push_back(i);
            }
        }
        sort(easy_desc.begin(), easy_desc.end(), greater<int>());

        Action cur;
        vector<Action> actions;
        const int max_actions = 6000;
        generate_actions(st, difficult, easy_desc, 0, st.good, cur, actions, max_actions);
        sort(actions.begin(), actions.end(), [&](const Action& lhs, const Action& rhs) {
            int lhs_used = action_cost(lhs);
            int rhs_used = action_cost(rhs);
            if (lhs_used != rhs_used) {
                return lhs_used > rhs_used;
            }
            int lhs_gain = optimistic_gain(lhs);
            int rhs_gain = optimistic_gain(rhs);
            return lhs_gain > rhs_gain;
        });

        for (size_t idx = 0; idx < actions.size(); ++idx) {
            const Action& act = actions[idx];
            if (action_cost(act) == 0) {
                continue;
            }
            vector<vector<int> > bad_options;
            vector<int> bad_cur(SIZE_CNT, 0);
            generate_bad_options(st, act, difficult, 0, st.bad_budget, bad_cur, bad_options);
            bool ok = true;
            for (size_t j = 0; j < bad_options.size(); ++j) {
                SearchState nxt = transition(st, act, bad_options[j]);
                nxt.rounds_left--;
                if (!can_win(nxt, nullptr)) {
                    ok = false;
                    break;
                }
            }
            if (ok) {
                entry.known = true;
                entry.win = true;
                entry.action = act;
                memo[st] = entry;
                if (best_action != nullptr) {
                    *best_action = act;
                }
                return true;
            }
        }

        entry.known = true;
        entry.win = false;
        memo[st] = entry;
        return false;
    }

    int action_cost(const Action& act) const {
        int total = 0;
        for (int i = 0; i < SIZE_CNT; ++i) {
            total += act.prep[i];
            total += 2 * act.finish_u[i];
            total += act.finish_o[i];
        }
        return total;
    }

    int optimistic_gain(const Action& act) const {
        int total = 0;
        for (int i = 0; i < SIZE_CNT; ++i) {
            total += SIZES[i] * (act.prep[i] + act.finish_u[i] + act.finish_o[i]);
        }
        return total;
    }

    void generate_actions(const SearchState& st, const vector<int>& difficult,
                          const vector<int>& easy_desc, int pos, int rem,
                          Action& cur, vector<Action>& out, int max_actions) const {
        if ((int)out.size() >= max_actions) {
            return;
        }
        if (pos == (int)difficult.size()) {
            Action done = cur;
            int leftover = rem;
            for (int i = 0; i < (int)easy_desc.size() && leftover > 0; ++i) {
                int idx = easy_desc[i];
                int take = min<int>(st.u[idx], leftover);
                done.prep[idx] = static_cast<unsigned char>(take);
                leftover -= take;
            }
            if (action_cost(done) > 0) {
                out.push_back(done);
            }
            return;
        }

        int idx = difficult[pos];
        int max_prep = min<int>(st.u[idx], rem);
        for (int prep = 0; prep <= max_prep; ++prep) {
            int max_finish_u = min<int>(st.u[idx] - prep, (rem - prep) / 2);
            for (int finish_u = 0; finish_u <= max_finish_u; ++finish_u) {
                int max_finish_o = min<int>(st.o[idx], rem - prep - 2 * finish_u);
                for (int finish_o = 0; finish_o <= max_finish_o; ++finish_o) {
                    cur.prep[idx] = static_cast<unsigned char>(prep);
                    cur.finish_u[idx] = static_cast<unsigned char>(finish_u);
                    cur.finish_o[idx] = static_cast<unsigned char>(finish_o);
                    generate_actions(st, difficult, easy_desc, pos + 1,
                                     rem - prep - 2 * finish_u - finish_o,
                                     cur, out, max_actions);
                    if ((int)out.size() >= max_actions) {
                        cur.prep[idx] = cur.finish_u[idx] = cur.finish_o[idx] = 0;
                        return;
                    }
                }
            }
        }
        cur.prep[idx] = cur.finish_u[idx] = cur.finish_o[idx] = 0;
    }

    void generate_bad_options(const SearchState& st, const Action& act,
                              const vector<int>& difficult, int pos, int rem_budget,
                              vector<int>& cur, vector<vector<int> >& out) const {
        if (pos == (int)difficult.size()) {
            out.push_back(cur);
            return;
        }
        int idx = difficult[pos];
        int cnt = act.finish_u[idx] + act.finish_o[idx];
        int w = SIZES[idx];
        int mx = min<int>(cnt, rem_budget / w);
        for (int bad = 0; bad <= mx; ++bad) {
            cur[idx] = bad;
            generate_bad_options(st, act, difficult, pos + 1, rem_budget - bad * w, cur, out);
        }
        cur[idx] = 0;
    }

    SearchState transition(const SearchState& st, const Action& act,
                           const vector<int>& bad_per_size) const {
        SearchState nxt = st;
        int gain = 0;
        int bad_cost = 0;
        for (int i = 0; i < SIZE_CNT; ++i) {
            int prep = act.prep[i];
            int finish_u = act.finish_u[i];
            int finish_o = act.finish_o[i];
            int bad = bad_per_size[i];
            nxt.u[i] = static_cast<unsigned char>(nxt.u[i] - prep - finish_u);
            nxt.o[i] = static_cast<unsigned char>(nxt.o[i] + prep - finish_o);
            gain += SIZES[i] * (finish_u + finish_o - bad);
            bad_cost += SIZES[i] * bad;
        }
        int good = min<int>(MAXN, st.good + gain);
        int budget = st.bad_budget - bad_cost;
        if (budget < 0) {
            budget = 0;
        }
        nxt.good = static_cast<unsigned short>(good);
        nxt.bad_budget = static_cast<unsigned char>(budget);
        return nxt;
    }
};

struct Solver {
    int n;
    int rounds_used;
    vector<Group> groups;
    vector<UPair> upairs;
    vector<OPair> opairs;
    vector<int> leftovers;
    vector<int> good_pool;
    int bad_budget;
    SearchEngine engine;

#ifdef LOCAL
    vector<int> hidden_good;
    vector<vector<int> > hidden_ans;
#endif

    Solver() : n(0), rounds_used(0), groups(1), upairs(), opairs(),
               leftovers(), good_pool(), bad_budget(0), engine() {}

    int add_group(const vector<int>& members, int status = UNKNOWN) {
        Group g;
        g.sz = (int)members.size();
        g.status = status;
        g.members = members;
        groups.push_back(g);
        return (int)groups.size() - 1;
    }

    int add_singleton(int x) {
        vector<int> v(1, x);
        return add_group(v, UNKNOWN);
    }

    int merge_groups(int a, int b) {
        vector<int> merged = groups[a].members;
        merged.insert(merged.end(), groups[b].members.begin(), groups[b].members.end());
        return add_group(merged, UNKNOWN);
    }

    void mark_good(int gid) {
        if (groups[gid].status == GOOD) {
            return;
        }
        groups[gid].status = GOOD;
        for (int i = 0; i < (int)groups[gid].members.size(); ++i) {
            good_pool.push_back(groups[gid].members[i]);
        }
    }

    void mark_bad(int gid) {
        groups[gid].status = BAD;
    }

    int representative(int gid) const {
        return groups[gid].members[0];
    }

    string ask_round(const vector<int>& to) {
        vector<int> used_to(n + 1, 0);
        vector<int> used_from(n + 1, 0);
        for (int i = 1; i <= n; ++i) {
            if (to[i] == 0) {
                continue;
            }
            used_from[i]++;
            used_to[to[i]]++;
            if (to[i] == i) {
                throw runtime_error("self-test is forbidden");
            }
        }
        for (int i = 1; i <= n; ++i) {
            if (used_from[i] > 1 || used_to[i] > 1) {
                throw runtime_error("invalid round schedule");
            }
        }
        rounds_used++;
        if (rounds_used > 12) {
            throw runtime_error("used more than 12 rounds");
        }
#ifdef LOCAL
        string ret(n, '-');
        for (int i = 1; i <= n; ++i) {
            if (to[i] != 0) {
                ret[i - 1] = char('0' + hidden_ans[i][to[i]]);
            }
        }
        return ret;
#else
        cout << "test";
        for (int i = 1; i <= n; ++i) {
            cout << ' ' << to[i];
        }
        cout << '\n';
        cout.flush();
        string ret;
        cin >> ret;
        return ret;
#endif
    }

    void send_answer() {
        string ans(n, '0');
        for (int gid = 1; gid < (int)groups.size(); ++gid) {
            if (groups[gid].status == GOOD) {
                for (int x : groups[gid].members) {
                    ans[x - 1] = '1';
                }
            }
        }
#ifdef LOCAL
        for (int i = 1; i <= n; ++i) {
            if ((ans[i - 1] == '1') != (hidden_good[i] == 1)) {
                cerr << "answer=" << ans << '\n';
                throw runtime_error("wrong answer");
            }
        }
#else
        cout << "answer " << ans << '\n';
        cout.flush();
#endif
    }

    void normalize_easy_oriented() {
        bool changed = true;
        while (changed) {
            changed = false;
            vector<OPair> kept;
            for (int i = 0; i < (int)opairs.size(); ++i) {
                if (opairs[i].sz > bad_budget) {
                    mark_bad(opairs[i].bad);
                    mark_good(opairs[i].unk);
                    changed = true;
                } else {
                    kept.push_back(opairs[i]);
                }
            }
            opairs.swap(kept);
        }
    }

    SearchState build_state(int rounds_left) {
        SearchState st;
        st.rounds_left = static_cast<unsigned char>(rounds_left);
        st.bad_budget = static_cast<unsigned char>(max(0, bad_budget));
        st.good = static_cast<unsigned short>(min<int>(MAXN, (int)good_pool.size()));
        memset(st.u, 0, sizeof(st.u));
        memset(st.o, 0, sizeof(st.o));
        for (int i = 0; i < (int)upairs.size(); ++i) {
            int idx = size_index(upairs[i].sz);
            st.u[idx]++;
        }
        for (int i = 0; i < (int)opairs.size(); ++i) {
            int idx = size_index(opairs[i].sz);
            st.o[idx]++;
        }
        return st;
    }

    void phase_one() {
        vector<int> active;
        for (int i = 1; i <= n; ++i) {
            active.push_back(add_singleton(i));
        }

        while ((int)active.size() >= 2) {
            vector<int> next_active;
            vector<pair<int, int> > pairs;
            vector<int> to(n + 1, 0);
            int m = (int)active.size();
            if (m % 2 == 1) {
                leftovers.push_back(active.back());
                active.pop_back();
            }
            for (int i = 0; i < (int)active.size(); i += 2) {
                int a = active[i];
                int b = active[i + 1];
                pairs.push_back(make_pair(a, b));
                to[representative(a)] = representative(b);
                to[representative(b)] = representative(a);
            }
            string ret = ask_round(to);
            for (int i = 0; i < (int)pairs.size(); ++i) {
                int a = pairs[i].first;
                int b = pairs[i].second;
                int ra = ret[representative(a) - 1] - '0';
                int rb = ret[representative(b) - 1] - '0';
                if (ra == 1 && rb == 1) {
                    next_active.push_back(merge_groups(a, b));
                } else if (ra == 1 && rb == 0) {
                    OPair p;
                    p.bad = a;
                    p.unk = b;
                    p.sz = groups[a].sz;
                    opairs.push_back(p);
                } else if (ra == 0 && rb == 1) {
                    OPair p;
                    p.bad = b;
                    p.unk = a;
                    p.sz = groups[a].sz;
                    opairs.push_back(p);
                } else {
                    UPair p;
                    p.a = a;
                    p.b = b;
                    p.sz = groups[a].sz;
                    upairs.push_back(p);
                }
            }
            active.swap(next_active);
        }
        if (!active.empty()) {
            leftovers.push_back(active[0]);
        }
    }

    void initialize_pair_game() {
        if (leftovers.empty()) {
            throw runtime_error("there must be a leftover good group");
        }
        int best = leftovers[0];
        for (int i = 1; i < (int)leftovers.size(); ++i) {
            if (groups[leftovers[i]].sz > groups[best].sz) {
                best = leftovers[i];
            }
        }
        vector<int> reordered;
        int other_sum = 0;
        reordered.push_back(best);
        for (int gid : leftovers) {
            if (gid != best) {
                reordered.push_back(gid);
                other_sum += groups[gid].sz;
            }
        }
        if (!(groups[best].sz > other_sum)) {
            throw runtime_error("largest leftover must dominate the rest");
        }
        mark_good(best);
        leftovers.swap(reordered);
        classify_other_leftovers();
        int good_sum = 0;
        int bad_sum = 0;
        for (int gid : leftovers) {
            if (groups[gid].status == GOOD) {
                good_sum += groups[gid].sz;
            } else if (groups[gid].status == BAD) {
                bad_sum += groups[gid].sz;
            } else {
                throw runtime_error("all leftovers must be classified before pair game");
            }
        }
        if (good_sum <= bad_sum) {
            throw runtime_error("classified leftovers must have a strict good majority");
        }
        bad_budget = (good_sum - bad_sum - 1) / 2;
        normalize_easy_oriented();
    }

    vector<int> take_good_testers(int need) {
        if ((int)good_pool.size() < need) {
            throw runtime_error("not enough good testers");
        }
        vector<int> testers;
        testers.reserve(need);
        for (int i = 0; i < need; ++i) {
            testers.push_back(good_pool[i]);
        }
        return testers;
    }

    void execute_pair_action(const Action& act) {
        vector<vector<int> > up_by_size(SIZE_CNT), op_by_size(SIZE_CNT);
        for (int i = 0; i < (int)upairs.size(); ++i) {
            up_by_size[size_index(upairs[i].sz)].push_back(i);
        }
        for (int i = 0; i < (int)opairs.size(); ++i) {
            op_by_size[size_index(opairs[i].sz)].push_back(i);
        }

        int need = 0;
        for (int i = 0; i < SIZE_CNT; ++i) {
            need += act.prep[i];
            need += 2 * act.finish_u[i];
            need += act.finish_o[i];
        }
        vector<int> testers = take_good_testers(need);
        int ptr = 0;
        vector<int> to(n + 1, 0);

        struct PrepTask {
            int pair_idx;
            bool is_u;
            int group_a;
            int group_b;
            int tester;
            int sz;
        };
        struct FinishUTask {
            int pair_idx;
            int group_a;
            int group_b;
            int tester_a;
            int tester_b;
            int sz;
        };
        struct FinishOTask {
            int pair_idx;
            int bad_gid;
            int unk_gid;
            int tester;
            int sz;
        };

        vector<PrepTask> prep_tasks;
        vector<FinishUTask> finish_u_tasks;
        vector<FinishOTask> finish_o_tasks;
        vector<int> used_u(upairs.size(), 0), used_o(opairs.size(), 0);

        for (int idx = 0; idx < SIZE_CNT; ++idx) {
            for (int k = 0; k < act.prep[idx]; ++k) {
                int pair_idx = up_by_size[idx][k];
                used_u[pair_idx] = 1;
                PrepTask task;
                task.pair_idx = pair_idx;
                task.is_u = true;
                task.group_a = upairs[pair_idx].a;
                task.group_b = upairs[pair_idx].b;
                task.sz = upairs[pair_idx].sz;
                task.tester = testers[ptr++];
                to[task.tester] = representative(task.group_a);
                prep_tasks.push_back(task);
            }
            for (int k = 0; k < act.finish_u[idx]; ++k) {
                int pair_idx = up_by_size[idx][act.prep[idx] + k];
                used_u[pair_idx] = 1;
                FinishUTask task;
                task.pair_idx = pair_idx;
                task.group_a = upairs[pair_idx].a;
                task.group_b = upairs[pair_idx].b;
                task.sz = upairs[pair_idx].sz;
                task.tester_a = testers[ptr++];
                task.tester_b = testers[ptr++];
                to[task.tester_a] = representative(task.group_a);
                to[task.tester_b] = representative(task.group_b);
                finish_u_tasks.push_back(task);
            }
            for (int k = 0; k < act.finish_o[idx]; ++k) {
                int pair_idx = op_by_size[idx][k];
                used_o[pair_idx] = 1;
                FinishOTask task;
                task.pair_idx = pair_idx;
                task.bad_gid = opairs[pair_idx].bad;
                task.unk_gid = opairs[pair_idx].unk;
                task.sz = opairs[pair_idx].sz;
                task.tester = testers[ptr++];
                to[task.tester] = representative(task.unk_gid);
                finish_o_tasks.push_back(task);
            }
        }

        string ret = ask_round(to);

        vector<UPair> next_u;
        vector<OPair> next_o;
        for (int i = 0; i < (int)upairs.size(); ++i) {
            if (!used_u[i]) {
                next_u.push_back(upairs[i]);
            }
        }
        for (int i = 0; i < (int)opairs.size(); ++i) {
            if (!used_o[i]) {
                next_o.push_back(opairs[i]);
            }
        }

        for (const PrepTask& task : prep_tasks) {
            int r = ret[task.tester - 1] - '0';
            if (r == 1) {
                mark_good(task.group_a);
                mark_bad(task.group_b);
            } else {
                mark_bad(task.group_a);
                OPair p;
                p.bad = task.group_a;
                p.unk = task.group_b;
                p.sz = task.sz;
                next_o.push_back(p);
            }
        }

        for (const FinishUTask& task : finish_u_tasks) {
            int ra = ret[task.tester_a - 1] - '0';
            int rb = ret[task.tester_b - 1] - '0';
            if (ra == 1 && rb == 0) {
                mark_good(task.group_a);
                mark_bad(task.group_b);
            } else if (ra == 0 && rb == 1) {
                mark_good(task.group_b);
                mark_bad(task.group_a);
            } else if (ra == 0 && rb == 0) {
                mark_bad(task.group_a);
                mark_bad(task.group_b);
                bad_budget -= task.sz;
                if (bad_budget < 0) {
                    bad_budget = 0;
                }
            } else {
                throw runtime_error("double-check on a U-pair returned 11");
            }
        }

        for (const FinishOTask& task : finish_o_tasks) {
            int r = ret[task.tester - 1] - '0';
            mark_bad(task.bad_gid);
            if (r == 1) {
                mark_good(task.unk_gid);
            } else {
                mark_bad(task.unk_gid);
                bad_budget -= task.sz;
                if (bad_budget < 0) {
                    bad_budget = 0;
                }
            }
        }

        upairs.swap(next_u);
        opairs.swap(next_o);
        normalize_easy_oriented();
    }

    void classify_other_leftovers() {
        vector<int> unknown_leftovers;
        for (int gid : leftovers) {
            if (groups[gid].status == UNKNOWN) {
                unknown_leftovers.push_back(gid);
            }
        }
        if (unknown_leftovers.empty()) {
            return;
        }
        vector<int> testers = take_good_testers((int)unknown_leftovers.size());
        vector<int> to(n + 1, 0);
        for (int i = 0; i < (int)unknown_leftovers.size(); ++i) {
            to[testers[i]] = representative(unknown_leftovers[i]);
        }
        string ret = ask_round(to);
        for (int i = 0; i < (int)unknown_leftovers.size(); ++i) {
            if (ret[testers[i] - 1] == '1') {
                mark_good(unknown_leftovers[i]);
            } else {
                mark_bad(unknown_leftovers[i]);
            }
        }
    }

    void solve_case() {
        phase_one();
        initialize_pair_game();
        while (!upairs.empty() || !opairs.empty()) {
            int rounds_left = 12 - rounds_used;
            SearchState st = build_state(rounds_left);
            Action act;
            if (!engine.can_win(st, &act)) {
                throw runtime_error("no winning action found for the compressed state");
            }
            execute_pair_action(act);
        }
        send_answer();
    }
};

#ifdef LOCAL
mt19937 rng(1234567);

Solver build_random_solver(int n) {
    Solver solver;
    solver.n = n;
    solver.hidden_good.assign(n + 1, 0);
    int good_cnt = 0;
    while (good_cnt * 2 <= n) {
        good_cnt = 0;
        for (int i = 1; i <= n; ++i) {
            solver.hidden_good[i] = int(rng() & 1);
            good_cnt += solver.hidden_good[i];
        }
    }
    solver.hidden_ans.assign(n + 1, vector<int>(n + 1, 0));
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= n; ++j) {
            if (i == j) {
                continue;
            }
            if (solver.hidden_good[i]) {
                solver.hidden_ans[i][j] = solver.hidden_good[j];
            } else {
                solver.hidden_ans[i][j] = int(rng() & 1);
            }
        }
    }
    return solver;
}
#endif

void solve() {
#ifdef LOCAL
    for (int n = 1; n <= 100; ++n) {
        for (int it = 0; it < 200; ++it) {
            Solver solver = build_random_solver(n);
            try {
                solver.solve_case();
            } catch (const exception& e) {
                cerr << "fail n=" << n << " it=" << it
                     << " rounds=" << solver.rounds_used
                     << " budget=" << solver.bad_budget << '\n';
                cerr << "hidden:";
                for (int i = 1; i <= n; ++i) {
                    cerr << solver.hidden_good[i];
                }
                cerr << '\n';
                for (int i = 1; i <= n; ++i) {
                    cerr << "row" << i << ':';
                    for (int j = 1; j <= n; ++j) {
                        if (i == j) {
                            cerr << '-';
                        } else {
                            cerr << solver.hidden_ans[i][j];
                        }
                    }
                    cerr << '\n';
                }
                throw;
            }
        }
    }
    cerr << "local-random-ok\n";
#else
    int batches;
    cin >> batches;
    while (batches--) {
        Solver solver;
        cin >> solver.n;
        solver.solve_case();
    }
#endif
}

}  // namespace

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    solve();
    return 0;
}
