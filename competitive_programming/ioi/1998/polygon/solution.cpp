// IOI 1998 - Polygon
// Interval DP on doubled circular array with min/max tracking
// Operations: 't' (add), 'x' (multiply); values can be negative
// Time: O(n^3), Space: O(n^2)
#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
const ll NEG_INF = -1e18;
const ll POS_INF = 1e18;

int n;
ll val[105];   // vertex values (doubled)
char op[105];  // edge operations (doubled): 't' for +, 'x' for *

ll maxDP[105][105], minDP[105][105];

int main() {
    scanf("%d", &n);

    // Input format: edge_op vertex_val for each vertex
    // Edge i connects vertex i-1 and vertex i (cyclically)
    for (int i = 0; i < n; i++) {
        char c;
        int v;
        scanf(" %c %d", &c, &v);
        op[i] = c;
        val[i] = v;
    }

    // Double the arrays for circular handling
    for (int i = 0; i < n; i++) {
        val[i + n] = val[i];
        op[i + n] = op[i];
    }

    int N = 2 * n;

    // Initialize DP
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++) {
            maxDP[i][j] = NEG_INF;
            minDP[i][j] = POS_INF;
        }

    // Base case: single vertices
    for (int i = 0; i < N; i++) {
        maxDP[i][i] = val[i];
        minDP[i][i] = val[i];
    }

    // Fill DP for increasing interval lengths
    for (int len = 2; len <= n; len++) {
        for (int i = 0; i + len - 1 < N; i++) {
            int j = i + len - 1;
            for (int k = i; k < j; k++) {
                // Edge between vertex k and vertex k+1 in the doubled array
                // op[k+1] is the edge leading into vertex k+1
                // In the original problem, edge[i] is between vertex[i-1] and vertex[i]
                // So the edge between positions k and k+1 is op[(k+1) % n]
                char o = op[(k + 1) % n];

                if (o == 't') {
                    maxDP[i][j] = max(maxDP[i][j],
                        maxDP[i][k] + maxDP[k + 1][j]);
                    minDP[i][j] = min(minDP[i][j],
                        minDP[i][k] + minDP[k + 1][j]);
                } else { // multiplication
                    ll cands[4] = {
                        maxDP[i][k] * maxDP[k + 1][j],
                        maxDP[i][k] * minDP[k + 1][j],
                        minDP[i][k] * maxDP[k + 1][j],
                        minDP[i][k] * minDP[k + 1][j]
                    };
                    for (int c = 0; c < 4; c++) {
                        maxDP[i][j] = max(maxDP[i][j], cands[c]);
                        minDP[i][j] = min(minDP[i][j], cands[c]);
                    }
                }
            }
        }
    }

    // Find maximum over all intervals of length n (each starting position = removing that edge)
    ll ans = NEG_INF;
    for (int i = 0; i < n; i++)
        ans = max(ans, maxDP[i][i + n - 1]);

    printf("%lld\n", ans);

    // Print which starting positions (1-indexed) achieve the maximum
    for (int i = 0; i < n; i++)
        if (maxDP[i][i + n - 1] == ans)
            printf("%d ", i + 1);
    printf("\n");

    return 0;
}
