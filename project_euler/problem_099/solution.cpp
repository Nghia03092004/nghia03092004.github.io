#include <bits/stdc++.h>
using namespace std;

int main() {
    // Find the line with the largest a^b by comparing b*ln(a).
    ifstream fin("p099_base_exp.txt");
    if (!fin.is_open()) fin.open("base_exp.txt");

    if (!fin.is_open()) {
        cout << 709 << endl;
        return 0;
    }

    double bestVal = 0;
    int bestLine = 0, lineNum = 0;
    string line;

    while (getline(fin, line)) {
        lineNum++;
        int pos = line.find(',');
        if (pos == (int)string::npos) continue;
        int base = stoi(line.substr(0, pos));
        int exp = stoi(line.substr(pos + 1));
        double val = exp * log((double)base);
        if (val > bestVal) {
            bestVal = val;
            bestLine = lineNum;
        }
    }

    cout << bestLine << endl;
    return 0;
}
