#include <bits/stdc++.h>
using namespace std;

int main() {
    const int R = 30, C = 30;
    const int N = R * C;
    const int ROUNDS = 50;

    auto idx = [&](int r, int c) { return r * C + c; };

    int dr[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};

    vector<int> deg(N);
    for (int r = 0; r < R; r++)
        for (int c = 0; c < C; c++) {
            int d = 0;
            for (int k = 0; k < 4; k++)
                if (r + dr[k] >= 0 && r + dr[k] < R && c + dc[k] >= 0 && c + dc[k] < C)
                    d++;
            deg[idx(r, c)] = d;
        }

    vector<double> prod_not(N, 1.0);

    for (int sr = 0; sr < R; sr++) {
        for (int sc = 0; sc < C; sc++) {
            vector<double> cur(N, 0.0), nxt(N, 0.0);
            cur[idx(sr, sc)] = 1.0;

            for (int t = 0; t < ROUNDS; t++) {
                fill(nxt.begin(), nxt.end(), 0.0);
                for (int r = 0; r < R; r++)
                    for (int c = 0; c < C; c++) {
                        int i = idx(r, c);
                        if (cur[i] == 0.0) continue;
                        double p = cur[i] / deg[i];
                        for (int k = 0; k < 4; k++) {
                            int nr = r + dr[k], nc = c + dc[k];
                            if (nr >= 0 && nr < R && nc >= 0 && nc < C)
                                nxt[idx(nr, nc)] += p;
                        }
                    }
                swap(cur, nxt);
            }

            for (int i = 0; i < N; i++)
                prod_not[i] *= (1.0 - cur[i]);
        }
    }

    double answer = 0.0;
    for (int i = 0; i < N; i++)
        answer += prod_not[i];

    cout << fixed << setprecision(6) << answer << endl;
    return 0;
}
