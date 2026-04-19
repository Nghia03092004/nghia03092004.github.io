// IOI 1993 - Day 2, Task 1: Operations
// BFS to find minimum operations to transform start into target.
// Operations: +1, -1, *2, /2 (integer division).
#include <bits/stdc++.h>
using namespace std;

int main() {
    int start, target;
    scanf("%d%d", &start, &target);

    if (start == target) {
        printf("0\n");
        return 0;
    }

    unordered_map<int, int> dist;
    unordered_map<int, int> parent;
    unordered_map<int, string> op;

    queue<int> q;
    q.push(start);
    dist[start] = 0;

    // Search bounds
    int lo = min(0, min(start, target) - 1);
    int hi = max(start, target) * 2 + 2;

    while (!q.empty()) {
        int cur = q.front(); q.pop();

        int nexts[] = {cur + 1, cur - 1, cur * 2, cur / 2};
        const char* ops[] = {"+1", "-1", "*2", "/2"};
        int cnt = (cur != 0) ? 4 : 3;

        for (int i = 0; i < cnt; i++) {
            int nxt = nexts[i];
            if (nxt < lo || nxt > hi) continue;
            if (dist.count(nxt)) continue;

            dist[nxt] = dist[cur] + 1;
            parent[nxt] = cur;
            op[nxt] = ops[i];

            if (nxt == target) {
                printf("%d\n", dist[nxt]);
                vector<string> path;
                int x = target;
                while (x != start) {
                    path.push_back(op[x]);
                    x = parent[x];
                }
                reverse(path.begin(), path.end());
                for (auto& s : path) printf("%s ", s.c_str());
                printf("\n");
                return 0;
            }
            q.push(nxt);
        }
    }

    printf("-1\n");
    return 0;
}
