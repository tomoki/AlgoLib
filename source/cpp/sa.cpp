#include <chrono>
#include <iostream>
#include <algorithm>
using namespace std;
#define debug(x) #x << "=" << (x)
#define dump(x) std::cerr << debug(x) << " (L:" << __LINE__ << ")" << std::endl

unsigned long xor128(){
    static unsigned long x=123456789,y=362436069,z=521288629,w=88675123;
    unsigned long t;
    t=(x^(x<<11));x=y;y=z;z=w; return( w=(w^(w>>19))^(t^(t>>8)) );
}

signed main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    const chrono::milliseconds annealing_time(2000);
    const auto start_time = chrono::system_clock::now();
    const auto end_time = start_time + annealing_time;
    const int precision_for_force_next = 1000000;
    auto current_time = start_time;

    const auto score = [](double x){
        return max(-pow(-x+100,2)+100,-pow(-x+333,2)+333);
    };

    auto current_state = 0.0;     // initial state
    auto current_score = score(current_state);
    auto best_state = current_state;
    auto best_score = current_score;

    // chrono::system_clock should NOT be called many times...
    while((current_time=chrono::system_clock::now()) < end_time){
        const auto progress = 1.0*(current_time-start_time)/(end_time-start_time);
        const bool force_next = progress*precision_for_force_next
                              < xor128()%precision_for_force_next;

        // [0..100] -> [-50..50] balance is IMPORTANT.
        const auto next_state = current_state + xor128()%101-50;
        const auto next_score = score(next_state);
        if(force_next or (next_score > current_score)){
            current_state = next_state;
            current_score = next_score;
            dump(current_state);
            dump(current_score);
        }else{
            // undo change.
        }

        if(current_score > best_score){
            best_score = current_score;
            best_state = current_state;
            dump(best_score);
        }
    }
    dump(best_score);
    dump(best_state);

    return 0;
}
