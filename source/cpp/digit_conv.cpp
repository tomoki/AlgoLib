vector<int> int_to_digits(int base,int N){
    vector<int> rev_ret;
    while(N != 0){
        rev_ret.push_back(N % base);
        N /= base;
    }
    reverse(all(rev_ret));
    return rev_ret;
}

int digits_to_int(int base,const vector<int>& digits){
    int ret = 0;
    for(int i=0;i<(int)digits.size();i++){
        ret *= base;
        ret += digits[i];
    }
    return ret;
}
