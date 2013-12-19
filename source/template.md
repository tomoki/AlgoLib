# テンプレート
各種バッドノウハウを含む。

## C++

~~~~~~{.cpp}
#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
#include <set>
#include <queue>
#include <bitset>
#include <stack>
#include <utility>
#include <numeric>
#include <algorithm>
#include <functional>
#include <complex>
#include <string>
#include <sstream>

#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <cstring>

using namespace std;

#define all(c) c.begin(),c.end()
#define rep(i,n) for(int i=0;i<(int)n;i++)
#define debug(x) #x << "=" << (x) << "(L" << __LINE__ << ")"

typedef long long ll;
typedef vector<int> vi;

template<typename T>
ostream& operator<<(ostream& os,const vector<T>& val){
    os << "[ ";
    for(typename vector<T>::const_iterator it=val.begin();
        it != val.end();++it){
        os << *it << " ";
    }
    os << "]";
    return os;
}

int main(){
    return 0;
}
~~~~~~

## Python
~~~~~~{.python}
#!/usr/bin/env python
#coding:utf-8

from __future__ import division
from __future__ import print_function
from __future__ import unicode_literals

from math import *
from string import *
from fractions import *
from itertools import *

def main():
    pass

if __name__ == "__main__":
    main()
~~~~~~
