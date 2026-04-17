#include <bits/stdc++.h>
using namespace std;
const long long MOD=1e9+7;
typedef vector<vector<long long>> Mat;
Mat mul(const Mat&A,const Mat&B){
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
    while(p>0){if(p&1)R=mul(R,M);M=mul(M,M);p>>=1;}
    return R;
}
int main(){
    int rows=3,cols=10,k=4;
    vector<vector<int>> valid;
    for(int a=0;a<k;a++) for(int b=0;b<k;b++) for(int c=0;c<k;c++)
        if(a!=b&&b!=c) valid.push_back({a,b,c});
    int C=valid.size();
    Mat T(C,vector<long long>(C,0));
    for(int i=0;i<C;i++) for(int j=0;j<C;j++){
        bool ok=true;
        for(int r=0;r<rows;r++) if(valid[i][r]==valid[j][r]){ok=false;break;}
        if(ok) T[i][j]=1;
    }
    Mat Tn=mpow(T,cols-1);
    long long ans=0;
    for(int i=0;i<C;i++) for(int j=0;j<C;j++) ans=(ans+Tn[i][j])%MOD;
    cout<<ans<<endl;
    return 0;
}
