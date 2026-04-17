#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 607: Marsh Crossing
 *
 * Minimize travel time across 7 regions with different speeds.
 * The marsh runs SW-NE (at 45 degrees) crossing the east-west path AB.
 *
 * Key insight: The path through each constant-speed region is a straight line.
 * We parameterize by the y-offset at each of the 6 boundary crossings.
 *
 * The geometry: A and B are 100 leagues apart, due east.
 * The marsh is 50 leagues wide perpendicular to its extent (SW-NE).
 * The midpoint of AB is at the center of the marsh.
 *
 * In the rotated frame (45 deg), the marsh boundaries are vertical.
 * The perpendicular width of each strip is 10 leagues, so in the
 * rotated frame each strip has x-width = 10/sqrt(2).
 *
 * But we can also work directly: The path from A to B can be parameterized
 * by the perpendicular offsets at each marsh boundary.
 */

const double S2 = sqrt(2.0);
const double INF_VAL = 1e18;

// The marsh is perpendicular to its SW-NE direction.
// Width of each strip measured perpendicular to the marsh direction = 10 leagues
// In the east direction, the strip boundaries are at specific positions.
//
// Let's set up coordinates with A at origin, B at (100, 0).
// The marsh runs SW to NE. Its center passes through (50, 0).
// The marsh direction is at 45 degrees (NE direction): unit vector (1/sqrt2, 1/sqrt2)
// The perpendicular to the marsh: (1/sqrt2, -1/sqrt2)
//
// The marsh boundaries (perpendicular distance from center):
// -25, -15, -5, 5, 15, 25 leagues from center
// The center of the marsh is at point (50, 0).
//
// A point (x, y) has perpendicular distance to center:
// d = ((x-50)/sqrt2 - y/sqrt2) ... actually let's think more carefully.
//
// The marsh runs along direction (1,1)/sqrt2. Its perpendicular is (1,-1)/sqrt2.
// The signed perpendicular distance from (50,0) of point (x,y) is:
// d = ((x-50) - y) / sqrt(2)   [using normal (1,-1)/sqrt2]
//
// Wait, the perpendicular to (1,1) is (1,-1). So the signed distance is:
// d = ((x-50)*1 + (y-0)*(-1)) / sqrt(2) = (x - 50 - y) / sqrt(2)
//
// The marsh extends from d = -25 to d = 25.
// Strip boundaries at d = -25, -15, -5, 5, 15, 25.
//
// For the path, we parameterize by the position along the marsh direction
// at each boundary. Let t_i be the component along the marsh direction
// (1,1)/sqrt(2) at boundary i.
//
// At boundary i with perpendicular distance d_i from center:
//   x - 50 - y = d_i * sqrt(2)
//   x + y = t_i * sqrt(2)   (position along marsh)
// So x = 50 + (d_i + t_i)/sqrt(2) * sqrt(2)/2 ... let me just parameterize differently.
//
// Actually let's use the approach: parameterize by the y-coordinate where
// the path crosses each marsh boundary.
//
// Boundary i is the locus of points where (x - 50 - y)/sqrt(2) = d_i
// i.e., x = 50 + y + d_i * sqrt(2)
//
// For a crossing at height y, the x-coordinate is:
//   x = 50 + y + d_i * sqrt(2)

double d_boundaries[6] = {-25, -15, -5, 5, 15, 25};
double speeds[7] = {10, 9, 8, 7, 6, 5, 10};

// Point A = (0, 0), point B = (100, 0)
// Crossing boundary i at height h_i means point (50 + h_i + d_i*sqrt(2), h_i)
// But A is at perpendicular distance (0 - 50 - 0)/sqrt(2) = -50/sqrt(2) ≈ -35.36
// which is < -25, so A is before the marsh. Good.
// B is at perpendicular distance (100 - 50 - 0)/sqrt(2) = 50/sqrt(2) ≈ 35.36
// which is > 25, so B is after the marsh. Good.

double compute_time(double h[6]) {
    // Compute points: A, boundary crossings, B
    // A = (0, 0)
    // Boundary i crossing at height h[i]: position = (50 + h[i] + d_boundaries[i]*S2, h[i])
    // B = (100, 0)

    double px[8], py[8];
    px[0] = 0; py[0] = 0;  // A
    for (int i = 0; i < 6; i++) {
        px[i+1] = 50.0 + h[i] + d_boundaries[i] * S2;
        py[i+1] = h[i];
    }
    px[7] = 100.0; py[7] = 0;  // B

    double total = 0;
    for (int i = 0; i < 7; i++) {
        double dx = px[i+1] - px[i];
        double dy = py[i+1] - py[i];
        double dist = sqrt(dx*dx + dy*dy);
        total += dist / speeds[i];
    }
    return total;
}

int main() {
    // Optimize using gradient descent with adaptive step size
    double h[6];
    // Initialize to 0 (straight path)
    for (int i = 0; i < 6; i++) h[i] = 0;

    double lr = 0.1;
    double best_time = compute_time(h);

    for (int outer = 0; outer < 100; outer++) {
        for (int iter = 0; iter < 100000; iter++) {
            double grad[6];
            double eps = 1e-10;
            for (int i = 0; i < 6; i++) {
                h[i] += eps;
                double tp = compute_time(h);
                h[i] -= 2*eps;
                double tm = compute_time(h);
                h[i] += eps;
                grad[i] = (tp - tm) / (2*eps);
            }

            double h_new[6];
            for (int i = 0; i < 6; i++) {
                h_new[i] = h[i] - lr * grad[i];
            }
            double t_new = compute_time(h_new);

            if (t_new < best_time) {
                for (int i = 0; i < 6; i++) h[i] = h_new[i];
                best_time = t_new;
            } else {
                lr *= 0.5;
                if (lr < 1e-18) break;
            }
        }
        if (lr < 1e-18) break;
    }

    printf("%.10f\n", best_time);
    return 0;
}
