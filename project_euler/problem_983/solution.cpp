#include <bits/stdc++.h>
using namespace std;
int main() {
    const int N = 1000000;
    vector<bool> is_p(N, true); is_p[0]=is_p[1]=false;
    for(int i=2;(long long)i*i<N;i++) if(is_p[i]) for(int j=i*i;j<N;j+=i) is_p[j]=false;
    map<string,int> groups;
    for(int p=2;p<N;p++){
        if(!is_p[p]) continue;
        string s=to_string(p); sort(s.begin(),s.end());
        groups[s]++;
    }
    int cnt=0;
    for(auto&[k,v]:groups) if(v>=4) cnt++;
    cout<<cnt<<endl;
    return 0;
}
