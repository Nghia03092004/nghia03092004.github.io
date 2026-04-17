#include <bits/stdc++.h>
using namespace std;

int main() {
    vector<string> attempts = {
        "319", "680", "180", "690", "129", "620", "762", "689", "762", "318",
        "368", "710", "720", "710", "629", "168", "160", "689", "716", "731",
        "736", "729", "316", "729", "729", "710", "769", "290", "719", "680",
        "318", "389", "162", "289", "162", "718", "729", "319", "790", "680",
        "890", "362", "319", "760", "316", "729", "380", "319", "728", "716"
    };

    // Build precedence DAG
    set<int> digits;
    map<int, set<int>> adj;

    for (auto& s : attempts) {
        int a = s[0] - '0', b = s[1] - '0', c = s[2] - '0';
        digits.insert({a, b, c});
        adj[a].insert(b);
        adj[a].insert(c);
        adj[b].insert(c);
    }

    // Compute in-degrees from the adjacency sets
    map<int, int> indeg;
    for (int d : digits) indeg[d] = 0;
    for (int d : digits)
        for (int v : adj[d])
            indeg[v]++;

    // Kahn's algorithm (topological sort)
    queue<int> q;
    for (int d : digits)
        if (indeg[d] == 0)
            q.push(d);

    string result;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        result += ('0' + u);
        for (int v : adj[u]) {
            if (--indeg[v] == 0)
                q.push(v);
        }
    }

    cout << result << endl;
    return 0;
}
