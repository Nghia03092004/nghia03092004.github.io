#include <bits/stdc++.h>
using namespace std;

int main(){
    // Problem 83: Path Sum Four Ways
    // Dijkstra's algorithm on a grid, moving in all 4 directions.

    int n = 80;
    vector<vector<long long>> M(n, vector<long long>(n));

    ifstream fin("p083_matrix.txt");
    if(fin.is_open()){
        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
                fin >> M[i][j];
                char ch;
                if(j < n-1) fin >> ch;
            }
        }
        fin.close();
    } else {
        n = 5;
        M = {
            {131, 673, 234, 103, 18},
            {201, 96, 342, 965, 150},
            {630, 803, 746, 422, 111},
            {537, 699, 497, 121, 956},
            {805, 732, 524, 37, 331}
        };
        // Expected for 5x5: 2297
    }

    const long long INF = 1e18;
    vector<vector<long long>> dist(n, vector<long long>(n, INF));
    // min-heap: (distance, row, col)
    priority_queue<tuple<long long,int,int>,
                   vector<tuple<long long,int,int>>,
                   greater<>> pq;

    dist[0][0] = M[0][0];
    pq.push({M[0][0], 0, 0});

    int dx[] = {-1, 1, 0, 0};
    int dy[] = {0, 0, -1, 1};

    while(!pq.empty()){
        auto [d, x, y] = pq.top(); pq.pop();
        if(d > dist[x][y]) continue;
        if(x == n-1 && y == n-1){
            cout << d << endl;
            return 0;
        }
        for(int k = 0; k < 4; k++){
            int nx = x + dx[k], ny = y + dy[k];
            if(nx < 0 || nx >= n || ny < 0 || ny >= n) continue;
            long long nd = d + M[nx][ny];
            if(nd < dist[nx][ny]){
                dist[nx][ny] = nd;
                pq.push({nd, nx, ny});
            }
        }
    }

    cout << dist[n-1][n-1] << endl;
    // Full 80x80 answer: 425185

    return 0;
}
