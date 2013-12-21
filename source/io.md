#入出力

基本はcin,coutを使おう。

##cin,cout
iostream,iomanipをincludeしておくこと。

### cin
基本的な使い方について。

~~~~~~{.cpp}
int n;
cin >> n;
vector<int> V(n);
rep(i,n) cin >> V[i];
~~~~~~
こうすると、短く書ける。

入力の最後まで読む。

~~~~~~{.cpp}
int n;
while(cin >> n){
    //処理
}
~~~~~~
n=0のとき終わりとかの場合は、条件に&&n!=0とかをつける。

数値をカンマ区切りで読み込む。

~~~~~~{.cpp}
int x,y;char c;
//cにカンマが入る
cin >> x >> c >> y;
~~~~~~
冗長かもだけど、一番楽。

空白とか含めて一行読む。

~~~~~~{.cpp}
string s;
getline(cin,s);
~~~~~~

改行文字は、sに入らず、かつ読み捨てされる。
cinでは、改行文字は読み捨てないことに注意しよう。
つまり、数値\<改行\>文字列\<改行\>を読みたいときには、

~~~~~~{.cpp}
int n;string s;
// 数値
cin >> n;
// 改行よみとばし
cin.ignore();
// 文字列
getline(cin,s);
~~~~~~

とする。cinは改行文字を残すので、ignoreでそれを読み捨てないといけない。
また、ignoreの引数は読み捨てる文字数。引数なしの場合は1を渡したのと同等
の効果がある。

### cout

有効数字等が設定されている問題は、必ず多めに出力すること。多めに出す分に
は大丈夫。

基本の使い方

~~~~~~{.cpp}
int n;vector<int> V(n);
cout << n << endl;
rep(i,n) cout << V[i] << " ";
cout << endl;
~~~~~~

以下主なiomanipの使い方

~~~~~~{.cpp}
int n = 123;double d = 1.23;

//10進数 −> 123
cout << dec << n << endl ;

//8進数 −> 173
cout << oct << n << endl ;

//16進数 −> 7b
cout << hex << n << endl ;

//16進数かつ、大文字 −> 7B
cout << hex << uppercase << n << endl;

//10進数に戻す
cout << dec;

//幅が10になるようにする。デフォルトは右寄せ
// -> xxxxxxx123 (default)
cout << setfill('x') << setw(10) << right << n << endl;

// -> 123xxxxxxx
cout << setfill('x') << setw(10) << left << n << endl;
// -> 123yyyyyyy
cout << setfill('y') << setw(10) << n << endl;

//小数点以下10桁表示に。
cout << fixed << setprecision(10);

// -> 1.2300000000
cout << d << endl;
// -> 12.3000000000
cout << 10*d << endl;

//小数点の表示を元に戻す
cout.unsetf(ios::fixed);
// -> 1.23
cout << d << endl;
~~~~~~

基本的には、引数のあるマニピュレータの効果は保存される。

##scanf,printf

C++では、cstdioをinclude。複雑な書式とかが必要なときにはこっちを使うといいかもしれない。

### scanf

基本的な使い方

~~~~~~{.cpp}
int n;char tmp[256];
scanf("%d\n",&n);
gets(tmp);
~~~~~~

stringに直接いれるのはだめ。scanfはcinと同様に改行を残す。getlineするな
らcin.ignore。getsするなら、直前のscanfで改行を読んでおく必要がある。
また、scanfで改行を読むのでなく、直後にgetc(stdin)してもいい。

### printf

基本的な使い方

~~~~~~{.cpp}
int n = 100;
printf("n is %d\n",n);
~~~~~~

scanfとほとんど同様の使い方ができる。

指定子   出力書式
-------- ---------------------------
%c       文字
%s       文字列
%d       符号付き10進整数
%u       符号なし10進
%f       10進浮動小数点数
%o       符号なし8進
%x       符号なし16進(Xなら大文字)
%%       %記号

Table: 書式指定子

## 高速化


以下のコードをmain関数の最初に書くことで、cin,coutの速度が2倍程度になる。

~~~~~~{.cpp}
ios::sync_with_stdio(false);
cin.tie(0);
~~~~~~
ただし、このコードはstdioとの同期を切るという意味なので、これを使うとき
にはprintfやscanfを使用してはだめ。
