# 文字列操作

stringをincludeする。cctypeもいるかも。

## std::string
charをラップしたテンプレートクラス。 基本的な使い方について

### 部分列
~~~~~~{.cpp}
          //0123456789
string str("abcdefghij");
// 5番目以降
str.substr(5);    // "fghij"
// 5番目から3つ
str.substr(5,3); // "fgh"
//全部小文字にする
transform(s.begin(),s.end(),s.begin(),::tolower);
~~~~~~

substrは一つの引数でそこから先全部、二つの場合は第一引数の位置から、第二
引数の数だけ持ってくる。

### 検索

stringには、いくつかのfindが定義されている。線形検索なので、早い検索が必
要なときには後述するKMP法やBM法を用いること。

-   find 引数が最初に現れる位置

-   rfind 引数が最後に表われる位置

-   find_first_of 引数の文字のどれかが最初に表われる位置

-   find_last_of 引数の文字のどれかが最後に表われる位置

-   find_first_not_of
    引数の文字のどれかではない文字が最初に表われる位置

-   find_first_not_of
    引数の文字のどれかではない文字が最後に表われる位置

第二引数として探すための最初の位置を指定できる。

#### Boyer Moore法


#### KMP法


## stringstream

cinやcoutのようなstreamをstringを元に作成したりする。基本的には、string
をフォーマットしたり、intやlongに、intやlongから変換するために使用する。

~~~~~~{.cpp}
stringstream ss;
ss << 102;
string s;
ss >> s;
~~~~~~

## 再帰下降構文解析
