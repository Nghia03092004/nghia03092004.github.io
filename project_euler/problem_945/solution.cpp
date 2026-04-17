#include <bits/stdc++.h>
using namespace std;
vector<int> primes;
map<pair<long long,int>,long long> memo;
long long psi(long long x, int idx){
    if(x<1) return 0;
    if(idx<0) return 1;
    auto key=make_pair(x,idx);
    if(memo.count(key)) return memo[key];
    long long r=psi(x,idx-1);
    long long pa=primes[idx];
    while(pa<=x){r+=psi(x/pa,idx-1);pa*=primes[idx];}
    return memo[key]=r;
}
int main(){
    int B=100;
    vector<bool> s(B+1,true); s[0]=s[1]=false;
    for(int i=2;i*i<=B;i++) if(s[i]) for(int j=i*i;j<=B;j+=i) s[j]=false;
    for(int i=2;i<=B;i++) if(s[i]) primes.push_back(i);
    long long x=10000000000LL;
    cout<<psi(x,(int)primes.size()-1)<<endl;
}
