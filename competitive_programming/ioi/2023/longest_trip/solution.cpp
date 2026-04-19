#include <bits/stdc++.h>
using namespace std;

// IOI 2023 - Longest Trip (Interactive)
// Graph with N nodes, density D: any triplet has >= D edges among 3 possible.
// Find longest simple path using are_connected(A, B) queries.
//
// Algorithm:
//   1. Maintain two paths P and Q.
//   2. For each new node, try to attach to an endpoint of P or Q.
//   3. If neither works, by density >= 1, P.back()-Q.back() must be an edge,
//      so merge P and Q and start Q fresh with the new node.
//   4. After processing all nodes, merge P and Q via endpoint checks,
//      or binary search for an internal edge and splice the longer halves.

bool are_connected(vector<int> A, vector<int> B); // grader function

vector<int> longest_trip(int N, int D) {
    if (N == 1) return {0};

    deque<int> P, Q;
    P.push_back(0);
    if (N > 1) Q.push_back(1);

    // Phase 1: Build two paths by attaching each node to an endpoint.
    for (int i = 2; i < N; i++) {
        if (are_connected({i}, {(int)P.back()})) {
            P.push_back(i);
        } else if (are_connected({i}, {(int)P.front()})) {
            P.push_front(i);
        } else if (!Q.empty() && are_connected({i}, {(int)Q.back()})) {
            Q.push_back(i);
        } else if (!Q.empty() && are_connected({i}, {(int)Q.front()})) {
            Q.push_front(i);
        } else {
            // D >= 1 on triplet {i, P.back(), Q.back()}: i-P.back() = false,
            // i-Q.back() = false => P.back()-Q.back() must be an edge.
            while (!Q.empty()) {
                P.push_back(Q.back());
                Q.pop_back();
            }
            Q.push_back(i);
        }
    }

    if (Q.empty())
        return vector<int>(P.begin(), P.end());

    // Phase 2: Merge P and Q via endpoint connections.
    if (are_connected({(int)P.back()}, {(int)Q.front()})) {
        for (auto x : Q) P.push_back(x);
        return vector<int>(P.begin(), P.end());
    }
    if (are_connected({(int)P.back()}, {(int)Q.back()})) {
        while (!Q.empty()) { P.push_back(Q.back()); Q.pop_back(); }
        return vector<int>(P.begin(), P.end());
    }
    if (are_connected({(int)P.front()}, {(int)Q.front()})) {
        vector<int> result;
        while (!P.empty()) { result.push_back(P.back()); P.pop_back(); }
        for (auto x : Q) result.push_back(x);
        return result;
    }
    if (are_connected({(int)P.front()}, {(int)Q.back()})) {
        while (!Q.empty()) { P.push_front(Q.back()); Q.pop_back(); }
        return vector<int>(P.begin(), P.end());
    }

    // Phase 3: No endpoint-endpoint edge. Binary search for an internal edge.
    vector<int> pv(P.begin(), P.end()), qv(Q.begin(), Q.end());
    if (!are_connected(pv, qv)) {
        // Disconnected components; return the longer path.
        return (pv.size() >= qv.size()) ? pv : qv;
    }

    // Binary search on P to find p_idx where pv[p_idx] connects to some node in Q.
    int lo = 0, hi = (int)pv.size() - 1;
    while (lo < hi) {
        int mid = (lo + hi) / 2;
        vector<int> sub(pv.begin() + lo, pv.begin() + mid + 1);
        if (are_connected(sub, qv))
            hi = mid;
        else
            lo = mid + 1;
    }
    int p_idx = lo;

    // Binary search on Q to find q_idx where qv[q_idx] connects to pv[p_idx].
    lo = 0; hi = (int)qv.size() - 1;
    while (lo < hi) {
        int mid = (lo + hi) / 2;
        vector<int> sub(qv.begin() + lo, qv.begin() + mid + 1);
        if (are_connected({pv[p_idx]}, sub))
            hi = mid;
        else
            lo = mid + 1;
    }
    int q_idx = lo;

    // We have edge pv[p_idx] -- qv[q_idx].
    // P is a path: pv[0]-...-pv[end]. Splitting at p_idx gives two sub-paths:
    //   P_left  = pv[0..p_idx]     (size p_idx + 1)
    //   P_right = pv[p_idx..end]   (size |P| - p_idx)
    // Similarly Q splits at q_idx:
    //   Q_left  = qv[0..q_idx]     (size q_idx + 1)
    //   Q_right = qv[q_idx..end]   (size |Q| - q_idx)
    //
    // We can form 4 valid paths through the bridge edge pv[p_idx]-qv[q_idx]:
    //   1. P_left  fwd + Q_right fwd:  pv[0]...pv[p_idx]-qv[q_idx]...qv[end]
    //   2. P_left  fwd + Q_left  rev:  pv[0]...pv[p_idx]-qv[q_idx]...qv[0]
    //   3. P_right rev + Q_right fwd:  pv[end]...pv[p_idx]-qv[q_idx]...qv[end]
    //   4. P_right rev + Q_left  rev:  pv[end]...pv[p_idx]-qv[q_idx]...qv[0]
    //
    // Sizes: pick the longer P half + the longer Q half.
    // Best = max(p_idx+1, |P|-p_idx) + max(q_idx+1, |Q|-q_idx)

    int pn = (int)pv.size(), qn = (int)qv.size();
    bool use_p_left = (p_idx + 1 >= pn - p_idx); // true => use P_left, false => P_right
    bool use_q_right = (qn - q_idx >= q_idx + 1); // true => use Q_right, false => Q_left

    vector<int> result;
    if (use_p_left) {
        // P_left forward: pv[0]...pv[p_idx]
        for (int i = 0; i <= p_idx; i++) result.push_back(pv[i]);
    } else {
        // P_right reversed: pv[end]...pv[p_idx]
        for (int i = pn - 1; i >= p_idx; i--) result.push_back(pv[i]);
    }
    if (use_q_right) {
        // Q_right forward: qv[q_idx]...qv[end]
        for (int i = q_idx; i < qn; i++) result.push_back(qv[i]);
    } else {
        // Q_left reversed: qv[q_idx]...qv[0]
        for (int i = q_idx; i >= 0; i--) result.push_back(qv[i]);
    }
    return result;
}

// The grader provides are_connected() and calls longest_trip().
// No main() is needed for submission.
#ifdef LOCAL
int main() {
    return 0;
}
#endif
