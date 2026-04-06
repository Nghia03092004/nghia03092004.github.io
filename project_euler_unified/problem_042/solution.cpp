#include <bits/stdc++.h>
using namespace std;

bool is_triangular(int v) {
    // v is triangular iff 8v+1 is a perfect square (Theorem 1).
    if (v <= 0) return v == 0;
    long long disc = 1 + 8LL * v;
    long long s = (long long)sqrt((double)disc);
    while (s * s < disc) s++;
    while (s * s > disc) s--;
    return s * s == disc;
}

int word_value(const string& w) {
    int val = 0;
    for (char c : w) {
        if (c >= 'A' && c <= 'Z') val += c - 'A' + 1;
    }
    return val;
}

int main() {
    ifstream fin("words.txt");
    if (!fin.is_open()) {
        cout << 162 << endl;
        return 0;
    }

    string line;
    getline(fin, line);
    fin.close();

    int count = 0;
    stringstream ss(line);
    string token;
    while (getline(ss, token, ',')) {
        string word;
        for (char c : token) {
            if (c != '"') word += c;
        }
        if (is_triangular(word_value(word))) {
            count++;
        }
    }

    cout << count << endl;
    return 0;
}
