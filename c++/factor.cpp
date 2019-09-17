#include <iostream>
#include <ctime>
#include <vector>
#include <algorithm>

using namespace std;

const int MAX = 10000000;

bool is_prime[MAX+1];
int min_factor[MAX+1];
vector<int> primes;

int main() {
    // Sieve of Eratosthenes
    fill_n(is_prime, MAX+1, true);
    is_prime[0] = is_prime[1] = false;
    min_factor[0] = 0;
    min_factor[1] = 1;
    for(int i = 2; i <= MAX; ++i) {
        if(is_prime[i]) {
            min_factor[i] = i;
            primes.push_back(i);
            for(long long j = static_cast<long long>(i)*i; j <= MAX; j += i) {
                is_prime[j] = false;
                if(min_factor[j] == 0) min_factor[j] = i;
            }
        }
    }

    ////////// Factoring by division loop //////////
    {
        long long sum = 0;
        clock_t start = clock();
        for(int i = 2; i <= MAX; ++i) {
            int tmp = i;
            for(int p : primes) {
                while(tmp % p == 0) {
                    sum += p;
                    tmp /= p;
                }
                if(tmp == 1) break;
                if(is_prime[tmp]) {
                    sum += tmp;
                    break;
                }
            }
        }
        clock_t end = clock();
        cout << "Factoring by division loop:" << endl;
        cout << "\tValue: " << sum << endl;
        cout << "\tTime: " << static_cast<double>(end-start) / CLOCKS_PER_SEC << endl;
    }
    ////////// Factoring by min_factor //////////
    {
        long long sum = 0;
        clock_t start = clock();
        for(int i = 2; i <= MAX; ++i) {
            int tmp = i;
            while(tmp > 1) {
                const int fact = min_factor[tmp];
                sum += fact;
                tmp /= fact;
            }
        }
        clock_t end = clock();
        cout << "Factoring by min_factor:" << endl;
        cout << "\tValue: " << sum << endl;
        cout << "\tTime: " << static_cast<double>(end-start) / CLOCKS_PER_SEC << endl;
    }
    return 0;
}
