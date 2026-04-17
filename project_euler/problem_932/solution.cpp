#include <bits/stdc++.h>
using namespace std;
int main(){
    const int LIM=10000;
    set<long long> tri;
    for(long long n=1;n*(n+1)/2<=2LL*LIM*(LIM+1);n++) tri.insert(n*(n+1)/2);
    int cnt=0;
    for(int a=1;a<=LIM;a++){
        long long ta=(long long)a*(a+1)/2;
        for(int b=a+1;b<=LIM;b++){
            long long tb=(long long)b*(b+1)/2;
            if(tri.count(ta+tb)) cnt++;
        }
    }
    cout<<cnt<<endl;
    return 0;
}
