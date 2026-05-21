#include <iostream>
#include <vector>

using namespace std;

int main() {
    const int first = 1864;
    const int last = 1909;

    vector<int> phi(last + 1);
    for (int i = 0; i <= last; i++) {
        phi[i] = i;
    }

    for (int value = 2; value <= last; value++) {
        if (phi[value] == value) {
            for (int multiple = value; multiple <= last; multiple += value) {
                phi[multiple] = phi[multiple] / value * (value - 1);
            }
        }
    }

    long long total = 1;
    for (int denominator = 2; denominator <= last; denominator++) {
        if (last / denominator > (first - 1) / denominator) {
            total += phi[denominator];
        }
    }

    cout << total << '\n';
    return 0;
}
