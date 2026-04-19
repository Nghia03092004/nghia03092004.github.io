#include <bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    double R;
    cin >> N >> R;

    vector<double> x(N), y(N), t(N);
    for(int i = 0; i < N; i++){
        cin >> x[i] >> y[i] >> t[i];
    }

    double bestAvg = -1e18;
    double bestCx = 0, bestCy = 0;

    // For each point as center, compute average
    auto evalCenter = [&](double cx, double cy) {
        double sumT = 0;
        int cnt = 0;
        for(int i = 0; i < N; i++){
            double dx = x[i] - cx, dy = y[i] - cy;
            if(dx*dx + dy*dy <= R*R + 1e-9){
                sumT += t[i];
                cnt++;
            }
        }
        if(cnt > 0){
            double avg = sumT / cnt;
            if(avg > bestAvg){
                bestAvg = avg;
                bestCx = cx; bestCy = cy;
            }
        }
    };

    // Try each point as center
    for(int i = 0; i < N; i++){
        evalCenter(x[i], y[i]);
    }

    // Try each pair of points on boundary
    for(int i = 0; i < N; i++){
        for(int j = i + 1; j < N; j++){
            double dx = x[j] - x[i], dy = y[j] - y[i];
            double d2 = dx*dx + dy*dy;
            if(d2 > 4*R*R + 1e-9) continue;

            double mx = (x[i] + x[j]) / 2;
            double my = (y[i] + y[j]) / 2;
            double h = sqrt(max(0.0, R*R - d2/4));

            // Perpendicular direction
            double px = -dy, py = dx;
            double len = sqrt(px*px + py*py);
            if(len < 1e-12) continue;
            px /= len; py /= len;

            // Two candidate centers
            evalCenter(mx + h * px, my + h * py);
            evalCenter(mx - h * px, my - h * py);
        }
    }

    cout << fixed << setprecision(6) << bestCx << " " << bestCy << "\n";
    return 0;
}
