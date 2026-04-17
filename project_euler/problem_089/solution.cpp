#include <bits/stdc++.h>
using namespace std;

int romanToInt(const string& s) {
    map<char, int> val = {
        {'I', 1}, {'V', 5}, {'X', 10}, {'L', 50},
        {'C', 100}, {'D', 500}, {'M', 1000}
    };
    int result = 0;
    for (int i = 0; i < (int)s.size(); i++) {
        if (i + 1 < (int)s.size() && val[s[i]] < val[s[i + 1]])
            result -= val[s[i]];
        else
            result += val[s[i]];
    }
    return result;
}

string intToRoman(int n) {
    const int values[] = {1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1};
    const string symbols[] = {"M", "CM", "D", "CD", "C", "XC", "L", "XL", "X", "IX", "V", "IV", "I"};
    string result;
    for (int i = 0; i < 13; i++) {
        while (n >= values[i]) {
            result += symbols[i];
            n -= values[i];
        }
    }
    return result;
}

int main() {
    // The roman numerals from Project Euler problem 89
    // Since we cannot download the file, we embed the known answer logic.
    // The file contains 1000 Roman numerals, one per line.
    // We read from stdin or a file.

    // Try reading from file first, then stdin
    ifstream fin("p089_roman.txt");
    istream& in = fin.is_open() ? fin : cin;

    int totalSaved = 0;
    string line;
    while (getline(in, line)) {
        // Remove trailing whitespace
        while (!line.empty() && (line.back() == '\r' || line.back() == '\n' || line.back() == ' '))
            line.pop_back();
        if (line.empty()) continue;

        int val = romanToInt(line);
        string minimal = intToRoman(val);
        totalSaved += (int)line.size() - (int)minimal.size();
    }

    cout << totalSaved << endl;
    return 0;
}
