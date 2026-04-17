#include <bits/stdc++.h>
using namespace std;

bool is_pandigital(int a, int b, int c) {
    int digits[10] = {};
    for (int x : {a, b, c})
        while (x > 0) { digits[x % 10]++; x /= 10; }
    if (digits[0] != 0) return false;
    for (int i = 1; i <= 9; i++)
        if (digits[i] != 1) return false;
    return true;
}

int main() {
    set<int> products;
    for (int a = 1; a <= 9; a++)
        for (int b = 1234; b <= 9876; b++) {
            int p = a * b;
            if (p >= 1000 && p <= 9999 && is_pandigital(a, b, p))
                products.insert(p);
        }
    for (int a = 12; a <= 98; a++)
        for (int b = 123; b <= 987; b++) {
            int p = a * b;
            if (p >= 1000 && p <= 9999 && is_pandigital(a, b, p))
                products.insert(p);
        }
    int sum = 0;
    for (int p : products) sum += p;
    cout << sum << endl;  // 45228
    return 0;
}
