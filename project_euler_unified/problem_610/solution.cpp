#include <bits/stdc++.h>
using namespace std;

string int_to_roman(int n) {
    int vals[] = {1000,900,500,400,100,90,50,40,10,9,5,4,1};
    string syms[] = {"M","CM","D","CD","C","XC","L","XL","X","IX","V","IV","I"};
    string result;
    for (int i = 0; i < 13; i++)
        while (n >= vals[i]) { result += syms[i]; n -= vals[i]; }
    return result;
}

int main() {
    int max_len = 0;
    for (int n = 1; n < 4000; n++) {
        int len = int_to_roman(n).size();
        max_len = max(max_len, len);
    }
    cout << "Max Roman numeral length: " << max_len << endl;
    return 0;
}
