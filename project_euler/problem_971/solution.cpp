#include <bits/stdc++.h>
using namespace std;
int main() {
    const int N = 500, M = N*N*2;
    vector<bool> is_p(M+1, true); is_p[0]=is_p[1]=false;
    for(int i=2;(long long)i*i<=M;i++) if(is_p[i]) for(int j=i*i;j<=M;j+=i) is_p[j]=false;
    int cnt = 0;
    for(int a=1;a<=N;a++) for(int b=1;b<=N;b++) if(is_p[a*a+b*b]) cnt++;
    for(int a=1;a<=N;a++) if(is_p[a] && a%4==3) cnt += 2;
    cout << cnt << endl;
    return 0;
}
