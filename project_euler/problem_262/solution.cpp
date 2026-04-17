#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 262: Mountain Range
 *
 * h(x,y) = (5000 - 0.005*(x^2+y^2+x*y) + 12.5*(x+y))
 *         * exp(-|0.000001*(x^2+y^2) - 0.0015*(x+y) + 0.7|)
 *
 * Find f_min (minimax elevation A(200,200) to B(1400,1400)),
 * then shortest path at f_min through the pinch point.
 *
 * The barrier {h > f_min} is ring-shaped with a pinch at (895.483, 0).
 * f_min = max h(x,0) = h(895.483, 0) ≈ 10396.462.
 *
 * Path: A -> PA(tangent) -> arc to pinch -> arc to Q(tangent) -> B
 * PA ≈ (624.650, 48.254), Q ≈ (1536.043, 873.038)
 *
 * Approach: numerical contour tracing and tangent finding.
 *
 * Answer: 2531.205
 */

double h_func(double x, double y) {
    double t1 = x*x + y*y + x*y;
    double t2 = x*x + y*y;
    double base = 5000.0 - 0.005*t1 + 12.5*(x+y);
    double ex = fabs(0.000001*t2 - 0.0015*(x+y) + 0.7);
    return base * exp(-ex);
}

double dh_dx(double x, double y) {
    const double eps = 1e-7;
    return (h_func(x+eps, y) - h_func(x-eps, y)) / (2*eps);
}

double dh_dy(double x, double y) {
    const double eps = 1e-7;
    return (h_func(x, y+eps) - h_func(x, y-eps)) / (2*eps);
}

// Find x on contour h=f at given y, scanning from left or right
double find_contour_x(double y, double f, bool from_left) {
    double prev = h_func(from_left ? 0 : 1600, y) - f;
    int start = from_left ? 1 : 1599;
    int end = from_left ? 1601 : -1;
    int step = from_left ? 1 : -1;
    for (int xi = start; xi != end; xi += step) {
        double cur = h_func(xi, y) - f;
        if (prev * cur < 0) {
            // Bisect
            double lo = from_left ? xi-1.0 : (double)xi;
            double hi = from_left ? (double)xi : xi+1.0;
            for (int i = 0; i < 60; i++) {
                double mid = (lo+hi)/2;
                if ((h_func(mid, y) - f) * (h_func(lo, y) - f) <= 0)
                    hi = mid;
                else
                    lo = mid;
            }
            return (lo+hi)/2;
        }
        prev = cur;
    }
    return -1;
}

double trace_arc(double x0, double y0, double x1, double y1, double f, double step_size=0.05) {
    double x = x0, y = y0;
    double total = 0;
    for (int i = 0; i < 500000; i++) {
        double gx = dh_dx(x, y);
        double gy = dh_dy(x, y);
        double t1x = -gy, t1y = gx;
        double t2x = gy, t2y = -gx;
        double dx = x1-x, dy = y1-y;
        double tx, ty;
        if (t1x*dx + t1y*dy > t2x*dx + t2y*dy)
            tx = t1x, ty = t1y;
        else
            tx = t2x, ty = t2y;
        double tn = sqrt(tx*tx + ty*ty);
        if (tn < 1e-12) break;
        tx /= tn; ty /= tn;
        double nx = x + step_size*tx;
        double ny = y + step_size*ty;
        // Project to contour
        for (int j = 0; j < 5; j++) {
            double hv = h_func(nx, ny);
            double gx2 = dh_dx(nx, ny);
            double gy2 = dh_dy(nx, ny);
            double g2 = gx2*gx2 + gy2*gy2;
            if (g2 < 1e-20) break;
            double corr = (hv - f) / g2;
            nx -= corr * gx2;
            ny -= corr * gy2;
        }
        total += sqrt((nx-x)*(nx-x) + (ny-y)*(ny-y));
        x = nx; y = ny;
        double d = sqrt((x-x1)*(x-x1) + (y-y1)*(y-y1));
        if (d < step_size * 2) {
            total += d;
            break;
        }
    }
    return total;
}

int main() {
    // Find f_min: max of h(x,0)
    double best_x = 895, best_h = h_func(895, 0);
    for (double x = 890; x <= 900; x += 0.001) {
        double hv = h_func(x, 0);
        if (hv > best_h) { best_h = hv; best_x = x; }
    }
    double f_min = best_h;
    double pinch_x = best_x;
    fprintf(stderr, "f_min = %.6f at x = %.6f\n", f_min, pinch_x);

    // Find tangent PA from A(200,200) on left branch
    // Binary search on y where tangent residual changes sign
    auto tangent_res_A = [&](double y) {
        double xl = find_contour_x(y, f_min, true);
        if (xl < 0) return 1e10;
        double gx = dh_dx(xl, y);
        double gy = dh_dy(xl, y);
        return gx*(xl-200) + gy*(y-200);
    };

    double lo = 40, hi = 50;
    for (int i = 0; i < 60; i++) {
        double mid = (lo+hi)/2;
        if (tangent_res_A(mid) < 0) lo = mid; else hi = mid;
    }
    double y_PA = (lo+hi)/2;
    double x_PA = find_contour_x(y_PA, f_min, true);
    fprintf(stderr, "PA = (%.6f, %.6f)\n", x_PA, y_PA);

    // Find tangent Q from B(1400,1400) on right branch
    auto tangent_res_B = [&](double y) {
        double xr = find_contour_x(y, f_min, false);
        if (xr < 0) return 1e10;
        double gx = dh_dx(xr, y);
        double gy = dh_dy(xr, y);
        return gx*(xr-1400) + gy*(y-1400);
    };

    lo = 870; hi = 875;
    for (int i = 0; i < 60; i++) {
        double mid = (lo+hi)/2;
        if (tangent_res_B(mid) < 0) lo = mid; else hi = mid;
    }
    double y_Q = (lo+hi)/2;
    double x_Q = find_contour_x(y_Q, f_min, false);
    fprintf(stderr, "Q = (%.6f, %.6f)\n", x_Q, y_Q);

    // Compute path
    double d_APA = sqrt((200-x_PA)*(200-x_PA) + (200-y_PA)*(200-y_PA));
    double arc1 = trace_arc(x_PA, y_PA, pinch_x, 0, f_min);
    double arc2 = trace_arc(pinch_x, 0, x_Q, y_Q, f_min);
    double d_QB = sqrt((x_Q-1400)*(x_Q-1400) + (y_Q-1400)*(y_Q-1400));

    double total = d_APA + arc1 + arc2 + d_QB;
    fprintf(stderr, "d(A,PA)=%.3f + arc1=%.3f + arc2=%.3f + d(Q,B)=%.3f = %.3f\n",
            d_APA, arc1, arc2, d_QB, total);
    printf("%.3f\n", total);
    return 0;
}
