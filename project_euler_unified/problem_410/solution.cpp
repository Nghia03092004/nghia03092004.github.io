#include <bits/stdc++.h>
using namespace std;

/**
 * Problem 410: Circle and Tangent Line
 *
 * Given a circle centered at origin with radius r and external point P=(a,0),
 * compute tangent length, tangent points, tangent line equations, and enclosed area.
 */

struct Point {
    double x, y;
    Point(double x = 0, double y = 0) : x(x), y(y) {}
    double norm() const { return sqrt(x * x + y * y); }
};

struct TangentResult {
    double tangent_length;
    double half_angle_rad;
    double half_angle_deg;
    Point t1, t2;
    double enclosed_area;
    double power_of_point;
};

TangentResult solve(double a, double r) {
    assert(a > r && r > 0);
    TangentResult res;

    // Tangent length: sqrt(a^2 - r^2)
    res.tangent_length = sqrt(a * a - r * r);

    // Half-angle at P
    res.half_angle_rad = asin(r / a);
    res.half_angle_deg = res.half_angle_rad * 180.0 / M_PI;

    // Tangent points
    double tx = r * r / a;
    double ty = r * sqrt(a * a - r * r) / a;
    res.t1 = Point(tx, ty);
    res.t2 = Point(tx, -ty);

    // Enclosed area = L*r - r^2 * alpha
    res.enclosed_area = res.tangent_length * r - r * r * res.half_angle_rad;

    // Power of point
    res.power_of_point = a * a - r * r;

    return res;
}

void print_result(double a, double r) {
    TangentResult res = solve(a, r);

    cout << fixed << setprecision(6);
    cout << "Circle: center (0,0), radius r = " << r << endl;
    cout << "External point: P = (" << a << ", 0)" << endl;
    cout << string(50, '=') << endl;

    cout << "Tangent length:        L = " << res.tangent_length << endl;
    cout << "Half-angle:            alpha = "
         << setprecision(4) << res.half_angle_deg << " deg  ("
         << setprecision(6) << res.half_angle_rad << " rad)" << endl;

    cout << "Tangent point T1:      (" << res.t1.x << ", " << res.t1.y << ")" << endl;
    cout << "Tangent point T2:      (" << res.t2.x << ", " << res.t2.y << ")" << endl;

    // Tangent line equations: r*x +/- sqrt(a^2-r^2)*y = r*a
    double L = res.tangent_length;
    cout << setprecision(4);
    cout << "Tangent line 1:        " << r << "*x + " << L << "*y = " << r * a << endl;
    cout << "Tangent line 2:        " << r << "*x + " << -L << "*y = " << r * a << endl;

    cout << setprecision(6);
    cout << "Enclosed area:         A = " << res.enclosed_area << endl;
    cout << "Power of point:        " << res.power_of_point
         << "  (= L^2 = " << res.tangent_length * res.tangent_length << ")" << endl;

    // Verification: tangent points lie on circle
    cout << "Verification |OT1|:    " << res.t1.norm() << "  (should be " << r << ")" << endl;
    cout << "Verification |OT2|:    " << res.t2.norm() << "  (should be " << r << ")" << endl;

    // Verification: OT perpendicular to PT
    double dot1 = res.t1.x * (res.t1.x - a) + res.t1.y * res.t1.y;
    double dot2 = res.t2.x * (res.t2.x - a) + res.t2.y * res.t2.y;
    cout << "Verification OT.PT (T1): " << scientific << dot1 << "  (should be 0)" << endl;
    cout << "Verification OT.PT (T2): " << dot2 << "  (should be 0)" << endl;
    cout << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    cout << string(60, '=') << endl;
    cout << "  PROBLEM 410: CIRCLE AND TANGENT LINE" << endl;
    cout << string(60, '=') << endl << endl;

    // Primary example
    print_result(3.0, 1.0);

    cout << string(60, '=') << endl;
    cout << "Additional examples:" << endl;
    cout << string(60, '=') << endl << endl;

    // Additional examples
    vector<pair<double, double>> cases = {{5, 2}, {10, 1}, {2, 1}};
    for (auto& [a, r] : cases) {
        TangentResult res = solve(a, r);
        cout << fixed << setprecision(4);
        cout << "--- r=" << r << ", P=(" << a << ",0) ---" << endl;
        cout << "  L=" << res.tangent_length
             << ", alpha=" << res.half_angle_deg << " deg"
             << ", area=" << res.enclosed_area << endl;
        cout << "  T1=(" << res.t1.x << ", " << res.t1.y
             << "), T2=(" << res.t2.x << ", " << res.t2.y << ")" << endl;
        cout << endl;
    }

    return 0;
}
