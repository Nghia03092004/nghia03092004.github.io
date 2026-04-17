#include <bits/stdc++.h>
using namespace std;

int main() {
    ifstream fin("names.txt");
    if (!fin.is_open()) {
        cerr << "Error: names.txt not found." << endl;
        return 1;
    }

    string content((istreambuf_iterator<char>(fin)), istreambuf_iterator<char>());
    fin.close();

    vector<string> names;
    string cur;
    bool in_quote = false;
    for (char c : content) {
        if (c == '"') {
            in_quote = !in_quote;
            if (!in_quote && !cur.empty()) {
                names.push_back(cur);
                cur.clear();
            }
        } else if (in_quote) {
            cur += c;
        }
    }

    sort(names.begin(), names.end());

    long long total = 0;
    for (int i = 0; i < (int)names.size(); i++) {
        int alpha = 0;
        for (char c : names[i])
            alpha += c - 'A' + 1;
        total += (long long)(i + 1) * alpha;
    }

    cout << total << endl;
    return 0;
}
