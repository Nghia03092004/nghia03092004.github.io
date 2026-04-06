#include <bits/stdc++.h>
using namespace std;
const long long MOD=1e9+7;
long long pw(long long a,long long b,long long m){long long r=1;a%=m;while(b>0){if(b&1)r=r*a%m;a=a*a%m;b>>=1;}return r;}
int main(){
    int N=4001;
    vector<long long> f(N),fi(N);
    f[0]=1;for(int i=1;i<N;i++) f[i]=f[i-1]*i%MOD;
    fi[N-1]=pw(f[N-1],MOD-2,MOD);
    for(int i=N-2;i>=0;i--) fi[i]=fi[i+1]*(i+1)%MOD;
    auto C=[&](int n,int k)->long long{
        if(k<0||k>n) return 0;
        return f[n]%MOD*fi[k]%MOD*fi[n-k]%MOD;
    };
    long long a=C(4000,2000), b=C(2000,1000);
    long long ans=(a+b%MOD*b%MOD)%MOD*pw(2,MOD-2,MOD)%MOD;
    cout<<ans<<endl;
}
