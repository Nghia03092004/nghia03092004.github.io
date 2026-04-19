// IOI 1990 - Problem 3: Festival Decoration
// Arrange colored balls so no two adjacent balls share the same color.
// Greedy with max-heap: always place the most frequent available color.
#include <bits/stdc++.h>
using namespace std;

int main() {
    int k;
    scanf("%d", &k);

    priority_queue<pair<int,int>> pq;
    int total = 0;

    for (int i = 0; i < k; i++) {
        int c;
        scanf("%d", &c);
        if (c > 0) {
            pq.push({c, i});
            total += c;
        }
    }

    if (pq.empty()) { printf("\n"); return 0; }

    // Check feasibility: max count must be <= ceil(total/2)
    if (pq.top().first > (total + 1) / 2) {
        printf("IMPOSSIBLE\n");
        return 0;
    }

    string result;
    result.reserve(total);
    int lastColor = -1;

    while (!pq.empty()) {
        auto [cnt1, col1] = pq.top();
        pq.pop();

        if (col1 != lastColor) {
            result += (char)('A' + col1);
            lastColor = col1;
            if (cnt1 > 1) pq.push({cnt1 - 1, col1});
        } else {
            if (pq.empty()) {
                printf("IMPOSSIBLE\n");
                return 0;
            }
            auto [cnt2, col2] = pq.top();
            pq.pop();

            result += (char)('A' + col2);
            lastColor = col2;
            pq.push({cnt1, col1});
            if (cnt2 > 1) pq.push({cnt2 - 1, col2});
        }
    }

    printf("%s\n", result.c_str());
    return 0;
}
