# 動的計画法およびそれに似たやつら。(TODO)
## LCS
Longest common sequence.

## LIS
Longest increasing subsequence.

## 巡回セールスマン問題
bit演算をする。bitのループを先に回すこと。$O(N^2\times2^{N})$

~~~~~~{.cpp}
ll dp[1 << N][N];
dp[0][0] = 1;
// Bitのほうが先!
for(int i=0;i<(1<<N);i++){
    for(int j=0;j<N;j++){
        //something.
    }
}
~~~~~~~
