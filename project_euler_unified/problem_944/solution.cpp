#include <bits/stdc++.h>
using namespace std;
int main(){
    int n=100;
    // V(n) = n*(n-1)*(2n+5)/72
    long long num=(long long)n*(n-1)*(2*n+5);
    cout<<num/72<<endl;
    return 0;
}
