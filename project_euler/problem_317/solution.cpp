#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 317: Firecracker
 *
 * V = pi * v0^2 * Z^2 / g
 * where Z = h0 + v0^2 / (2*g)
 *
 * h0 = 100, v0 = 20, g = 9.81
 */

int main(){
    double h0 = 100.0;
    double v0 = 20.0;
    double g = 9.81;

    double Z = h0 + v0 * v0 / (2.0 * g);
    double V = M_PI * v0 * v0 * Z * Z / g;

    printf("%.4f\n", V);
    return 0;
}
