#include <algorithm>
#include <cassert>
#include <iostream>
#include <numeric>

long long count_right_triangles(int n) {
    long long total = 3LL * n * n;

    for (int x = 1; x <= n; ++x) {
        for (int y = 1; y <= n; ++y) {
            const int g = std::gcd(x, y);
            const int step_x = y / g;
            const int step_y = x / g;

            total += std::min((n - x) / step_x, y / step_y);
            total += std::min(x / step_x, (n - y) / step_y);
        }
    }

    return total;
}

int main() {
    const long long answer = count_right_triangles(50);
    assert(answer == 14234);
    std::cout << answer << '\n';
    return 0;
}
