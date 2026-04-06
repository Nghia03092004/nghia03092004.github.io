#include <bits/stdc++.h>
using namespace std;
typedef vector<vector<long long>> Mat;
const long long MOD=1e9+7;
Mat mul(Mat&A,Mat&B){
    int n=A.size();
    Mat C(n,vector<long long>(n,0));
    for(int i=0;i<n;i++) for(int k=0;k<n;k++) if(A[i][k])
        for(int j=0;j<n;j++) C[i][j]=(C[i][j]+A[i][k]*B[k][j])%MOD;
    return C;
}
Mat mpow(Mat M,long long p){
    int n=M.size();
    Mat R(n,vector<long long>(n,0));
    for(int i=0;i<n;i++) R[i][i]=1;
    while(p>0){if(p&1) R=mul(R,M); M=mul(M,M); p>>=1;}
    return R;
}
long long fib(long long n){
    if(n<=0) return 0;
    if(n==1) return 1;
    Mat M={{1,1},{1,0}};
    Mat R=mpow(M,n-1);
    return R[0][0];
}
int main(){
    long long N=1000000000000LL;
    long long fn2=fib(N+2), fn3=fib(N+3);
    long long ans=((N%MOD)*fn2%MOD-fn3+2+2*MOD)%MOD;
    cout<<ans<<endl;
}
