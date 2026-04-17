#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>

int cutoff_index_regular_polygon(int sides) {
    const double pi = std::acos(-1.0);
    const double theta = pi / static_cast<double>(sides);
    const double tangent = std::tan(theta);

    auto threshold_function = [&](int k) {
        const double angle = k * theta;
        return std::sin(angle) - (k + sides) * tangent * std::cos(angle);
    };

    int k = 0;
    while (threshold_function(k) < 0.0) {
        ++k;
    }
    return k - 1;
}

double critical_speed_regular_polygon(int sides) {
    const double pi = std::acos(-1.0);
    const double theta = pi / static_cast<double>(sides);
    const double tangent = std::tan(theta);
    const int k = cutoff_index_regular_polygon(sides);

    const double angle = k * theta;
    // Theorem 2 gives V_n = sec(alpha).
    const double correction = std::acos(
        2.0 * std::sin(angle) / ((k + sides) * tangent) - std::cos(angle)
    );
    const double alpha = (angle + correction) / 2.0;
    return 1.0 / std::cos(alpha);
}

std::string solve() {
    std::ostringstream output;
    output << std::fixed << std::setprecision(8)
           << critical_speed_regular_polygon(6);
    return output.str();
}

int main() {
    std::cout << solve() << '\n';
    return 0;
}
