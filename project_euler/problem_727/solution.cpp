#include <bits/stdc++.h>
using namespace std;

// Problem 727: Triangle of Circular Arcs
// Compute E(d) for triples (ra, rb, rc) with 1<=ra<rb<rc<=100, gcd=1

typedef long long ll;

int gcd3(int a, int b, int c) {
    return __gcd(a, __gcd(b, c));
}

struct Point {
    double x, y;
    Point(double x=0, double y=0): x(x), y(y) {}
    Point operator+(const Point& p) const { return {x+p.x, y+p.y}; }
    Point operator-(const Point& p) const { return {x-p.x, y-p.y}; }
    Point operator*(double t) const { return {x*t, y*t}; }
    double norm() const { return sqrt(x*x + y*y); }
};

// Circumcenter of triangle
Point circumcenter(Point A, Point B, Point C) {
    double ax = A.x, ay = A.y;
    double bx = B.x, by = B.y;
    double cx = C.x, cy = C.y;
    double D = 2 * (ax*(by-cy) + bx*(cy-ay) + cx*(ay-by));
    if (fabs(D) < 1e-15) return {0, 0};
    double ux = ((ax*ax+ay*ay)*(by-cy) + (bx*bx+by*by)*(cy-ay) + (cx*cx+cy*cy)*(ay-by)) / D;
    double uy = ((ax*ax+ay*ay)*(cx-bx) + (bx*bx+by*by)*(ax-cx) + (cx*cx+cy*cy)*(bx-ax)) / D;
    return {ux, uy};
}

// Compute the inner Soddy circle center using complex Descartes theorem
// Centers are at complex positions z_a, z_b, z_c with curvatures k_a, k_b, k_c
pair<double,double> soddy_center(double ka, double kb, double kc,
                                   double xa, double ya,
                                   double xb, double yb,
                                   double xc, double yc) {
    double ks = ka + kb + kc + 2*sqrt(ka*kb + kb*kc + kc*ka);

    // Complex Descartes: ks*zs = ka*za + kb*zb + kc*zc + 2*sqrt(ka*kb*za*zb + ...)
    // Using real and imaginary parts
    // We need: sqrt(ka*kb*za*zb + kb*kc*zb*zc + kc*ka*zc*za)
    // where za = xa + i*ya, etc.

    // Compute w = ka*kb*za*zb + kb*kc*zb*zc + kc*ka*zc*za
    double w_real = ka*kb*(xa*xb - ya*yb) + kb*kc*(xb*xc - yb*yc) + kc*ka*(xc*xa - yc*ya);
    double w_imag = ka*kb*(xa*yb + ya*xb) + kb*kc*(xb*yc + yb*xc) + kc*ka*(xc*ya + yc*xa);

    // sqrt(w) = sqrt(|w|) * (cos(arg/2) + i*sin(arg/2))
    double w_abs = sqrt(w_real*w_real + w_imag*w_imag);
    double w_arg = atan2(w_imag, w_real);
    double sq_abs = sqrt(w_abs);
    double sq_real = sq_abs * cos(w_arg/2);
    double sq_imag = sq_abs * sin(w_arg/2);

    double num_real = ka*xa + kb*xb + kc*xc + 2*sq_real;
    double num_imag = ka*ya + kb*yb + kc*yc + 2*sq_imag;

    return {num_real / ks, num_imag / ks};
}

int main() {
    double total_d = 0;
    int count = 0;

    for (int ra = 1; ra <= 100; ra++) {
        for (int rb = ra+1; rb <= 100; rb++) {
            for (int rc = rb+1; rc <= 100; rc++) {
                if (gcd3(ra, rb, rc) != 1) continue;
                count++;

                // Place circles
                double Oax = 0, Oay = 0;
                double Obx = ra + rb, Oby = 0;
                double d_ac = ra + rc;
                double d_bc = rb + rc;
                double d_ab = ra + rb;
                double Ocx = (d_ac*d_ac - d_bc*d_bc + d_ab*d_ab) / (2.0 * d_ab);
                double Ocy = sqrt(max(0.0, d_ac*d_ac - Ocx*Ocx));

                // Tangency points
                Point Tab = Point(Oax, Oay) * ((double)rb/(ra+rb)) + Point(Obx, Oby) * ((double)ra/(ra+rb));
                Point Tac = Point(Oax, Oay) * ((double)rc/(ra+rc)) + Point(Ocx, Ocy) * ((double)ra/(ra+rc));
                Point Tbc = Point(Obx, Oby) * ((double)rc/(rb+rc)) + Point(Ocx, Ocy) * ((double)rb/(rb+rc));

                // Circumcenter
                Point D = circumcenter(Tab, Tac, Tbc);

                // Inner Soddy circle center
                double ka = 1.0/ra, kb = 1.0/rb, kc = 1.0/rc;
                auto [Ex, Ey] = soddy_center(ka, kb, kc, Oax, Oay, Obx, Oby, Ocx, Ocy);

                double dist = sqrt((D.x-Ex)*(D.x-Ex) + (D.y-Ey)*(D.y-Ey));
                total_d += dist;
            }
        }
    }

    printf("Count of valid triples: %d\n", count);
    printf("E(d) = %.8f\n", total_d / count);

    return 0;
}
