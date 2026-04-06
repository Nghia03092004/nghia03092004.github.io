#include <bits/stdc++.h>
using namespace std;

// Compute area of triangle with sides a, b, c using Heron's formula
double triangleArea(double a, double b, double c) {
    double s = (a + b + c) / 2.0;
    return sqrt(s * (s - a) * (s - b) * (s - c));
}

// Compute angles of triangle
void triangleAngles(double a, double b, double c, double &A, double &B, double &C) {
    A = acos((b*b + c*c - a*a) / (2.0*b*c));
    B = acos((a*a + c*c - b*b) / (2.0*a*c));
    C = M_PI - A - B;
}

// Malfatti circle radii using Steiner's formula
void malfattiRadii(double a, double b, double c, double &r1, double &r2, double &r3) {
    double s = (a + b + c) / 2.0;
    double area = sqrt(s * (s-a) * (s-b) * (s-c));
    double r = area / s; // inradius

    double A, B, C;
    triangleAngles(a, b, c, A, B, C);

    double tA = tan(A/2), tB = tan(B/2), tC = tan(C/2);

    // Malfatti radii (Steiner's formulas)
    double d = 1 + tA + tB + tC;
    r1 = r / (1.0 + tA) * (1.0 - (tB + tC - tA + 1.0) / d) / 2.0;
    r2 = r / (1.0 + tB) * (1.0 - (tA + tC - tB + 1.0) / d) / 2.0;
    r3 = r / (1.0 + tC) * (1.0 - (tA + tB - tC + 1.0) / d) / 2.0;

    // Use more standard Malfatti formulas
    // r_i = r * (1 - tan(Aj/2)) * (1 - tan(Ak/2)) / (2 * (1 + tan(Ai/2)))
    // Actually use the correct Schellbach/Steiner formulas:
    double IBC = 2*r / (1 - tA + tB + tC);
    double IAC = 2*r / (1 + tA - tB + tC);
    double IAB = 2*r / (1 + tA + tB - tC);

    // Correct Malfatti circle radii
    r1 = r * (tA) / (1 + tA);
    r2 = r * (tB) / (1 + tB);
    r3 = r * (tC) / (1 + tC);

    // Actually the standard formulas are more complex. Let's use the known parameterization.
    // For Malfatti circles with half-angle tangents t_i = tan(A_i/2):
    // Let D = 1 + t1 + t2 + t3
    // r_i = r / D * (1 + t_i) * ...

    // Use Goldberg's formulation: Malfatti radii
    double x = r * (1.0/(cos((B-C)/2) + 1.0/(2*cos(A/2))));

    // Simplified: just compute via the iterative Descartes approach or direct formula
    // For correctness, use the known exact formula:
    double sA = sin(A/2), sB = sin(B/2), sC = sin(C/2);
    double cA = cos(A/2), cB = cos(B/2), cC = cos(C/2);

    // Malfatti radii (correct form):
    r1 = r / (2*cA*cA) * (cA + sB + sC - 1);
    r2 = r / (2*cB*cB) * (cB + sA + sC - 1);
    r3 = r / (2*cC*cC) * (cC + sA + sB - 1);
}

// Greedy packing: incircle + 2 largest corner circles
double greedyPacking(double a, double b, double c) {
    double s = (a + b + c) / 2.0;
    double area = sqrt(s * (s-a) * (s-b) * (s-c));
    double r = area / s;

    double A, B, C;
    triangleAngles(a, b, c, A, B, C);

    // Corner circle radii (circle fitting in corner between incircle and two sides)
    double rA = r * tan(A/2) / (1.0 + 1.0/sin(A/2));
    double rB = r * tan(B/2) / (1.0 + 1.0/sin(B/2));
    double rC = r * tan(C/2) / (1.0 + 1.0/sin(C/2));

    // Sort corner radii descending
    double corners[3] = {rA, rB, rC};
    sort(corners, corners + 3, greater<double>());

    // Greedy: incircle + two largest corners
    return M_PI * (r*r + corners[0]*corners[0] + corners[1]*corners[1]);
}

double malfattiArea(double a, double b, double c) {
    double r1, r2, r3;
    malfattiRadii(a, b, c, r1, r2, r3);
    return M_PI * (r1*r1 + r2*r2 + r3*r3);
}

int main() {
    int N = 1803;
    double totalR = 0;
    long long count = 0;

    for (int a = 1; a <= N; a++) {
        for (int b = a; b <= N; b++) {
            // c >= b and c < a+b and a+b <= N => c < a+b and c <= N and c >= b
            // also a + b <= N
            if (a + b > N) break;
            int cmin = b;
            int cmax = min(a + b - 1, N); // c < a+b means c <= a+b-1; also c <= N
            // but we also need a + b <= N (from the constraint c < a+b <= n)
            // Wait: the constraint is 1 <= a <= b <= c < a+b <= n
            // So c < a+b AND a+b <= n
            // So a+b <= N, and c < a+b, and c >= b
            for (int c = cmin; c <= cmax; c++) {
                double da = a, db = b, dc = c;
                double mArea = malfattiArea(da, db, dc);
                double gArea = greedyPacking(da, db, dc);
                totalR += max(mArea, gArea);
                count++;
            }
        }
    }

    double S = totalR / count;
    printf("%.5f\n", S);
    return 0;
}
