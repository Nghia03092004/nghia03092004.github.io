#include <bits/stdc++.h>
using namespace std;
int main() {
    const int N = 1000000;
    vector<bool> is_p(N, true); is_p[0]=is_p[1]=false;
    for (int i=2;(long long)i*i<N;i++) if(is_p[i]) for(int j=i*i;j<N;j+=i) is_p[j]=false;
    set<int> small_p;
    for (int i=2;i<55;i++) if(is_p[i]) small_p.insert(i);
    int cnt=0;
    for(int p=2;p<N;p++){
        if(!is_p[p]) continue;
        int ds=0,t=p; while(t){ds+=t%10;t/=10;}
        if(small_p.count(ds)) cnt++;
    }
    cout<<cnt<<endl;
    return 0;
}
