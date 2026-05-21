#include <algorithm>
#include <array>
#include <iostream>
#include <tuple>
#include <vector>

using namespace std;

using Matrix = array<array<long long, 3>, 3>;

const Matrix matrices[3] = {
    {{{1, -2, 2}, {2, -1, 2}, {2, -2, 3}}},
    {{{1, 2, 2}, {2, 1, 2}, {2, 2, 3}}},
    {{{-1, 2, 2}, {-2, 1, 2}, {-2, 2, 3}}},
};

tuple<long long, long long, long long> transform(const Matrix& matrix,
                                                 long long a,
                                                 long long b,
                                                 long long c) {
    long long x = matrix[0][0] * a + matrix[0][1] * b + matrix[0][2] * c;
    long long y = matrix[1][0] * a + matrix[1][1] * b + matrix[1][2] * c;
    long long z = matrix[2][0] * a + matrix[2][1] * b + matrix[2][2] * c;
    return {x, y, z};
}

int main() {
    const long long limit = 75000000LL;
    long long count = 0;
    vector<tuple<long long, long long, long long>> stack = {{2, 2, 3}};

    while (!stack.empty()) {
        long long a0, b0, c;
        tie(a0, b0, c) = stack.back();
        stack.pop_back();

        long long a = min(a0, b0);
        long long b = max(a0, b0);

        if (a + b + c > limit) {
            continue;
        }

        count++;
        int childCount = (a == b ? 2 : 3);
        for (int i = 0; i < childCount; i++) {
            long long x0, y0, z;
            tie(x0, y0, z) = transform(matrices[i], a, b, c);
            if (x0 <= 0 || y0 <= 0 || z <= 0) {
                continue;
            }

            long long x = min(x0, y0);
            long long y = max(x0, y0);
            stack.push_back(make_tuple(x, y, z));
        }
    }

    cout << count << '\n';
    return 0;
}
