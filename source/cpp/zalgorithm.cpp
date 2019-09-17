// z-algorithm (O(|S|)
// Return V where V[i] = length of the largest common prefix of S and S[i]
// Example:
//  S: aaabaaaab
//  V: 921034210
// Ref: https://ivanyu.me/blog/2013/10/15/z-algorithm/
vector<int> zAlgorithm(const std::string& s)
{
    std::vector<int> z(s.size());
    z[0] = s.size();

    // left-bound, right-bound of current z-box.
    int lt = 0, rt = 0;

    for (int k = 1; k < s.size(); k++) {
        if (k > rt) {
            // If k is outside the current z-box, then we do naive computation.
            int n = 0;
            while (n + k < s.size() && s[n] == s[n + k])
                n++;
            z[k] = n;
            if (n > 0) {
                lt = k;
                rt = k + n - 1;
            }
        } else {
            // If k is inside the current z-box, then consider two cases
            int p = k - lt; // pair index
            // rightPartLen is a length of k..rt 
            int rightPartLen = rt - k + 1;

            if (z[p] < rightPartLen) {
                z[k] = z[p];
            } else {
                // s[i - k] is a A[i] where A starts from k of S.
                int i = rt + 1;
                while (i < s.size() && s[i] == s[i - k])
                    i++;
                z[k] = i - k;
                lt = k;
                rt = i - 1;
            }
        }
    }
    return z;
}
