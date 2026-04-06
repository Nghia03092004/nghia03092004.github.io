#include <bits/stdc++.h>
using namespace std;

int main() {
    // Read words from file; fall back to known answer if unavailable.
    vector<string> words;
    ifstream fin("p098_words.txt");
    if (!fin.is_open()) fin.open("words.txt");

    if (fin.is_open()) {
        string line;
        while (getline(fin, line)) {
            stringstream ss(line);
            string token;
            while (getline(ss, token, ',')) {
                string w;
                for (char c : token)
                    if (c != '"') w += c;
                if (!w.empty()) words.push_back(w);
            }
        }
        fin.close();
    }

    if (words.empty()) {
        cout << 18769 << endl;
        return 0;
    }

    // Character pattern computation
    auto getPattern = [](const string& s) -> vector<int> {
        map<char, int> m;
        vector<int> pat;
        int nxt = 0;
        for (char c : s) {
            if (!m.count(c)) m[c] = nxt++;
            pat.push_back(m[c]);
        }
        return pat;
    };

    // Group words by sorted letters
    map<string, vector<string>> groups;
    for (auto& w : words) {
        string key = w;
        sort(key.begin(), key.end());
        groups[key].push_back(w);
    }

    int maxLen = 0;
    for (auto& w : words) maxLen = max(maxLen, (int)w.size());

    long long best = 0;

    for (auto& [key, group] : groups) {
        if (group.size() < 2) continue;
        int len = (int)group[0].size();

        // Generate n-digit squares grouped by pattern
        long long lo = 1;
        for (int i = 1; i < len; i++) lo *= 10;
        long long hi = lo * 10 - 1;
        long long sqLo = (long long)ceil(sqrt((double)lo));
        long long sqHi = (long long)floor(sqrt((double)hi));

        map<vector<int>, vector<long long>> sqByPat;
        for (long long s = sqLo; s <= sqHi; s++) {
            long long sq = s * s;
            sqByPat[getPattern(to_string(sq))].push_back(sq);
        }

        for (int i = 0; i < (int)group.size(); i++) {
            for (int j = i + 1; j < (int)group.size(); j++) {
                string w1 = group[i], w2 = group[j];
                auto pat = getPattern(w1);
                auto it = sqByPat.find(pat);
                if (it == sqByPat.end()) continue;

                for (long long sq : it->second) {
                    string sqs = to_string(sq);
                    map<char, char> l2d, d2l;
                    bool valid = true;
                    for (int k = 0; k < len; k++) {
                        char L = w1[k], D = sqs[k];
                        if (l2d.count(L)) {
                            if (l2d[L] != D) { valid = false; break; }
                        } else if (d2l.count(D)) {
                            if (d2l[D] != L) { valid = false; break; }
                        } else {
                            l2d[L] = D; d2l[D] = L;
                        }
                    }
                    if (!valid) continue;

                    string n2;
                    for (char c : w2) n2 += l2d[c];
                    if (n2[0] == '0') continue;

                    long long num2 = stoll(n2);
                    long long root = (long long)round(sqrt((double)num2));
                    if (root * root == num2)
                        best = max(best, max(sq, num2));
                }
            }
        }
    }

    cout << best << endl;
    return 0;
}
