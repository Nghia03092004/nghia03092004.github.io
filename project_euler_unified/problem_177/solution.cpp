#include <bits/stdc++.h>
using namespace std;

// Problem 177: Integer Angled Quadrilaterals
// Fixed range: alpha and zeta can go up to 179-p (not 178-p).
// Convexity requires A,B,C,D < 180, not the sub-angle pair sums.

int main() {
    double sinv[181];
    for (int i = 0; i <= 180; i++) sinv[i] = sin(i * M_PI / 180.0);

    typedef array<int,8> T8;

    int r_perm[8] = {3,2,4,5,6,7,1,0};
    int s_perm[8] = {1,0,7,6,5,4,3,2};

    int perms[8][8];
    for (int i = 0; i < 8; i++) perms[0][i] = i;
    for (int i = 0; i < 8; i++) perms[1][i] = r_perm[i];
    for (int k = 2; k <= 3; k++)
        for (int i = 0; i < 8; i++) perms[k][i] = perms[k-1][r_perm[i]];
    for (int i = 0; i < 8; i++) perms[4][i] = s_perm[i];
    for (int k = 5; k <= 7; k++)
        for (int i = 0; i < 8; i++) perms[k][i] = perms[4][perms[k-4][i]];

    auto canonical = [&](const T8& t) -> T8 {
        T8 best = t;
        for (int sym = 1; sym < 8; sym++) {
            T8 tt;
            for (int i = 0; i < 8; i++) tt[i] = t[perms[sym][i]];
            if (tt < best) best = tt;
        }
        return best;
    };

    set<T8> seen;

    for (int p = 2; p <= 178; p++) {
        double sp = sinv[p], cp = cos(p * M_PI / 180.0);
        int max_az = 179 - p;  // FIXED: was 178-p
        int max_d = p - 1;

        for (int alpha = 1; alpha <= max_az; alpha++) {
            int gamma = 180 - p - alpha;
            for (int delta = 1; delta <= max_d; delta++) {
                int epsilon = p - delta;
                for (int zeta = 1; zeta <= max_az; zeta++) {
                    int eta = 180 - p - zeta;

                    double L = sinv[alpha] * sinv[delta] * sinv[zeta];
                    double R = sinv[gamma] * sinv[epsilon] * sinv[eta];

                    double y = R * sp;
                    double x = L + R * cp;
                    double theta_exact = atan2(y, x) * 180.0 / M_PI;

                    if (theta_exact < 0.5 || theta_exact > max_d + 0.5) continue;

                    int t_round = (int)round(theta_exact);
                    for (int theta = max(1, t_round - 1); theta <= min(max_d, t_round + 1); theta++) {
                        int beta = p - theta;
                        if (beta < 1) continue;

                        double lhs = sinv[alpha] * sinv[delta] * sinv[zeta] * sinv[theta];
                        double rhs = sinv[beta] * sinv[gamma] * sinv[epsilon] * sinv[eta];
                        double rel = fabs(lhs - rhs) / max(fabs(lhs), fabs(rhs));
                        if (rel > 1e-9) continue;

                        // Convexity: quad angles < 180
                        int A = alpha + beta, B = gamma + delta, C = epsilon + zeta, D = eta + theta;
                        if (A >= 180 || B >= 180 || C >= 180 || D >= 180) continue;

                        T8 t = {alpha, beta, gamma, delta, epsilon, zeta, eta, theta};
                        seen.insert(canonical(t));
                    }
                }
            }
        }
    }

    cout << seen.size() << endl;
    return 0;
}
