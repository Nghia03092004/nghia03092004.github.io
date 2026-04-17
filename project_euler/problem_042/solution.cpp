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
    vector<string> candidates = {
        "words.txt",
        "0042_words.txt",
        "project_euler/problem_042/words.txt",
        "project_euler/problem_042/0042_words.txt"
    };

    ifstream fin;
    for (const auto& path : candidates) {
        fin.open(path);
        if (fin.is_open()) {
            break;
        }
        fin.clear();
    }

    if (!fin.is_open()) {
        cerr << "Could not locate the Problem 42 word list." << endl;
        return 1;
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
